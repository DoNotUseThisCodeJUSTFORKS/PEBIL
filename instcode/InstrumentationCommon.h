#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <dlfcn.h>

//#define COMPILE_32BIT
#ifdef COMPILE_32BIT
  #define DINT_TYPE int32_t
  #define DINT_PRNTSZ l
#else
  #define DINT_TYPE int64_t
  #define DINT_PRNTSZ ll
#endif // COMPILE_32BIT

#define PRINT_INSTR(...) fprintf(stdout, "-[p%d]- ", getpid()); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n"); \
    fflush(stdout);