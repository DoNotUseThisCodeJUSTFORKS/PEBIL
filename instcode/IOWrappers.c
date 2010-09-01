/* 
 * This file is part of the pebil project.
 * 
 * Copyright (c) 2010, University of California Regents
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef PRELOAD_WRAPPERS
#define _GNU_SOURCE
#include <dlfcn.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <InstrumentationCommon.h>
#include <IOWrappers.h>

char* appName;
int32_t* currentSiteIndex;
char** fileNames;
int32_t* lineNumbers;

// to handle trace buffering
TraceBuffer_t traceBuffer = { NULL, __IO_BUFFER_SIZE, 0, 0 };
uint32_t callDepth = 0;
uint64_t eventIndex = 0;
uint32_t fileRegSeq = 0x400;
uint32_t activeTrace = 1;

// the dump function makes IO calls. so we must protect from an infinite recursion by not entering
// any buffer function if one is already stacked
uint32_t iowrapperDepth = 0;
#define CALL_DEPTH_ENTER(__var) if (__var) { return 0; } __var++;
#define CALL_DEPTH_EXIT(__var) __var--;

// to handle timers
int64_t timerstart;
int64_t timerstop;
#define TIMER_START (timerstart = readtsc())
#define TIMER_STOP  (timerstop  = readtsc())
#define TIMER_VALUE (timerstop - timerstart)
#define TIMER_EXECUTE(__stmts) TIMER_START; __stmts TIMER_STOP; 
#define PRINT_TIMER(__file) PRINT_INSTR(__file, "timer value (in cycles): %lld", TIMER_VALUE)

extern void printSystemIORecords();

uint32_t dumpBuffer(){
    if (activeTrace){
        if (traceBuffer.outFile == NULL){
            char fname[__MAX_STRING_SIZE];
            sprintf(fname, "pebiliotrace.rank%05d.tasks%05d.bin", __taskid, __ntasks);
            traceBuffer.outFile = fopen(fname, "w");
        }

        uint32_t oerr = fwrite(traceBuffer.buffer, sizeof(char), traceBuffer.freeIdx, traceBuffer.outFile);
        assert(oerr == traceBuffer.freeIdx);
    }
        
    traceBuffer.freeIdx = 0;
    assert(traceBuffer.freeIdx == 0);

    return traceBuffer.freeIdx;
}

uint32_t storeRecord(uint8_t type, uint32_t size){

    uint32_t headerSz = sizeof(uint32_t);
    uint32_t header = RECORD_HEADER(type, size);
    if (headerSz >= traceBuffer.size - traceBuffer.freeIdx){
        dumpBuffer();
    }
    assert(headerSz < traceBuffer.size - traceBuffer.freeIdx);
    memcpy(&(traceBuffer.buffer[traceBuffer.freeIdx]), &header, headerSz);
    traceBuffer.freeIdx += headerSz;

    return headerSz;
}

uint32_t storeFileName(char* name, uint64_t handle, uint8_t class, uint8_t type, uint8_t protect, uint32_t comm){
    if (protect) { CALL_DEPTH_ENTER(iowrapperDepth); }

    IOFileName_t filereg;
    bzero(&filereg, sizeof(IOFileName_t));

    filereg.handle_class = class;
    filereg.access_type = type;

    char myname[__MAX_MESSAGE_SIZE];
    sprintf(myname, "%s\0", name);
    filereg.numchars = strlen(myname)+1;

    if (class == IOFileAccess_ONCE){
        filereg.handle = fileRegSeq++;
    } else {
        filereg.handle = handle;
    }

    filereg.event_id = eventIndex;
    filereg.communicator = comm;

    uint32_t nameSz = sizeof(IOFileName_t) + filereg.numchars;
    storeRecord(IORecord_FileName, nameSz);

    if (nameSz >= traceBuffer.size - traceBuffer.freeIdx){
        dumpBuffer();
    }
    assert(nameSz < traceBuffer.size - traceBuffer.freeIdx);

    memcpy(&(traceBuffer.buffer[traceBuffer.freeIdx]), &filereg, sizeof(IOFileName_t));
    traceBuffer.freeIdx += sizeof(IOFileName_t);
    memcpy(&(traceBuffer.buffer[traceBuffer.freeIdx]), myname, strlen(myname) + 1);
    traceBuffer.freeIdx += strlen(myname)+1;

    if (protect) { CALL_DEPTH_EXIT(iowrapperDepth); }

    return filereg.handle;
}

uint32_t storeEventInfo(EventInfo_t* event){
    CALL_DEPTH_ENTER(iowrapperDepth);

    uint32_t eventSz = sizeof(EventInfo_t);
    storeRecord(IORecord_EventInfo, eventSz);

    event->unqid = eventIndex;

    // if traceBuffer is full dump it
    if (eventSz >= traceBuffer.size - traceBuffer.freeIdx){
        dumpBuffer();
    }
    assert(eventSz < traceBuffer.size - traceBuffer.freeIdx);

    memcpy(&(traceBuffer.buffer[traceBuffer.freeIdx]), event, eventSz);
    traceBuffer.freeIdx += eventSz;

    eventIndex++;
    CALL_DEPTH_EXIT(iowrapperDepth);

    return traceBuffer.freeIdx;
}

void printSystemIORecords(){
    storeFileName("stdout", stdout->_fileno, IOEventClass_CLIB, IOFileAccess_SYS, 0, PEBIL_NULL_COMMUNICATOR);
    eventIndex++;
    storeFileName("stderr", stderr->_fileno, IOEventClass_CLIB, IOFileAccess_SYS, 0, PEBIL_NULL_COMMUNICATOR);
    eventIndex++;
    storeFileName("stdin",  stdin->_fileno,  IOEventClass_CLIB, IOFileAccess_SYS, 0, PEBIL_NULL_COMMUNICATOR);
    eventIndex++;
}

// do any initialization here
// NOTE: on static-linked binaries, calling system/clib functions from here is a bad idea
int32_t _pebil_init(int32_t* indexLoc, char** fNames, int32_t* lNum){
    // at each call site we will put the index of the originating point in this location
    currentSiteIndex = indexLoc;
    fileNames = fNames;
    lineNumbers = lNum;

    _init_wrappers();
}

int32_t _pebil_fini(){
    _fini_wrappers();
}

#ifdef PRELOAD_WRAPPERS
// call these functions at program entry/exit
// this works for gcc; intel?
int32_t _init_wrappers() __attribute__ ((constructor));
int32_t _fini_wrappers() __attribute__ ((destructor));
#endif // PRELOAD_WRAPPERS

int32_t _init_wrappers(){
    CALL_DEPTH_ENTER(iowrapperDepth);
    PRINT_INSTR(stdout, "Starting IO Trace for task %d", __taskid);
    printSystemIORecords();    
    CALL_DEPTH_EXIT(iowrapperDepth);
}

int32_t _fini_wrappers(){
    CALL_DEPTH_ENTER(iowrapperDepth);
    PRINT_INSTR(stdout, "Finishing IO Trace for task %d, dumping buffer", __taskid);
    dumpBuffer();
    activeTrace = 0;
    fclose(traceBuffer.outFile);
    CALL_DEPTH_EXIT(iowrapperDepth);
}

#ifdef HAVE_MPI
#define WRAPPING_MPIO_IO
#endif // HAVE_MPI
#define WRAPPING_CLIB_IO
#define WRAPPING_POSX_IO

#include <IOEvents.c>
