/***************************  Fraunhofer IIS CDK Tools  ***********************

                        (C) Copyright Fraunhofer IIS (2008)
                               All Rights Reserved

   $Id: CDK_matrixCalloc.h
   Author(s):
   Description: matrix memory allocation

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef CDK_MATRIX_CALLOC_H
#define CDK_MATRIX_CALLOC_H

#include "CDK_tools_settings.h"


#include "machine_type.h"
#include "genericStds.h"


/* It is recommended to use CDK_ALLOCATE_MEMORY_1D instead of cdkCallocMatrix1D */
void* cdkCallocMatrix1D(UINT dim1, UINT size);
void* cdkCallocMatrix1D_aligned(UINT dim1, UINT size);
/* It is recommended to use CDK_ALLOCATE_MEMORY_1D_INT instead of cdkCallocMatrix1D_int */
void* cdkCallocMatrix1D_int(UINT dim1, UINT size, MEMORY_SECTION s);
void* cdkCallocMatrix1D_int_aligned(UINT dim1, UINT size, MEMORY_SECTION s);
/* It is recommended to use CDK_FREE_MEMORY_1D instead of cdkFreeMatrix1D */
void cdkFreeMatrix1D(void* p);
void cdkFreeMatrix1D_aligned(void* p);

/* It is recommended to use CDK_ALLOCATE_MEMORY_2D instead of cdkCallocMatrix2D */
void** cdkCallocMatrix2D(UINT dim1, UINT dim2, UINT size);
/*
   The function cdkCallocMatrix2D_dyn() allocates memory only for dim1.
   If the pointer d is set to an external memory (i.e. dynamic buffer) the pointers
   in dim1 will be set to the base address of equidistant areas of size dim2 of the
   external memory.
   If the pointer d is NULL the pointers in dim1 will be initialized with NULL.
 */
/* It is recommended to use CDK_ALLOCATE_MEMORY_2D_dyn instead of cdkCallocMatrix2D_dyn */
void** cdkCallocMatrix2D_dyn(void *d, UINT dim1, UINT dim2, UINT size);
void** cdkCallocMatrix2D_aligned(UINT dim1, UINT dim2, UINT size);
/* It is recommended to use CDK_ALLOCATE_MEMORY_2D_INT instead of cdkCallocMatrix2D_int */
void** cdkCallocMatrix2D_int(UINT dim1, UINT dim2, UINT size, MEMORY_SECTION s);
/* It is recommended to use CDK_ALLOCATE_MEMORY_2D_INT_ALIGNED instead of cdkCallocMatrix2D_int_aligned */
void** cdkCallocMatrix2D_int_aligned(UINT dim1, UINT dim2, UINT size, MEMORY_SECTION s);
/* It is recommended to use CDK_FREE_MEMORY_2D instead of cdkFreeMatrix2D */
void cdkFreeMatrix2D(void **p);
/* It is recommended to use CDK_FREE_MEMORY_2D_DYN instead of cdkFreeMatrix2D_dyn */
void cdkFreeMatrix2D_dyn(void **p);
/* It is recommended to use CDK_FREE_MEMORY_2D_ALIGNED instead of cdkFreeMatrix2D_aligned */
void cdkFreeMatrix2D_aligned(void **p);

/* It is recommended to use CDK_ALLOCATE_MEMORY_3D instead of cdkCallocMatrix3D */
void*** cdkCallocMatrix3D(UINT dim1, UINT dim2, UINT dim3, UINT size);
/*
   The function cdkCallocMatrix3D_dyn() allocates memory only for dim1 and dim2.
   If the pointer d is set to an external memory (i.e. dynamic buffer) the pointers
   in dim2 will be set to the base address of equidistant areas of size dim3 of the
   external memory.
   If the pointer d is NULL the pointers in dim2 will be initialized with NULL.
 */
/* It is recommended to use CDK_ALLOCATE_MEMORY_3D_dyn instead of cdkCallocMatrix3D_dyn */
void*** cdkCallocMatrix3D_dyn(void *d, UINT dim1, UINT dim2, UINT dim3, UINT size);
/* It is recommended to use CDK_ALLOCATE_MEMORY_3D_ALIGNED instead of cdkCallocMatrix3D_aligned */
void*** cdkCallocMatrix3D_aligned(UINT dim1, UINT dim2, UINT dim3, UINT size);
/* It is recommended to use CDK_ALLOCATE_MEMORY_3D_INT instead of cdkCallocMatrix3D_int */
void*** cdkCallocMatrix3D_int(UINT dim1, UINT dim2, UINT dim3, UINT size, MEMORY_SECTION s);
/* It is recommended to use CDK_FREE_MEMORY_3D instead of cdkFreeMatrix3D */
void cdkFreeMatrix3D(void ***p);
/* It is recommended to use CDK_FREE_MEMORY_3D_DYN instead of cdkFreeMatrix3D_dyn */
void cdkFreeMatrix3D_dyn(void ***p);
/* It is recommended to use CDK_FREE_MEMORY_3D_ALIGNED instead of cdkFreeMatrix3D_aligned */
void cdkFreeMatrix3D_aligned(void ***p);



#define CDK_ALLOCATE_MEMORY_1D(a,dim1,type,pMemRequirements)                                                   \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type*)cdkCallocMatrix1D((dim1),sizeof(type)))==NULL) {                        \
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizePersistent +=                                                   \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(type));                                             \
                     }

#define CDK_ALLOCATE_MEMORY_1D_ALIGNED(a,dim1,type,pMemRequirements)                                           \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type*)cdkCallocMatrix1D_aligned((dim1),sizeof(type)))==NULL) {                \
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizePersistent +=                                                   \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(type));                                             \
                     }

#define CDK_ALLOCATE_MEMORY_1D_P(a,dim1,type, ptype,pMemRequirements)                                                   \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(ptype)cdkCallocMatrix1D((dim1),sizeof(type)))==NULL) {                        \
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizePersistent +=                                                   \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(type));                                             \
                     }

#define CDK_ALLOCATE_MEMORY_1D_INT(a,dim1,type,s,pMemRequirements)                                             \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type*)cdkCallocMatrix1D_int((dim1),sizeof(type),(s)))==NULL) {                \
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizeFastPersistent +=                                               \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(type));                                             \
                     }

#define CDK_FREE_MEMORY_1D(a)                                                                                  \
                     do {                                                                                      \
                       cdkFreeMatrix1D( (void *) (a) );                                                        \
                       (a) = NULL;                                                                             \
                     } while (0)

#define CDK_FREE_MEMORY_1D_ALIGNED(a)                                                                          \
                     do {                                                                                      \
                       cdkFreeMatrix1D_aligned( (void *) (a) );                                                \
                       (a) = NULL;                                                                             \
                     } while (0)

#define CDK_ALLOCATE_MEMORY_2D(a,dim1,dim2,type,pMemRequirements)                                              \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type**)cdkCallocMatrix2D((dim1),(dim2),sizeof(type)))==NULL) {                \
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizePersistent +=                                                   \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(void*)) +                                           \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*sizeof(type));                                      \
                     }

#define CDK_ALLOCATE_MEMORY_2D_dyn(a,d,dim1,dim2,type,pMemRequirements)                                        \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type**)cdkCallocMatrix2D_dyn((d),(dim1),(dim2),sizeof(type)))==NULL) {        \
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizePersistent +=                                                   \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(void*));                                            \
                     }

#define CDK_ALLOCATE_MEMORY_2D_INT(a,dim1,dim2,type,s,pMemRequirements)                                        \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type**)cdkCallocMatrix2D_int((dim1),(dim2),sizeof(type),(s)))==NULL) {        \
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizeFastPersistent +=                                               \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(void*)) +                                           \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*sizeof(type));                                      \
                     }

#define CDK_ALLOCATE_MEMORY_2D_INT_ALIGNED(a,dim1,dim2,type,s,pMemRequirements)                                \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type**)cdkCallocMatrix2D_int_aligned((dim1),(dim2),sizeof(type),(s)))==NULL) {\
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizeFastPersistent +=                                               \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(void*)) +                                           \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*sizeof(type) + ALIGNMENT_DEFAULT + sizeof(void*));  \
                     }

#define CDK_FREE_MEMORY_2D(a)                                                                                  \
                     do {                                                                                      \
                       cdkFreeMatrix2D( (void **) (a) );                                                       \
                       (a) = NULL;                                                                             \
                     } while (0)

#define CDK_FREE_MEMORY_2D_DYN(a)                                                                              \
                     do {                                                                                      \
                       cdkFreeMatrix2D_dyn( (void **) (a) );                                                   \
                       (a) = NULL;                                                                             \
                     } while (0)

#define CDK_FREE_MEMORY_2D_ALIGNED(a)                                                                          \
                     do {                                                                                      \
                       cdkFreeMatrix2D_aligned( (void **) (a) );                                               \
                       (a) = NULL;                                                                             \
                     } while (0)

#define CDK_ALLOCATE_MEMORY_3D(a,dim1,dim2,dim3,type,pMemRequirements)                                         \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type***)cdkCallocMatrix3D((dim1),(dim2),(dim3),sizeof(type)))==NULL) {        \
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizePersistent +=                                                   \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(void**)) +                                          \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*sizeof(void*)) +                                    \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*(dim3)*sizeof(type));                               \
                     }

#define CDK_ALLOCATE_MEMORY_3D_dyn(a,d,dim1,dim2,dim3,type,pMemRequirements)                                   \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type***)cdkCallocMatrix3D_dyn((d),(dim1),(dim2),(dim3),sizeof(type)))==NULL) {\
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizePersistent +=                                                   \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(void**)) +                                          \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*sizeof(void*));                                     \
                     }

#define CDK_ALLOCATE_MEMORY_3D_ALIGNED(a,dim1,dim2,dim3,type,pMemRequirements)                                 \
                     if ((pMemRequirements)==NULL) {                                                           \
                       if( ((a)=(type***)cdkCallocMatrix3D_aligned((dim1),(dim2),(dim3),sizeof(type)))==NULL) {\
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizePersistent +=                                                   \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(void**)) +                                          \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*sizeof(void*)) +                                    \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*(dim3)*sizeof(type) + ALIGNMENT_DEFAULT + sizeof(void*)); \
                     }


#define CDK_ALLOCATE_MEMORY_3D_INT(a,dim1,dim2,dim3,type,s,pMemRequirements)                                   \
                     if ((pMemRequirements==NULL)) {                                                           \
                       if( ((a)=(type***)cdkCallocMatrix3D_int((dim1),(dim2),(dim3),sizeof(type),(s)))==NULL) {\
                         goto bail;                                                                            \
                       }                                                                                       \
                     } else {                                                                                  \
                       (pMemRequirements)->sizeFastPersistent +=                                               \
                            ALGN_SIZE_EXTRES((dim1)*sizeof(void**)) +                                          \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*sizeof(void*)) +                                    \
                            ALGN_SIZE_EXTRES((dim1)*(dim2)*(dim3)*sizeof(type));                               \
                     }

#define CDK_FREE_MEMORY_3D(a)                                                                                  \
                     do {                                                                                      \
                       cdkFreeMatrix3D( (void ***) (a) );                                                      \
                       (a) = NULL;                                                                             \
                     } while (0)

#define CDK_FREE_MEMORY_3D_DYN(a)                                                                              \
                     do {                                                                                      \
                       cdkFreeMatrix3D_dyn( (void ***) (a) );                                                  \
                       (a) = NULL;                                                                             \
                     } while (0)

#define CDK_FREE_MEMORY_3D_ALIGNED(a)                                                                          \
                     do {                                                                                      \
                       cdkFreeMatrix3D_aligned( (void ***) (a) );                                              \
                       (a) = NULL;                                                                             \
                     } while (0)




#endif /* CDK_MATRIX_CALLOC_H */
