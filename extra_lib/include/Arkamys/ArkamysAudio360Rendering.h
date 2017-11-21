#ifndef ArkamysAudio360Rendering_h
#define ArkamysAudio360Rendering_h

#ifdef __cplusplus
extern "C"{
#endif

#include "Arkamys.h" 

enum{
	ARKAMYS_STEREO_INPUT,
	ARKAMYS_51_INPUT,
	ARKAMYS_71_INPUT,
	ARKAMYS_AMBI_INPUT,
};

typedef struct ArkamysAudio360RenderingRotation_
{
    int yaw;
    int pitch;
    int roll;
}ArkamysAudio360RenderingRotation;

/* Set the rendering to Stereo, 5.1, 7.1 or ambisonics */
ARKAMYS_RETURN ArkamysAudio360RenderingGetInputModeId(ARKAMYS_HANDLE handle, int *inputMode);

/* Modify the sound stage according to the user head roatation */
ARKAMYS_RETURN ArkamysAudio360RenderingSetRotation(ARKAMYS_HANDLE handle, int pitch, int yaw, int roll);

/* Get the current sound stage rotation */
ARKAMYS_RETURN ArkamysAudio360RenderingGetRotation(ARKAMYS_HANDLE handle, int *pitch, int*yaw, int *roll);

/* Get the lib version */
ARKAMYS_RETURN ArkamysAudio360RenderingGetVersion(ARKAMYS_HANDLE handle, int *libVersion);


/* Returns the needed memory size to allocate */
int ArkamysAudio360RenderingGetMemSize(int inputMode);

/* Sets the effect configuration -->see the config structure for information */
ARKAMYS_RETURN ArkamysAudio360RenderingConfig(ARKAMYS_HANDLE handle, int inputMode);

/* Resets all the internal parameters */
ARKAMYS_RETURN ArkamysAudio360RenderingInit(ARKAMYS_HANDLE handle);

/* Processes the sample of pIn and copy the result into pOut
	pIn and pOut are audio interleaved.
	The pIn channel depends on the InputMode
	The pOut channel is always 2 ( stereo output for headphones)
 */
ARKAMYS_RETURN ArkamysAudio360RenderingProcess(ARKAMYS_HANDLE handle, float *pIn, float *pOut, int NumberOfSamplesPerChannel);

#ifdef __cplusplus
}
#endif

#endif
