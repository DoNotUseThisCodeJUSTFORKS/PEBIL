#ifndef _Base_h_
#define _Base_h_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <CStructuresElf.h>
#include <CStructuresDwarf.h>

typedef void (*fprintf_ftype)(FILE*, const char*, ...);

#define WARNING_SEVERITY 4
//#define DEVELOPMENT
//#define DEBUG_OPERAND
//#define DEBUG_OPTARGET
//#define DEBUG_OPCODE
//#define DEBUG_HASH
//#define DEBUG_NOTE
//#define DEBUG_LINEINFO
//#define DEBUG_BASICBLOCK
//#define DEBUG_HASHCODE
//#define DEBUG_CFG
//#define DEBUG_LOOP
//#define DEBUG_INST

#define __MAX_STRING_SIZE 1024
#define __SHOULD_NOT_ARRIVE ASSERT(0 && "Control should not reach this point")
#define __FUNCTION_NOT_IMPLEMENTED PRINT_ERROR("Function %s in file %s not implemented", __FUNCTION__, __FILE__); __SHOULD_NOT_ARRIVE;

#define GET_FIELD_BASIS(__type,__field) virtual __type get_ ## __field() \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define GET_FIELD_CLASS(__type,__field) inline __type get_ ## __field() \
    { return (__type)(entry.__field); }
#define GET_FIELD_BASIS_A(__type,__field,__union) virtual __type get_ ## __union ## _ ## __field() \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define GET_FIELD_CLASS_A(__type,__field,__union) inline __type get_ ## __union ## _ ## __field() \
    { return (__type)(entry.__union.__field); }
#define GET(__field) get_ ## __field()
#define GET_A(__field,__union) get_ ## __union ## _ ## __field()


#define SET_FIELD_BASIS(__type,__field) virtual __type set_ ## __field(__type __value) \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define SET_FIELD_CLASS(__type,__field) inline __type set_ ## __field(__type __value) \
    { entry.__field = __value; return (__type)(entry.__field); }
#define SET_FIELD_BASIS_A(__type,__field,__union) virtual __type set_ ## __union ## _ ## __field(__type __value) \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define SET_FIELD_CLASS_A(__type,__field,__union) inline __type set_ ## __union ## _ ## __field(__type __value) \
    { entry.__union.__field = __value; return (__type)(entry.__union.__field); }
#define SET(__field,__value) set_ ## __field(__value)
#define SET_A(__field,__union,__value) set_ ## __union ## _ ## __field(__value)


#define INCREMENT_FIELD_BASIS(__type,__field) virtual __type increment_ ## __field(__type __value) \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define INCREMENT_FIELD_CLASS(__type,__field) inline __type increment_ ## __field(__type __value) \
    { entry.__field += __value; return (__type)(entry.__field); }
#define INCREMENT_FIELD_BASIS_A(__type,__field,__union) virtual __type increment_ ## __union ## _ ## __field(__type __value) \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define INCREMENT_FIELD_CLASS_A(__type,__field,__union) inline __type increment_ ## __union ## _ ## __field(__type __value) \
    { entry.__union.__field += __value; return (__type)(entry.__union.__field); }
#define INCREMENT(__field,__value) increment_ ## __field(__value)
#define INCREMENT_A(__field,__union,__value) increment_ ## __union ## _ ## __field(__value)


#define PRINT_ERROR(...) fprintf(stderr,"*********** ERROR : "); \
    fprintf(stderr, "At line %d in file %s, function %s\n", __LINE__, __FILE__,__FUNCTION__);    \
    fprintf(stderr,## __VA_ARGS__);                              \
    fprintf(stderr,"\n");                                \
    ASSERT(0); \
    exit(-1);

#ifdef WARNING_SEVERITY
#define PRINT_WARN(__severity,...)  if (__severity >= WARNING_SEVERITY){ \
    fprintf(stderr,"*** WARNING : ");                            \
    fprintf(stderr,## __VA_ARGS__);                              \
    fprintf(stderr,"\n");                                        \
    fflush(stderr); }
#else
#define PRINT_WARN(...)
#endif

#define PRINT_INFOR(...) fprintf(stdout,"Information : "); \
    fprintf(stdout,## __VA_ARGS__);                        \
    fprintf(stdout,"\n");                                  \
    fflush(stdout);

#define PRINT_INFO() fprintf(stdout,"Information : "); \
    fflush(stdout);

#define PRINT_OUT(...) fprintf(stdout,## __VA_ARGS__); \
    fflush(stdout);

#ifdef DEBUG_OPCODE
#define PRINT_DEBUG_OPCODE(...) fprintf(stdout,"OPCODE : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_OPCODE(...)
#endif

#ifdef DEBUG_OPERAND
#define PRINT_DEBUG_OPERAND(...) fprintf(stdout,"OPERAND : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_OPERAND(...)
#endif

#ifdef DEBUG_OPTARGET
#define PRINT_DEBUG_OPTARGET(...) fprintf(stdout,"OPTARGET : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_OPTARGET(...)
#endif

#ifdef DEBUG_HASH
#define PRINT_DEBUG_HASH(...) fprintf(stdout,"HASH : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_HASH(...)
#endif

#ifdef DEBUG_LINEINFO
#define PRINT_DEBUG_LINEINFO(...) fprintf(stdout,"LINEINFO : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_LINEINFO(...)
#endif

#ifdef DEBUG_BASICBLOCK
#define PRINT_DEBUG_BASICBLOCK(...) fprintf(stdout,"BASICBLOCK : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_BASICBLOCK(...)
#endif

#ifdef DEBUG_HASHCODE
#define PRINT_DEBUG_HASHCODE(...) fprintf(stdout,"HASHCODE : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_HASHCODE(...)
#endif

#ifdef DEBUG_CFG
#define PRINT_DEBUG_CFG(...) fprintf(stdout,"CFG : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_CFG(...)
#endif

#ifdef DEBUG_LOOP
#define PRINT_DEBUG_LOOP(...) fprintf(stdout,"LOOP : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_LOOP(...)
#endif

#ifdef DEBUG_INST
#define PRINT_DEBUG_INST(...) fprintf(stdout,"INST : "); \
    fprintf(stdout,## __VA_ARGS__); \
    fprintf(stdout,"\n"); \
    fflush(stdout);
#else
#define PRINT_DEBUG_INST(...)
#endif



#ifdef  DEVELOPMENT

#define PRINT_DEBUG(...) fprintf(stdout,"----------- DEBUG : "); \
                         fprintf(stdout,## __VA_ARGS__); \
                         fprintf(stdout,"\n"); \
                         fflush(stdout);


#define ASSERT(__str) assert(__str);
#define DEBUG(...) __VA_ARGS__
#define DEBUG_MORE(...)
#define TIMER(...) __VA_ARGS__
#define INNER_TIMER(...) __VA_ARGS__

#else

#define PRINT_DEBUG(...)
#define DEBUG(...)
#define DEBUG_MORE(...)
#define ASSERT(__str) assert(__str);
#define TIMER(...) __VA_ARGS__
#define INNER_TIMER(...) 

#endif

#define Invalid_UInteger_ID                 (uint32_t)-1
#define Size__32_bit_File_Header            sizeof(Elf32_Ehdr)
#define Size__64_bit_File_Header            sizeof(Elf64_Ehdr)
#define Size__32_bit_Program_Header         sizeof(Elf32_Phdr)
#define Size__64_bit_Program_Header         sizeof(Elf64_Phdr)
#define Size__32_bit_Section_Header         sizeof(Elf32_Shdr)
#define Size__64_bit_Section_Header         sizeof(Elf64_Shdr)
#define Size__32_bit_Symbol                 sizeof(Elf32_Sym)
#define Size__64_bit_Symbol                 sizeof(Elf64_Sym)
#define Size__32_bit_Relocation             sizeof(Elf32_Rel)
#define Size__64_bit_Relocation             sizeof(Elf64_Rel)
#define Size__32_bit_Relocation_Addend      sizeof(Elf32_Rela)
#define Size__64_bit_Relocation_Addend      sizeof(Elf64_Rela)
#define Size__32_bit_Global_Offset_Table_Entry sizeof (uint32_t)
#define Size__64_bit_Global_Offset_Table_Entry sizeof (uint64_t)
#define Size__32_bit_Dynamic_Entry          sizeof(Elf32_Dyn)
#define Size__64_bit_Dynamic_Entry          sizeof(Elf64_Dyn)
#define Size__32_bit_Hash_Entry             sizeof(uint32_t)
#define Size__64_bit_Hash_Entry             sizeof(uint32_t)
#define Size__32_bit_GNU_Hash_Bloom_Entry   sizeof(uint32_t)
#define Size__64_bit_GNU_Hash_Bloom_Entry   sizeof(uint64_t)
#define Size__32_bit_Note_Section_Entry     sizeof(uint32_t)
#define Size__64_bit_Note_Section_Entry     sizeof(uint32_t)
#define Size__32_bit_Gnu_Verneed            sizeof(Elf32_Verneed)
#define Size__64_bit_Gnu_Verneed            sizeof(Elf64_Verneed)
#define Size__32_bit_Gnu_Vernaux            sizeof(Elf32_Vernaux)
#define Size__64_bit_Gnu_Vernaux            sizeof(Elf64_Vernaux)
#define Size__32_bit_Gnu_Versym             sizeof(uint16_t)
#define Size__64_bit_Gnu_Versym             sizeof(uint16_t)
#define Size__Dwarf_LineInfo_Header         sizeof(DWARF2_Internal_LineInfo)
#define Size__Dwarf_LineInfo                sizeof(DWARF2_LineInfo_Registers)


#define Print_Code_All                      0x00000001
#define Print_Code_FileHeader               0x00000002
#define Print_Code_SectionHeader            0x00000004
#define Print_Code_ProgramHeader            0x00000008
#define Print_Code_StringTable              0x00000010
#define Print_Code_SymbolTable              0x00000020
#define Print_Code_NoteSection              0x00000040
#define Print_Code_RelocationTable          0x00000080
#define Print_Code_GlobalOffsetTable        0x00000100
#define Print_Code_HashTable                0x00000200
#define Print_Code_DynamicTable             0x00000400
#define Print_Code_GnuVersymTable           0x00000800
#define Print_Code_GnuVerneedTable          0x00001000
#define Print_Code_Disassemble              0x00002000
#define Print_Code_Instruction              0x00004000
#define Print_Code_Instrumentation          0x00008000
#define Print_Code_DwarfSection             0x00010000
#define Print_Code_Loops                    0x00020000

#define HAS_PRINT_CODE(__value,__Print_Code) ((__value & __Print_Code) || (__value & Print_Code_All))
#define SET_PRINT_CODE(__value,__Print_Code) (__value |= __Print_Code)

typedef enum {
    InstLocation_dont_care = 0,
    InstLocation_start,
    InstLocation_end,
    InstLocation_Total_Types
} InstLocations;

typedef enum {
    DebugFormat_undefined = 0,
    DebugFormat_DWARF2_32bit,
    DebugFormat_DWARF2_64bit,
    DebugFormat_Total_Types
} DebugFormats;

typedef enum {
    ElfRelType_undefined = 0,
    ElfRelType_rel,
    ElfRelType_rela,
    ElfRelType_Total_Types
} ElfRelTypes;

typedef enum {
    ElfClassTypes_no_type = 0,
    ElfClassTypes_BasicBlock,
    ElfClassTypes_DwarfSection,
    ElfClassTypes_DwarfLineInfoSection,
    ElfClassTypes_Dynamic,
    ElfClassTypes_DynamicTable,
    ElfClassTypes_FileHeader,
    ElfClassTypes_FreeText,
    ElfClassTypes_Function,
    ElfClassTypes_GlobalOffsetTable,
    ElfClassTypes_GnuVerneed,
    ElfClassTypes_GnuVerneedTable,
    ElfClassTypes_GnuVersym,
    ElfClassTypes_GnuVersymTable,
    ElfClassTypes_HashTable,
    ElfClassTypes_Instruction,
    ElfClassTypes_InstrumentationFunction,
    ElfClassTypes_InstrumentationPoint,
    ElfClassTypes_InstrumentationSnippet,
    ElfClassTypes_Note,
    ElfClassTypes_NoteSection,
    ElfClassTypes_ProgramHeader,
    ElfClassTypes_RawSection,
    ElfClassTypes_RelocationTable,
    ElfClassTypes_Relocation,
    ElfClassTypes_SectionHeader,
    ElfClassTypes_StringTable,
    ElfClassTypes_Symbol,
    ElfClassTypes_SymbolTable,
    ElfClassTypes_TextSection,
    ElfClassTypes_TextUnknown,
    ElfClassTypes_Total_Types
} ElfClassTypes;

typedef enum {
    ByteSource_no_source = 0,
    ByteSource_Application,
    ByteSource_Application_FreeText,
    ByteSource_Application_Function,
    ByteSource_Instrumentation,
    ByteSource_Total_Types
} ByteSources;

class BinaryInputFile;
class BinaryOutputFile;

class Base {
protected:
    const static uint32_t invalidOffset = 0xffffffff;

    ElfClassTypes type;
    uint32_t sizeInBytes;
    uint32_t fileOffset;

    Base() : type(ElfClassTypes_no_type),sizeInBytes(0),fileOffset(invalidOffset) {}
    Base(ElfClassTypes t) : type(t),sizeInBytes(0),fileOffset(invalidOffset) {}
    virtual ~Base() {}

public:
    ElfClassTypes getType() { return type; }
    uint32_t getSizeInBytes() { return sizeInBytes; }

    virtual void print() { __SHOULD_NOT_ARRIVE; }
    virtual uint32_t read(BinaryInputFile* b) { __SHOULD_NOT_ARRIVE; return 0; }


    uint32_t getFileOffset() { return fileOffset; }
    uint32_t setFileOffset(uint32_t offset) { fileOffset = offset; return (fileOffset+sizeInBytes);}
    bool hasInvalidFileOffset() { return (invalidOffset == fileOffset); }

    virtual const char* briefName() { __SHOULD_NOT_ARRIVE; return NULL; }

    bool includesFileOffset(uint32_t offset);
};

class HashCode {
private:
    typedef union {
        struct {
            uint32_t memop   : 16;
            uint32_t block   : 16;
            uint32_t function: 16;
            uint32_t section : 8;
            uint32_t res     : 8;
        } fields;
        uint64_t bits;
    } HashCodeEntry;

    const static uint64_t INVALID_FIELD = 0;

    HashCodeEntry entry;

    inline bool hasSection()       { return (entry.fields.section   != INVALID_FIELD); }
    inline bool hasFunction()      { return (entry.fields.function  != INVALID_FIELD); }
    inline bool hasBlock()         { return (entry.fields.block     != INVALID_FIELD); }
    inline bool hasMemop()         { return (entry.fields.memop     != INVALID_FIELD); }

    inline static bool validSection(uint32_t s)    { return ((0 <= s) && (s < (0x1 << 8))); }
    inline static bool validFunction(uint32_t f){ return ((0 <= f) && (f < ((0x1 << 16) - 1))); }
    inline static bool validBlock(uint32_t b)    { return ((0 <= b) && (b < ((0x1 << 16) - 1))); }
    inline static bool validMemop(uint32_t m)    { return ((0 <= m) && (m < ((0x1 << 16) - 1))); }
public:

    inline uint64_t getValue(){ return entry.bits; }

    inline HashCode() { entry.bits = INVALID_FIELD; }
    inline HashCode(uint64_t a) { entry.bits = a; }

    HashCode(uint32_t s);
    HashCode(uint32_t s,uint32_t f);
    HashCode(uint32_t s,uint32_t f,uint32_t b);
    HashCode(uint32_t s,uint32_t f,uint32_t b,uint32_t m);

    inline bool isSection()  { return (hasSection() && !hasFunction() && !hasBlock() && !hasMemop()); }
    inline bool isFunction() { return (hasSection() && hasFunction() && !hasBlock() && !hasMemop()); }
    inline bool isBlock()    { return (hasSection() && hasFunction() && hasBlock() && !hasMemop()); }
    inline bool isMemop()    { return (hasSection() && hasFunction() && hasBlock() && hasMemop()); }
    inline bool isValid()    { return (isSection() || isFunction() || isBlock() || isMemop()); }

    inline uint32_t getSection()  { return entry.fields.section; }
    inline uint32_t getFunction() { return (hasFunction() ? (entry.fields.function - 1) : INVALID_FIELD); }
    inline uint32_t getBlock()      { return (hasBlock() ? (entry.fields.block - 1) : INVALID_FIELD); }
    inline uint32_t getMemop()    { return (hasMemop() ? (entry.fields.memop - 1) : INVALID_FIELD); }
};


extern bool isAddressAligned(uint64_t addr, uint32_t align);
extern bool isPowerOfTwo(uint32_t n);
extern uint64_t nextAlignAddress(uint64_t addr, uint32_t align);
extern uint64_t nextAlignAddressHalfWord(uint64_t addr);
extern uint64_t nextAlignAddressWord(uint64_t addr);
extern uint64_t nextAlignAddressDouble(uint64_t addr);

#define FIRST_HALFWORD(__n) ((__n) & 0xffff)
#define SECOND_HALFWORD(__n) (((__n) >> 16) & 0xffff)

extern double timer();
#endif
