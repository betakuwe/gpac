/**************************  Fraunhofer IIS CDK SysLib  **********************

                        (C) Copyright Fraunhofer IIS (2003)
                               All Rights Reserved

   $Id$
   Author(s):

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

/** \file   machine_type.h
 *  \brief  Type defines for various processors and compiler tools.
 */

#if !defined(__MACHINE_TYPE_H__)
#define __MACHINE_TYPE_H__

/**
\page MEMORY_CONSIDERATIONS Memory Considerations

\section MEMORY_ALLOCATION_MODELS Memory Allocation Models

\subsection Dynamic_Memory Dynamic Memory

Any amount of instances can be used for any CDK library, but requires a working memory
heap implementation (malloc). ::EXTERNAL_RESOURCES must be undefined.
To get the memory during runtime, call the function \c Get{name}(). Free'ing memory is done
by he functions \c Free{name}(). The free functions must be called always, even when using
the static memory setting and it would maybe not produce any error. This is for the sake of
consistency.

\subsection Static_Memory Static Memory: Usage of the Memory Manager

The memory manager (active if the macro ::EXTERNAL_RESOURCES is defined) receives pointers
to memory areas to be managed from a context outside of a CDK library or application. In
order to perform that initialization operation, a structure of type ::CDK_MEM_TAB has to
be filled with the sizes and pointer values of these memory areas and passed to the
function CDK_feedMemory(). But since the memory manager itself also requires a little bit
of memory, the function CDK_getMemRequire() initializes the ::CDK_MEM_TAB structure so
that the memory manager memory overhead is already included in the size fields. Thus, the
required steps for setting up the memory manager are as follows:

-# Allocate a ::CDK_MEM_TAB structure (on the stack for example).
-# Call CDK_getMemRequire(), passing the ::CDK_MEM_TAB structure.
-# Add (not assign) the required memory areas to the corresponding fields of the
   ::CDK_MEM_TAB structure, and assign the corresponding pointers to the memory areas of
   the given sizes.
-# Call CDK_feedMemory() passing the ::CDK_MEM_TAB structure in order to setup the current
   memory manager instance.
-# Get a reference to the current memory manager by calling CDK_getInstance().
-# Switch among memory manager instances calling CDK_setInstance().

Libraries that use the memory manager are not reentrant-capable.

\section MEMORY_MANAGEMENT Memory Management

The CDK architecture features a simple and flexible concept to manage memory. The
definition of memory areas allows CDK libraries to be compiled using static or dynamic
(heap) memory alternatively, and to share certain memory areas with other libraries.

\subsection CMA Creating Memory Areas
To create a memory area, the compiler requires its declaration (header file) and
definition (object file). There is only one memory declaration macro but several
definition macros. That is because requesting and releasing memory areas is always done
the same way, while the memory areas can be of different nature.

Each memory area has an identifier (ID) and a corresponding request and release function.
The request and release function names are generated using the ID, thus it is very
important that the memory area names are unique.

\subsection Declaration Declaration
To declare a memory area use the macro H_ALLOC_MEM(name, type). Declaring memory areas
requires to specify a unique name and a data type.

\subsection Definition Definition
To define a memory area you additionally require one or two sizes, depending if the
memory should be organized into one or two dimensions. The macros containing the keyword
AALLOC instead of ALLOC also take care of returning aligned memory addresses (beyond the
natural alignment of its type). The preprocesor macro ::ALIGNMENT_DEFAULT indicates the
aligment to be used (this is hardware specific). The _L suffix indicates that the memory
will be located in a specific section. This is useful to allocate critical memory into
fast internal SRAM for example. machine_type.h defines an enum type for the section type.

To get the memory during runtime, call the function \c Get{name}(). Free'ing memory is done
by the functions \c Free{name}(). For the sake of consistency the free functions must always
be called, even when using the static memory setting. The complete list of available
definition macros is located at genericStds.h.

Simple one-dimensional array:
\code
  C_ALLOC_MEM(ID, data type, size)
\endcode

Simple two-dimensional array:
\code
  C_ALLOC_MEM2(ID, data type, size2, size1)
\endcode

Memory area inside a specific linker section:
\code
  C_ALLOC_MEM_L(ID, data type, size, section)
\endcode

Example: Definition of a memory area of type int and size 10*sizeof(int), then the associated
function calls are:

\code
   C_ALLOC_MEM(FooMemory, int, 10)
   GetFooMemory();
   FreeFooMemory();
\endcode

\section MO Memory Overlays

Memory overlays allow to share memory regions among different modules. The most common
reason to do this is when certain memory does not require to be persistent over a certain
period of time (temporary work buffers). To define a memory overlay, it is required to
provide a size and a unique identifier (ID). Every time a memory overlay is requested using
the same ID, the memory areas will be shared. The size does not need to be exactly the same,
however the first request must specify the largest size of all requested sizes. Ideally the
requested size should always be the same for a given overlay tag. An overlay lives from the
first request until its last release (reference counting is handled internally).

Memory overlays can also share only portions of its entire size dynamically. The shared area
is called "area", and there are functions for either defining the shared area or retrieving
its address. Requesting an overlay area does not have any effect on its reference counting.
Using this feature can be useful, if some regions of an overlay should stay untouched by other
modules for some time, but can be shared afterwards. When any module requests its area while
no explicit memory area setup was done, the default area address will be at offset 0 of the
memory area, and the size will be the complete overlay.

Examples:

Declaring a memory overlay (header file)

\code
H_ALLOC_MEM_OVERLAY(AACdynamic_RAM, FIXP_DBL)
\endcode

Implementing a memory overlay (source file)

\code
C_ALLOC_MEM_OVERLAY(AACdynamic_RAM, FIXP_DBL, (MAX_CHAN*MAX_BINS_LONG), WORKBUF_TAG)
\endcode

The function being called in the example code below are declared and implemented
automatically by the macros above. Requesting memory overlay:

\code
hAacEncoder->dynamic_RAM.COMMONdynamic_RAM = GetAreaAACdynamic_RAM();
\endcode

Defining the shared area of a memory overlay

\code
SetAreaRam_SbrDynamic_RAM(hEnvEncoder->SBRdynamic_RAM+P_BUF_SCR, SBR_DYN_RAM_SIZE);
\endcode

Getting a memory overlay area address

\code
hAacEncoder->dynamic_RAM.COMMONdynamic_RAM = GetAreaAACdynamic_RAM();
\endcode

Release a memory overlay

\code
FreeAACdynamic_RAM(&hAacEncoder->dynamic_RAM.REFdynamic_RAM);
\endcode

*/

#include <stddef.h>     /* Needed to define size_t */

#if defined(__ANDROID__) && (__GNUC__ == 4) && (__GNUC_MINOR__ == 4) && (__GNUC_GNU_INLINE__ == 1)
typedef unsigned long long uint64_t;
#include <sys/types.h>
#endif

/* Library calling convention spec. __cdecl and friends might be added here as required. */
  #define LINKSPEC_H
  #define LINKSPEC_CPP


/* for doxygen the following docu parts must be separated */
/** \var  SCHAR
 *        Data type representing at least 1 byte signed integer on all supported platforms.
 */
/** \var  UCHAR
 *        Data type representing at least 1 byte unsigned integer on all supported platforms.
 */
/** \var  INT
 *        Data type representing at least 4 byte signed integer on all supported platforms.
 */
/** \var  UINT
 *        Data type representing at least 4 byte unsigned integer on all supported platforms.
 */
/** \var  LONG
 *        Data type representing 4 byte signed integer on all supported platforms.
 */
/** \var  ULONG
 *        Data type representing 4 byte unsigned integer on all supported platforms.
 */
/** \var  SHORT
 *        Data type representing 2 byte signed integer on all supported platforms.
 */
/** \var  USHORT
 *        Data type representing 2 byte unsigned integer on all supported platforms.
 */
/** \var  INT64
 *        Data type representing 8 byte signed integer on all supported platforms.
 */
/** \var  UINT64
 *        Data type representing 8 byte unsigned integer on all supported platforms.
 */
/** \def  SHORT_BITS
 *        Number of bits the data type short represents. sizeof() is not suited to get this info,
 *        because a byte is not always defined as 8 bits.
 */
/** \def  CHAR_BITS
 *        Number of bits the data type char represents. sizeof() is not suited to get this info,
 *        because a byte is not always defined as 8 bits.
 */
/** \var  INT_PCM
 *        Data type representing the width of input and output PCM samples.
 */

#if   defined(__ADSP21000__) /* sharc platform */

  /* char         32 bits */
  /* short        32 bits */
  /* int          32 bits */
  /* long         32 bits */
  /* long long    32 bits */
  /* float        32 bits */
  /* double       32 bits */
  /* long double  32 bits */

  typedef signed int INT;
  typedef unsigned int UINT;
  typedef signed long LONG;
  typedef unsigned long ULONG;
  typedef signed short SHORT;
  typedef unsigned short USHORT;
  typedef signed char SCHAR;
  typedef unsigned char UCHAR;

#if defined(__BA_SHARC__)
  #define SHORT_BITS 16
  #define CHAR_BITS 8
#else
  #define SHORT_BITS 32
  #define CHAR_BITS 32
#endif

#else /* Any other toolchain */

  typedef signed int INT;
  typedef unsigned int UINT;
#ifdef __LP64__
  /* force CDK long-datatypes to 4 byte  */
  /* jdr: Use defines to avoid type alias problems on 64 bit machines. */
  #define LONG INT
  #define ULONG UINT
#else /* __LP64__ */
  typedef signed long LONG;
  typedef unsigned long ULONG;
#endif	/* __LP64__ */
  typedef signed short SHORT;
  typedef unsigned short USHORT;
  typedef signed char SCHAR;
  typedef unsigned char UCHAR;

  #define SHORT_BITS 16
  #define CHAR_BITS 8

#endif

/* Define 64 bit base integer type. */
#ifdef _MSC_VER
  typedef __int64 INT64;
  typedef unsigned __int64 UINT64;
#elif defined(__ADSP21000__)
  #ifdef __CCESVERSION__ /* CCES and also VDSP 5 update 9 upwards supports 64 bit. */
  typedef signed long long INT64;
  typedef unsigned long long UINT64;
  #else
  typedef INT INT64;
  typedef UINT UINT64;
  #pragma warning "INT64 datatype not supported by sharc processors. Remove this line if you know what you do and accept UINT64 being mapped to UINT."
  #endif
#else
  typedef long long INT64;
  typedef unsigned long long UINT64;
#endif

#ifndef NULL
  #ifdef __cplusplus
    #define NULL    0
  #else
    #define NULL    ((void *)0)
  #endif
#endif

/**
 * \def  CDK_ASSERT_ENABLE
 *       Enable ASSERTs. Asserts are always enabled on PC platforms and in debug mode as well.
 *       Note: changing this macro obviously does not have any effect for precompiled libraries.
 */
#if  (defined(__i686__) || defined(__i586__) || defined(__i386__) || defined(__x86_64__)) || (defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))) && !defined(CDK_ASSERT_ENABLE)
  #define CDK_ASSERT_ENABLE
#endif
 
#if defined(CDK_ASSERT_ENABLE)
#ifdef __ADSP21000__
  #include <stdio.h>
  #define CDK_ASSERT(x) do { if (!(x)) fprintf(stderr, "Assert " #x " failed!\n"); } while (0)
#else
  #include <assert.h>
  #define CDK_ASSERT(x) assert(x)
#endif
#else
  #define CDK_ASSERT(ignore)
#endif

/*!
* \def  CONF_TEST
*       Fraunhofer internal testing macro.
*
* \def  PCM16
*       Defines the maximum supported PCM sample quantization bit depth to 16 bits.
*       Libraries compiled with PCM16 do not support higher PCM quantization bitrates than 16 bit.
*       The base variable type is 16 bit signed integer.
*
* \def  PCM24
*       Defines the maximum supported PCM sample quantization bit depth to 24 bits.
*       Libraries compiled with PCM24 do not support higher PCM quantization bitrates than 24 bit.
*       The base variable type is 32 bit signed integer.
*
* \def  PCM32
*       Defines the maximum supported PCM sample quantization bit depth to 32 bits.
*       Libraries compiled with PCM32 do not support higher PCM quantization bitrates than 32 bit.
*       The base variable type is 32 bit signed integer.
*
* \def  WAV_BITS
*       WAV_BITS defines the PCM quantization bitrate of WAVE files.
*       Used as function argument when opening a WAVE file.
*
* \def  SAMPLE_BITS
*       Number of bits used to store one sample. This information is required because the size of
*       this storage does not always correspond to WAV_BITS since there is no data type of size 24 bits,
*       where we then use 32 bits.
*/

#ifdef CONF_TEST
  #define PCM24
#else
  #define PCM16
  /* #define PCM24 */
  /* #define PCM32 */
#endif

#if defined(PCM24)
  typedef LONG         INT_PCM;
  #define WAV_BITS     24
  #define SAMPLE_BITS  32
  #define SAMPLE_MAX ((INT_PCM)(((ULONG)1<<(SAMPLE_BITS-1))-1))
  #define SAMPLE_MIN (~SAMPLE_MAX)
#elif defined(PCM32)
  typedef LONG         INT_PCM;
  #define WAV_BITS     32
  #define SAMPLE_BITS  32
  #define SAMPLE_MAX ((INT_PCM)(((ULONG)1<<(SAMPLE_BITS-1))-1))
  #define SAMPLE_MIN (~SAMPLE_MAX)
#elif defined(PCM16)
  typedef SHORT        INT_PCM;
  #define WAV_BITS     16
  #define SAMPLE_BITS  16
  #define SAMPLE_MAX ((INT_PCM)(((ULONG)1<<(SAMPLE_BITS-1))-1))
  #define SAMPLE_MIN (~SAMPLE_MAX)
#else
  #error "Error: INT_PCM is not defined."
#endif


/*!
* \def    RAM_ALIGN
*  Used to align memory as prefix before memory declaration. For example:
   \code
   RAM_ALIGN
   int myArray[16];
   \endcode

   Note, that not all platforms support this mechanism. For example with TI compilers
   a preprocessor pragma is used, but to do something like

   \code
   #define RAM_ALIGN #pragma DATA_ALIGN(x)
   \endcode

   would require the preprocessor to process this line twice to fully resolve it. Hence,
   a fully platform-independant way to use alignment is not supported.

* \def    ALIGNMENT_DEFAULT
*         Default alignment in bytes.
*/

#if (defined(_M_ARM) || defined(__CC_ARM) || defined(__TMS470__) || defined(__arm__) || defined(__aarch64__)) && \
    (defined(__TARGET_FEATURE_NEON) || defined(__ARM_NEON) || defined(__ARM_NEON__) || defined(__ARM_AARCH64_NEON__))
  #define ALIGNMENT_DEFAULT 16
#else
  #define ALIGNMENT_DEFAULT 8
#endif

/* RAM_ALIGN keyword causes memory alignment of global variables. */
#if defined(__CC_ARM)
  #define RAM_ALIGN __align(ALIGNMENT_DEFAULT)
#elif defined(__ANALOG_EXTENSIONS__)
  #define RAM_ALIGN
  #pragma pack(ALIGNMENT_DEFAULT)
#elif defined (_MSC_VER)
  #define RAM_ALIGN   __declspec(align(ALIGNMENT_DEFAULT))
#elif defined(__GNUC__)
  #define RAM_ALIGN __attribute__((aligned(ALIGNMENT_DEFAULT)))
#else
  #define RAM_ALIGN
#endif

/*!
* \def  RESTRICT
*       The restrict keyword is supported by some platforms and RESTRICT maps to
*       either the corresponding keyword on each platform or to void if the
*       compiler does not provide such feature. It tells the compiler that a pointer
*       points to memory that does not overlap with other memories pointed to by other
*       pointers. If this keyword is used and the assumption of no overlap is not true
*       the resulting code might crash.
*
* \def  WORD_ALIGNED(x)
*       Tells the compiler that pointer x is 16 bit aligned. It does not cause the address
*       itself to be aligned, but serves as a hint to the optimizer. The alignment of the
*       pointer must be guarranteed, if not the code might crash.
*
* \def  DWORD_ALIGNED(x)
*       Tells the compiler that pointer x is 32 bit aligned. It does not cause the address
*       itself to be aligned, but serves as a hint to the optimizer. The alignment of the
*       pointer must be guarranteed, if not the code might crash.
*
* \def  CDK_PRAGMA_MUST_ITERATE(var,min,max,multiple)
*       Tells the compiler that the loop next to this line runs
*       minimum 'min' times, maximum 'max' times, the number of loop
*       iterations is always a multiple of 'multiple'.
*       'min' defaults to 0, if not set
*       'max' defaults to 0x7FFF.FFF, if not set
*       'multiple' defaults to 1, if not set
*       The parameter 'var' is used for debug purpose and checked to be in range [min,max], if DEBUG=1
*         CDK_PRAGMA_MUST_ITERATE(nBands, 8, 64, 4)
*         for (int i = 0; i < nBands; i++) { ... }
*       Note: Use this macro without an ending semicolon
*
* \def  CDK_PRAGMA_CODE_SECTION(section)
*       Tells the compiler that the function code, next to this line should
*       be linked into the section, given in quotes. Example
*       CDK_PRAGMA_CODE_SECTION(".text:text_qmf")
*       Note: Use this macro without an ending semicolon
*
* \def  CDK_PRAGMA_DATA_SECTION(section)
*       Tells the compiler that the data block, next to this line should
*       be linked into the section, given in quotes. Example
*       CDK_PRAGMA_DATA_SECTION(".L1ram")
*       Note: Use this macro without an ending semicolon
*/
#if   defined(__ANALOG_EXTENSIONS__)
  #define RESTRICT restrict
  #define WORD_ALIGNED(x)  C_ALLOC_ALIGNED_CHECK2((const void*)(x),2)
  #define DWORD_ALIGNED(x) C_ALLOC_ALIGNED_CHECK2((const void*)(x),4)
  #define CDK_PRAGMA_MUST_ITERATE(var,min,max,multiple)
  #define CDK_PRAGMA_CODE_SECTION(section)
  #define CDK_PRAGMA_DATA_SECTION(section)
#elif defined (__XTENSA__)
  #define RESTRICT __restrict
  #define CDK_PRAGMA(x) _Pragma(#x)

  #define WORD_ALIGNED(x)  { __attribute((unused)) const void *ptr=(x); C_ALLOC_ALIGNED_CHECK2(ptr,2); CDK_PRAGMA(aligned(ptr,2)); }
  #define DWORD_ALIGNED(x) { __attribute((unused)) const void *ptr=(x); C_ALLOC_ALIGNED_CHECK2(ptr,4); CDK_PRAGMA(aligned(ptr,4)); }
  #define CDK_PRAGMA_MUST_ITERATE(var,min,max,multiple)
  #define CDK_PRAGMA_CODE_SECTION(section)
  #define CDK_PRAGMA_DATA_SECTION(section)
#else
  #define RESTRICT
  /* Non-debug macros */
  #define WORD_ALIGNED(x)  C_ALLOC_ALIGNED_CHECK2((const void*)(x),2);
  #define DWORD_ALIGNED(x) C_ALLOC_ALIGNED_CHECK2((const void*)(x),4);
  #define CDK_PRAGMA_MUST_ITERATE(var,min,max,multiple)
  #define CDK_PRAGMA_CODE_SECTION(section)
  #define CDK_PRAGMA_DATA_SECTION(section)
#endif


/*-----------------------------------------------------------------------------------
 * ALIGN_SIZE
 *-----------------------------------------------------------------------------------*/
/*!
 * \brief  This macro aligns a given value depending on ::ALIGNMENT_DEFAULT.
 *
 * For example if #ALIGNMENT_DEFAULT equals 8, then:
 * - ALIGN_SIZE(3) returns 8
 * - ALIGN_SIZE(8) returns 8
 * - ALIGN_SIZE(9) returns 16
 */
#define ALIGN_SIZE(a) ((a)+ (((INT)ALIGNMENT_DEFAULT - ((size_t)(a) & (ALIGNMENT_DEFAULT-1)) ) & (ALIGNMENT_DEFAULT-1)))

/*-----------------------------------------------------------------------------------
 * ALIGN_PTR
 * cast (a) to width of pointer
 *-----------------------------------------------------------------------------------*/
/*!
 * \brief  This macro aligns a given address depending on ::ALIGNMENT_DEFAULT.
 */
#define ALIGN_PTR(a) ( (a) + ((((INT)ALIGNMENT_DEFAULT - ((size_t)(a) & (ALIGNMENT_DEFAULT-1)) ) & (ALIGNMENT_DEFAULT-1))/sizeof(*(a))) )

 /* Alignment macro for libSYS heap implementation */
#define ALIGNMENT_EXTRES    ( ALIGNMENT_DEFAULT )
#define ALGN_SIZE_EXTRES(a) ((a)+ (((INT)ALIGNMENT_EXTRES - ((INT)(a) & (ALIGNMENT_EXTRES-1)) ) & (ALIGNMENT_EXTRES-1)))


/* Hardware weaknesses/limitations */

/*!
 * \def  ARCH_WA_NOCMDLINE
 *       If a platform does not support the possibility to provide command line arguments
 *       to call main() with, the definition of ARCH_WA_NOCMDLINE initiates the use of a
 *       workaround mechanism.
 *       In every project's example framework, main.cpp has two implementations of main(),
 *       and depending on whether ARCH_WA_NOCMDLINE is set or not, either one of those is used.
 *       If ARCH_WA_NOCMDLINE is set, main(), by use of IIS_ProcessCmdlList(), parses a text
 *       file and interprets each line it finds as an argument list to feed process_file().
 *       So this is a convenient way to test a long set of test files at one go.
 *
 * \def  ARCH_WA_FLUSH_CONSOLE
 *       Additionally causes stderr and stdout being flushed by fflush() whenever there is
 *       something printed. See CDKprintf() and CDKprintfErr().
 */

#if defined(__ANALOG_EXTENSIONS__) || defined (__TI_TMS470_V5__)
  /* Hardware where the console is extremely slow (stdio through jtag polling). */
  #define ARCH_WA_SLOWCON
#endif

#if defined(__ANALOG_EXTENSIONS__) || defined __TI_TMS470_V5__
  /* Hardware where commandline parameter passing is not possible with a reasonable effort or in an anyhow portable way. */
  #define ARCH_WA_NOCMDLINE
#endif


#if defined(_WIN32) || defined(__linux__)
  /* Platform were processes/tasks are scheduled pre-emptively (unrelated to the Linux pre-emptive kernel feature). */
  #define ARCH_WA_PREEMPTIVE_PROCESS_SCHEDULING
#endif

/*!
 * \def  CDK_FORCEINLINE
 *       Sometimes compiler do not do what they are told to do, and in case of inlining some
 *       additional command might be necessary depending on the platform.
 *
 * \def  CDK_INLINE
 *       Defines how the compiler is told to inline stuff.
 */
#ifndef CDK_FORCEINLINE
  #if defined(__CC_ARM) && (__ARMCC_VERSION > 200000)
    #define CDK_FORCEINLINE __forceinline
  #elif defined(__GNUC__) && !defined(__SDE_MIPS__)
    #define CDK_FORCEINLINE inline __attribute((always_inline))
  #else
    #define CDK_FORCEINLINE inline
  #endif
#endif

#if   defined(__XTENSA__)
  #define CDK_INLINE __attribute__((always_inline)) static
#else  
  /* for all other compilers without that bug. */
  #define CDK_INLINE static inline
#endif


/*!
 * \def  LNK_SECTION_DATA_L1
 *       The LNK_SECTION_* defines allow memory to be drawn from specific memory
 *       sections. Used as prefix before variable declaration.
 *
 * \def  LNK_SECTION_DATA_L2
 *       See ::LNK_SECTION_DATA_L1
 * \def  LNK_SECTION_L1_DATA_A
 *       See ::LNK_SECTION_DATA_L1
 * \def  LNK_SECTION_L1_DATA_B
 *       See ::LNK_SECTION_DATA_L1
 * \def  LNK_SECTION_CONSTDATA_L1
 *       See ::LNK_SECTION_DATA_L1
 * \def  LNK_SECTION_CONSTDATA
 *       See ::LNK_SECTION_DATA_L1
 * \def  LNK_SECTION_CODE_L1
 *       See ::LNK_SECTION_DATA_L1
 * \def  LNK_SECTION_CODE_L2
 *       See ::LNK_SECTION_DATA_L1
 * \def  LNK_SECTION_INITCODE
 *       See ::LNK_SECTION_DATA_L1
 */
/**************************************************
 * Code Section macros
 **************************************************/
  #define LNK_SECTION_CODE_L1
  #define LNK_SECTION_CODE_L2
  #define LNK_SECTION_INITCODE

/* Memory section macros. */

  /* default fall back */
  #define LNK_SECTION_DATA_L1
  #define LNK_SECTION_DATA_L2
  #define LNK_SECTION_CONSTDATA
  #define LNK_SECTION_CONSTDATA_L1

  #define LNK_SECTION_L1_DATA_A
  #define LNK_SECTION_L1_DATA_B


#ifdef _MSC_VER
  /*
   * Sometimes certain features are excluded from compilation and therefore the warning 4065 may occur:
   * "switch statement contains 'default' but no 'case' labels"
   * We consider this warning irrelevant and disable it.
   */
  #pragma warning( disable : 4065 )
#endif

#endif /* __MACHINE_TYPE_H__ */
