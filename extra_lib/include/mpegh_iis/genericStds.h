/**************************  Fraunhofer IIS CDK SysLib  **********************

                        (C) Copyright Fraunhofer IIS (2002)
                               All Rights Reserved

   $Id$
   Author(s):

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

/** \file   genericStds.h
    \brief  Generic Run-Time Support function wrappers and heap allocation monitoring.
 */

#if !defined(__GENERICSTDS_H__)
#define __GENERICSTDS_H__

#include "machine_type.h"
#include "syslib_settings.h"

#ifndef M_PI
  #define M_PI   3.14159265358979323846  /*!< Pi. Only used in example projects. */
#endif


/**
 * Identifiers for various memory locations. They are used along with memory allocation
 * functions like CDKcalloc_L() to specify the requested memory's location.
 */
typedef enum {
  /* Internal */
  SECT_DATA_L1 = 0x2000,
  SECT_DATA_L2,
  SECT_DATA_L1_A,
  SECT_DATA_L1_B,
  SECT_CONSTDATA_L1,

  /* External */
  SECT_DATA_EXTERN = 0x4000,
  SECT_CONSTDATA_EXTERN

} MEMORY_SECTION;


/*! \addtogroup SYSLIB_MEMORY_MACROS CDK memory macros
 *
 * The \c H_ prefix indicates that the macro is to be used in a header file, the \c C_ prefix indicates that the macro is to be used in a source file.
 *
 * Declaring memory areas requires to specify a unique name and a data type.
 *
 * For defining a memory area you require additionally one or two sizes, depending if the
 * memory should be organized into one or two dimensions.
 *
 * The macros containing the keyword \c AALLOC instead of \c ALLOC additionally  take care of returning
 * aligned memory addresses (beyond the natural alignment of its type). The preprocesor macro
 * ::ALIGNMENT_DEFAULT indicates the aligment to be used (this is hardware specific).
 *
 * The \c _L suffix indicates that the memory will be located in a specific section. This is
 * useful to allocate critical memory section into fast internal SRAM for example.
 *
 * @{
 */

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define H_ALLOC_MEM(name,type)          type * Get ## name(int n=0); void Free ## name(type** p); \
                                        UINT GetRequiredMem ## name(void);

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#ifdef CDK_OVERLAYS_ENABLE
#define H_ALLOC_MEM_OVERLAY(name,type)  type * Get ## name(void); void Free ## name(type** p); \
                                        type * GetArea ## name(void); void SetArea ## name(void *addr, INT asize); \
                                        UINT GetRequiredMem ## name(void);
#else
#define H_ALLOC_MEM_OVERLAY(name,type)  type * Get ## name(int n=0); void Free ## name(type** p); \
                                        UINT GetRequiredMem ## name(void);
#endif

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_ALLOC_MEM(name,type,num) \
  type * Get ## name(int n) { CDK_ASSERT((n) == 0); return ((type*)CDKcalloc(num, sizeof(type))); } \
  void Free ## name(type** p) { if (p != NULL) { CDKfree(*p); *p=NULL; } } \
  UINT GetRequiredMem ## name(void) { return ALGN_SIZE_EXTRES((num) * sizeof(type)); }


/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_ALLOC_MEM2(name,type,n1,n2) \
  type * Get ## name (int n) { CDK_ASSERT((n) < (n2)); return ((type*)CDKcalloc(n1, sizeof(type))); } \
  void Free ## name(type** p) { if (p != NULL) { CDKfree(*p); *p=NULL; } } \
  UINT GetRequiredMem ## name(void) { return ALGN_SIZE_EXTRES((n1) * sizeof(type)) * (n2); }

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_AALLOC_MEM(name,type,num) \
  type * Get ## name(int n) { type *ap; CDK_ASSERT((n) == 0); ap=((type*)CDKaalloc((num)*sizeof(type), ALIGNMENT_DEFAULT)); return ap; } \
  void Free ## name(type** p) { if (p != NULL) { CDKafree(*p); *p=NULL; } } \
  UINT GetRequiredMem ## name(void) { return ALGN_SIZE_EXTRES((num) * sizeof(type) + ALIGNMENT_DEFAULT + sizeof(void *)); }

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_AALLOC_MEM2(name,type,n1,n2) \
  type * Get ## name (int n) { type *ap; CDK_ASSERT((n) < (n2)); ap=((type*)CDKaalloc((n1)*sizeof(type), ALIGNMENT_DEFAULT)); return ap; } \
  void Free ## name(type** p) { if (p != NULL) { CDKafree(*p); *p=NULL; } } \
  UINT GetRequiredMem ## name(void) { return ALGN_SIZE_EXTRES((n1) * sizeof(type) + ALIGNMENT_DEFAULT + sizeof(void *)) * (n2); }

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_ALLOC_MEM_L(name,type,num,s) \
  type * Get ## name(int n) { CDK_ASSERT((n) == 0); return ((type*)CDKcalloc_L(num, sizeof(type), s)); } \
  void Free ## name(type** p) { if (p != NULL) { CDKfree_L(*p); *p=NULL; } } \
  UINT GetRequiredMem ## name(void) { return ALGN_SIZE_EXTRES((num) * sizeof(type)); }

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_ALLOC_MEM2_L(name,type,n1,n2,s) \
  type * Get ## name (int n) { CDK_ASSERT((n) < (n2)); return (type*)CDKcalloc_L(n1, sizeof(type), s); } \
  void Free ## name(type** p) { if (p != NULL) { CDKfree_L(*p); *p=NULL; } } \
  UINT GetRequiredMem ## name(void) { return ALGN_SIZE_EXTRES((n1) * sizeof(type)) * (n2); }

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_AALLOC_MEM_L(name,type,num,s) \
  type * Get ## name(int n) { type *ap; CDK_ASSERT((n) == 0); ap=((type*)CDKaalloc_L((num)*sizeof(type), ALIGNMENT_DEFAULT, s)); return ap; } \
  void Free ## name(type** p) { if (p != NULL) { CDKafree_L(*p); *p=NULL; } } \
  UINT GetRequiredMem ## name(void) { return ALGN_SIZE_EXTRES((num) * sizeof(type) + ALIGNMENT_DEFAULT + sizeof(void *)); }

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_AALLOC_MEM2_L(name,type,n1,n2,s) \
  type * Get ## name (int n) { type *ap; CDK_ASSERT((n) < (n2)); ap=((type*)CDKaalloc_L((n1)*sizeof(type), ALIGNMENT_DEFAULT, s)); return ap; } \
  void Free ## name(type** p) { if (p != NULL) { CDKafree_L(*p); *p=NULL; } } \
  UINT GetRequiredMem ## name(void) { return ALGN_SIZE_EXTRES((n1) * sizeof(type) + ALIGNMENT_DEFAULT + sizeof(void *)) * (n2); }

#ifdef CDK_OVERLAYS_ENABLE
  /** See \ref SYSLIB_MEMORY_MACROS for description. */
  #define C_ALLOC_MEM_OVERLAY(name,type,num,sect,tag) \
    type * Get ## name(void) { return ((type*)CDKgetOverlay((num)*sizeof(type), tag, sect)); } \
    type * GetArea ## name(void) { return ((type*)CDKgetOverlayArea((num)*sizeof(type), tag)); } \
    void SetArea ## name(void *addr, INT asize) { CDKsetOverlayArea(addr, asize, tag); } \
    void Free ## name(type** p) { if (p != NULL) { CDKfreeOverlay(*p); *p=NULL; } } \
    UINT GetRequiredMem ## name(void) { return ALGN_SIZE_EXTRES((num) * sizeof(type) + ALIGNMENT_DEFAULT + sizeof(void *)) ; } \
    int GetTag ## name(void) { return tag; }
#else
  /** See \ref SYSLIB_MEMORY_MACROS for description. */
  #define C_ALLOC_MEM_OVERLAY(name,type,num,sect,tag) C_AALLOC_MEM_L(name,type,num,sect)
#endif /* CDK_OVERLAYS_ENABLE */

#ifdef CDK_SCRATCHBUFFER_STATIC_ENABLE

   /** See \ref SYSLIB_MEMORY_MACROS for description. */
   #define C_AALLOC_SCRATCH_START(name,type,n) \
     type * name = (type*)CDKgetScratchBuf((n) * sizeof(type)); \
     C_ALLOC_ALIGNED_REGISTER(name, (n)* sizeof(type));
   /** See \ref SYSLIB_MEMORY_MACROS for description. */
   #define C_ALLOC_SCRATCH_START(name,type,n) \
     type * name = (type*)CDKgetScratchBuf((n) * sizeof(type));

   /** See \ref SYSLIB_MEMORY_MACROS for description. */
   #define C_AALLOC_SCRATCH_END(name,type,n) \
     CDKfreeScratchBuf((n) * sizeof(type)); \
     C_ALLOC_ALIGNED_CHECK(name); \
     C_ALLOC_ALIGNED_UNREGISTER(name);
   /** See \ref SYSLIB_MEMORY_MACROS for description. */
   #define C_ALLOC_SCRATCH_END(name,type,n) \
     CDKfreeScratchBuf((n) * sizeof(type));

#else /* CDK_SCRATCHBUFFER_STATIC_ENABLE */

   /** See \ref SYSLIB_MEMORY_MACROS for description. */
   #define C_AALLOC_SCRATCH_START(name,type,n) \
     type _ ## name[(n)+(ALIGNMENT_DEFAULT+sizeof(type)-1)]; \
     type * name = (type*)ALIGN_PTR(_ ## name); \
     C_ALLOC_ALIGNED_REGISTER(name, (n) * sizeof(type));

   /** See \ref SYSLIB_MEMORY_MACROS for description. */
   #define C_ALLOC_SCRATCH_START(name,type,n) \
     type name[n];

   /** See \ref SYSLIB_MEMORY_MACROS for description. */
   #define C_AALLOC_SCRATCH_END(name,type,n) \
     C_ALLOC_ALIGNED_UNREGISTER(name);
  /** See \ref SYSLIB_MEMORY_MACROS for description. */
   #define C_ALLOC_SCRATCH_END(name,type,n)

#endif /* CDK_SCRATCHBUFFER_STATIC_ENABLE */

/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_AALLOC_STACK_START(name,type,n) \
  type _ ## name[(n)+(ALIGNMENT_DEFAULT+sizeof(type)-1)]; \
  type * name = (type*)ALIGN_PTR(_ ## name); \
  C_ALLOC_ALIGNED_REGISTER(name, (n) * sizeof(type));
  
/** See \ref SYSLIB_MEMORY_MACROS for description. */
#define C_AALLOC_STACK_END(name,type,n) \
  C_ALLOC_ALIGNED_UNREGISTER(name);

/*! @} */

#define C_ALLOC_ALIGNED_REGISTER(x,size)
#define C_ALLOC_ALIGNED_UNREGISTER(x)
#define C_ALLOC_ALIGNED_CHECK(x)
#define C_ALLOC_ALIGNED_CHECK2(x,y)
#define CDK_showBacktrace(a,b)


/*! \addtogroup SYSLIB_EXITCODES Unified exit codes
 *  Exit codes to be used as return values of CDK software test and demonstration applications.
 *  Not as return values of product modules and/or libraries.
 *  @{
 */
#define CDK_EXITCODE_OK            0  /*!< Successful termination. No errors. */
#define CDK_EXITCODE_USAGE        64  /*!< The command/application was used incorrectly, e.g. with the wrong number of
                                           arguments, a bad flag, a bad syntax in a parameter, or whatever. */
#define CDK_EXITCODE_DATAERROR    65  /*!< The input data was incorrect in some way. This should only be used for
                                           user data and not system files. */
#define CDK_EXITCODE_NOINPUT      66  /*!< An input file (not a system file) did not exist or was not readable. */
#define CDK_EXITCODE_UNAVAILABLE  69  /*!< A service is unavailable. This can occur if a support program or file does
                                           not exist. This can also be used as a catchall message when something you
                                           wanted to do doesn't work, but you don't know why. */
#define CDK_EXITCODE_SOFTWARE     70  /*!< An internal software error has been detected. This should be limited to non-
                                           operating system related errors as possible. */
#define CDK_EXITCODE_CANTCREATE   73  /*!< A (user specified) output file cannot be created. */
#define CDK_EXITCODE_IOERROR      74  /*!< An error occurred while doing I/O on some file. */
/*! @} */


/*--------------------------------------------
 * Runtime support declarations
 *---------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/** printf() using stdout. If ::ARCH_WA_FLUSH_CONSOLE defined, a flush is  done additionally after printf(). */
void CDKprintf    ( const char* szFmt, ...);

/** printf() using stderr. If ::ARCH_WA_FLUSH_CONSOLE defined, a flush is done additionally after printf(). */
void CDKprintfErr ( const char* szFmt, ...);

/** Wrapper for <stdio.h>'s getchar(). */
int CDKgetchar(void);

INT  CDKfprintf(void  *stream,  const  char *format, ...);
INT  CDKsprintf(char *str, const char *format, ...);


char *CDKstrchr(char *s, INT c);
const char *CDKstrstr(const char *haystack, const char *needle);
char *CDKstrcpy(char *dest, const char *src);
char *CDKstrncpy(char *dest, const char *src, const UINT n);

#define CDK_MAX_OVERLAYS 8   /**< Maximum number of memory overlays. */

typedef struct
{
  void *pPersistent;      /*< Static memory (mostly external).      */
  void *pFastPersistent;  /*< Fast static memory (mostly internal). */
  void *pScratch;         /*< Scratch (dynamic) memory.             */
  void *pFastScratch;     /*< Fast scratch memory (internal).       */

  UINT sizePersistent;
  UINT sizeFastPersistent;
  UINT sizeScratch;
  UINT sizeScratchOL[CDK_MAX_OVERLAYS]; /* sizes of each overlay to please all overlay requests */

  UINT sizeFastScratch;

} CDK_MEM_TAB;

/**
 * Sum up memory usage of various (sub-)libraries.
 * As fastScratch and Overlays can be shared between various
 * instances, only the max. allocated area is counted in.
 * All other memory types are summed up.
 *
 * \param *dst       Destination memory tab.
 * \param *src       Source memory table.
 * \return           Nothing
 */
void addMemReq(CDK_MEM_TAB *dst, CDK_MEM_TAB *src);

#ifdef EXTERNAL_RESOURCES

int  CDK_feedMemory    (const CDK_MEM_TAB *pMemTab);
int  CDK_setInstance   (void *pMemAdmin);
void *CDK_getInstance  (void);

#endif /* EXTERNAL_RESOURCES */

void *CDKcalloc (const UINT n, const UINT size);
void *CDKmalloc (const UINT size);
void  CDKfree   (void *ptr);

/**
 *  Allocate and clear an aligned memory area. Use CDKafree() instead of CDKfree() for these memory areas.
 *
 * \param size       Size of requested memory in bytes.
 * \param alignment  Alignment of requested memory in bytes.
 * \return           Pointer to allocated memory.
 */
void *CDKaalloc (const UINT size, const UINT alignment);

/**
 *  Free an aligned memory area.
 *
 * \param ptr  Pointer to be freed.
 */
void CDKafree (void *ptr);

/**
 *  Get memory overlay of a given tag. If the overlay never has been requested, the memory is allocated.
 *
 * \param size
 * \param tag
 * \param s
 * \return
 */
void *CDKgetOverlay(const UINT size, int tag, MEMORY_SECTION s);

/**
 *  Free memory overlay of address. If the overlay internal reference counting
 *  is zero, the memory is free()'ed.
 */
void CDKfreeOverlay(void *ptr);

/**
 *  Set shared area address of overlay.
 */
void CDKsetOverlayArea(void *addr, const UINT size, int tag);

/**
 *  Get shared area of overlay.
 */
void *CDKgetOverlayArea(const UINT size, int tag);

/**
 *  Allocate memory in a specific memory section.
 *  Requests can be made for internal or external memory. If internal memory is
 *  requested, CDKcalloc_L() first tries to use L1 memory, which sizes are defined
 *  by ::DATA_L1_A_SIZE and ::DATA_L1_B_SIZE. If no L1 memory is available, then
 *  CDKcalloc_L() tries to use L2 memory. If that fails as well, the requested
 *  memory is allocated at an extern location using the fallback CDKcalloc().
 *
 * \param n     See MSDN documentation on calloc().
 * \param size  See MSDN documentation on calloc().
 * \param s     Memory section.
 * \return      See MSDN documentation on calloc().
 */
void *CDKcalloc_L(const UINT n, const UINT size, MEMORY_SECTION s);

/**
 *  Allocate aligned memory in a specific memory section.
 *  See CDKcalloc_L() description for details - same applies here.
 */
void *CDKaalloc_L(const UINT size, const UINT alignment, MEMORY_SECTION s);

/**
 *  Free memory that was allocated in a specific memory section.
 */
void  CDKfree_L(void *ptr);

/**
 *  Free aligned memory that was allocated in a specific memory section.
 */
void  CDKafree_L(void *ptr);

#ifdef CDK_SCRATCHBUFFER_STATIC_ENABLE
/**
 *  Request scratch memory.
 *
 * \param size  Requested number of bytes.
 * \return      Pointer to memory.
 */
void *CDKgetScratchBuf(int size);

/**
 *  Free scratch memory.
 *
 * \param size  Number of bytes to be freed.
 */
void CDKfreeScratchBuf(int size);
#endif

/**
 * Copy memory. Source and destination memory must not overlap.
 * Either use implementation from a Standard Library, or, if no Standard Library
 * is available, a generic implementation.
 * The define ::USE_BUILTIN_MEM_FUNCTIONS in genericStds.cpp controls what to use.
 * The function arguments correspond to the standard memcpy(). Please see MSDN
 * documentation for details on how to use it.
 */
void CDKmemcpy(void *dst, const void *src, const UINT size);

/**
 * Copy memory. Source and destination memory are allowed to overlap.
 * Either use implementation from a Standard Library, or, if no Standard Library
 * is available, a generic implementation.
 * The define ::USE_BUILTIN_MEM_FUNCTIONS in genericStds.cpp controls what to use.
 * The function arguments correspond to the standard memmove(). Please see MSDN
 * documentation for details on how to use it.
*/
void CDKmemmove(void *dst, const void *src, const UINT size);

/**
 * Clear memory.
 * Either use implementation from a Standard Library, or, if no Standard Library
 * is available, a generic implementation.
 * The define ::USE_BUILTIN_MEM_FUNCTIONS in genericStds.cpp controls what to use.
 * The function arguments correspond to the standard memclear(). Please see MSDN
 * documentation for details on how to use it.
*/
void CDKmemclear(void *memPtr, const UINT size);

/**
 * Fill memory with values.
 * The function arguments correspond to the standard memset(). Please see MSDN
 * documentation for details on how to use it.
 */
void CDKmemset(void *memPtr, const INT value, const UINT size);

/* Compare function wrappers */
INT   CDKmemcmp(const void *s1, const void *s2, const UINT size);
INT   CDKstrcmp(const char *s1, const char *s2);
INT   CDKstrncmp(const char *s1, const char *s2, const UINT size);

UINT  CDKstrlen(const char *s);

#define CDKmax(a,b) ( (a) > (b) ? (a):(b))
#define CDKmin(a,b) ( (a) < (b) ? (a):(b))

#define CDK_INT_MAX ((INT)0x7FFFFFFF)
#define CDK_INT_MIN ((INT)0x80000000)

/* Math function wrappers. Only intended for compatibility, not to be highly optimized. */
/* Used for debugging, dev code .. */
#if defined(CDK_STD_MATH_WRAPPER_ENABLE)
INT CDKabs(INT j);
double CDKfabs(double x);
double CDKpow(double x, double y);
double CDKsqrt(double x);
double CDKatan(double x);
double CDKlog(double x);
double CDKsin(double x);
double CDKcos(double x);
double CDKexp(double x);
#define CDKlog2(a) (CDKlog(a)*1.442695041) /* log(2.0) = 1.442695041 */
#define CDKlog10(a) (CDKlog(a)*0.434294482) /* 1.0/log(10.0) = 0.434294482 */
double CDKatan2(double y, double x);
double CDKacos(double x);
double CDKtan(double x);
double CDKfloor(double x);
double CDKceil(double x);
INT   CDKatoi(const char *nptr);
long  CDKatol(const char *nptr);
float CDKatof(const char *nptr);
/* LONG LONG CDKatoll(const char *nptr); */
/* LONG LONG CDKatoq(const char *nptr); */
#endif


/* FILE I/O */

/*!
 *  Check platform for endianess.
 *
 * \return  1 if platform is little endian, non-1 if platform is big endian.
 */
int IS_LITTLE_ENDIAN(void);

/*!
 *  Convert input value to little endian format.
 *
 * \param val  Value to be converted. It may be in both big or little endian.
 * \return     Value in little endian format.
 */
UINT TO_LITTLE_ENDIAN(UINT val);

#ifdef FILE_IO_SUPPORT
/*!
 * \fn     CDKFILE *CDKfopen(const char *filename, const char *mode);
 *         Standard fopen() wrapper.
 * \fn     INT CDKfclose(CDKFILE *FP);
 *         Standard fclose() wrapper.
 * \fn     INT CDKfseek(CDKFILE *FP, LONG OFFSET, int WHENCE);
 *         Standard fseek() wrapper.
 * \fn     INT CDKftell(CDKFILE *FP);
 *         Standard ftell() wrapper.
 * \fn     INT CDKfflush(CDKFILE *fp);
 *         Standard fflush() wrapper.
 * \fn     UINT CDKfwrite(const void *ptrf, INT size, UINT nmemb, CDKFILE *fp);
 *         Standard fwrite() wrapper.
 * \fn     UINT CDKfread(void *dst, INT size, UINT nmemb, CDKFILE *fp);
 *         Standard fread() wrapper.
 */
typedef void CDKFILE;
extern const INT CDKSEEK_SET, CDKSEEK_CUR, CDKSEEK_END;

CDKFILE *CDKfopen(const char *filename, const char *mode);
INT CDKfclose(CDKFILE *FP);
INT CDKfseek(CDKFILE *FP, LONG OFFSET, int WHENCE);
INT CDKftell(CDKFILE *FP);
INT CDKfflush(CDKFILE *fp);
UINT CDKfwrite(const void *ptrf, INT size, UINT nmemb, CDKFILE *fp);
UINT CDKfread(void *dst, INT size, UINT nmemb, CDKFILE *fp);
char* CDKfgets(void *dst, INT size, CDKFILE *fp);
void CDKrewind(CDKFILE *fp);
INT CDKfeof(CDKFILE *fp);

/**
 * \brief        Write each member in little endian order. Convert automatically to host endianess.
 * \param ptrf   Pointer to memory where to read data from.
 * \param size   Size of each item to be written.
 * \param nmemb  Number of items to be written.
 * \param fp     File pointer of type CDKFILE.
 * \return       Number of items read on success and fread() error on failure.
 */
UINT CDKfwrite_EL(const void *ptrf, INT size, UINT nmemb, CDKFILE *fp);

/**
 * \brief        Read variable of size "size" as little endian. Convert automatically to host endianess.
 *                4-byte alignment is enforced for 24 bit data, at 32 bit full scale.
 * \param dst    Pointer to memory where to store data into.
 * \param size   Size of each item to be read.
 * \param nmemb  Number of items to be read.
 * \param fp     File pointer of type CDKFILE.
 * \return       Number of items read on success and fread() error on failure.
 */
UINT CDKfread_EL(void *dst, INT size, UINT nmemb, CDKFILE *fp);
#endif /* FILE_IO_SUPPORT */

/**
 * \brief  Check CDK initialization and initialize if required.
 *   - Initialize global structures (memory overlay handler).
 *   - SRAM allocation (Linux platform with kernel driver)
 * \return 0 if already initialized, 1 if initialization was required and 2 in case of consistency failure.
 */
int CDKinit_check(void);

/**
 * \brief Report debug informations and do several sanity/integrity checks. This function is meant to be called
 *        as a last step in a CDK test/demo program.
 */
int CDKdebugReport(void);

/* Backward compatibility wrapper */
#define CDKprolog CDKinit_check
#define CDKepilog CDKdebugReport

/**
 * \brief  Print CDK software disclaimer.
 */
void CDKprintDisclaimer(void);

#ifdef __cplusplus
}
#endif

#endif /* __GENERICSTDS_H__ */
