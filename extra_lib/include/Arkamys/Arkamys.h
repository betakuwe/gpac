#ifndef ARKAMYS_H
#define ARKAMYS_H

/*****************************************************************/
/* Arkamys API definition*/
#ifndef ARKAMYS_HANDLE_TYPE
typedef void*  ARKAMYS_HANDLE;
typedef int ARKAMYS_RETURN;
#define ARKAMYS_HANDLE_TYPE
#define ARKAMYS_UNUSED (int)0
#endif

/*****************************************************************/
/*	ERROR DEFINITION*/
enum
{
	ARKAMYS_NO_ERROR=0,  
	ARKAMYS_NO_HANDLE_ERROR = -100,
	ARKAMYS_BLOCK_ID_ERROR, 
	ARKAMYS_INSTANCE_ID_ERROR,
	ARKAMYS_PARAMETER_ID_ERROR,
    ARKAMYS_PARAMETER_VALUE_ERROR,
	ARKAMYS_SIZE_ERROR,
	ARKAMYS_MEMORY_ALLOCATION_ERROR
};
#endif