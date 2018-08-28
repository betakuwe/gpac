#include "CDK_formatConverter_intern.h"
#include "CDK_formatConverterLib.h"
#include "CDK_bitstream.h"

#define DMX_MATRIX_MAX_SPEAKER_COUNT 32
#define DMX_MATRIX_CODER_STATE_COUNT_MAX 512
#define DMX_MATRIX_GAIN_TABLE_SIZE_MAX ((22 - (-46)) * (1 << 2) + 2)
#define DMX_MATRIX_GAIN_ZERO -256
#define CDK_DMX_MATRIX_GAIN_ZERO ((FIXP_DBL) DMX_MATRIX_GAIN_ZERO << 23) /* Q23 */
#define ARRAYLENGTH(a) (sizeof(a) / sizeof(a[0]))

#define CDK_MPEGHAUDIO_DEC_MAX_LOUDSPEAKERS    (48)  /* Mpeg-H actually allows up to 65536 loudspeakers in this context. */
#define CDK_MPEGHAUDIO_DEC_MAX_SIGNAL_GROUPS   (CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNEL_GROUPS)
#define CDK_MPEGHAUDIO_DEC_MAX_OUTPUT_CHANNELS (CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS)

#define CDK_DOWNMIX_CONFIG_MAX_DMX_MATRICES_PER_ID                     (8)
#define CDK_DOWNMIX_CONFIG_MAX_GROUPS_ASSIGNED_TO_DOWNMIX_MATRIX      (16)
#define CDK_DOWNMIX_CONFIG_MAX_DMX_MATRIX_SIZE                       (256)
#define CDK_DOWNMIX_CONFIG_MAX_DMX_MATRIX_ELEMENTS                    (32)

typedef struct 
{
  UCHAR downmixId;
  UCHAR downmixType;
  UCHAR CICPspeakerLayoutIdx;
  UCHAR bsDownmixMatrixCount;
  USHORT bsNumAssignedGroupIDs[CDK_DOWNMIX_CONFIG_MAX_DMX_MATRICES_PER_ID];
  UCHAR signal_groupID[CDK_DOWNMIX_CONFIG_MAX_DMX_MATRICES_PER_ID][CDK_DOWNMIX_CONFIG_MAX_GROUPS_ASSIGNED_TO_DOWNMIX_MATRIX];
  UINT  DmxMatrixLenBits[CDK_DOWNMIX_CONFIG_MAX_DMX_MATRICES_PER_ID];
  UCHAR DownmixMatrix[CDK_DOWNMIX_CONFIG_MAX_DMX_MATRICES_PER_ID][CDK_DOWNMIX_CONFIG_MAX_DMX_MATRIX_SIZE];
} CDK_DOWNMIX_CONFIG_MATRIX_SET;

typedef struct
{
  UCHAR downmixMatrix[CDK_MPEGHAUDIO_DEC_MAX_SIGNAL_GROUPS][256];
  INT   downmixMatrixSize[CDK_MPEGHAUDIO_DEC_MAX_SIGNAL_GROUPS];
} CDK_DOWNMIX_GROUPS_MATRIX_SET;

typedef enum {
  SP_PAIR_CENTER, /* one center speaker */
  SP_PAIR_SYMMETRIC, /* a symmetric L/R speaker pair */
  SP_PAIR_SINGLE, /* an asymmetric single speaker */
  SP_PAIR_NONE /* the right speaker of a symmetric L/R speaker pair */
} SP_PAIR_TYPE;

typedef struct SpeakerInformationStruct {
  SHORT elevation;  /* speaker elevation in degrees, positive angles upwards */
  SHORT azimuth;    /* speaker azimuth in degrees, positive angles to the left */
  SHORT isLFE;      /* whether the speaker type is LFE */

  SHORT originalPosition;   /* original speaker position in the channel list */
  SHORT isAlreadyUsed;      /* channel is already used in the compact channel list */
  struct SpeakerInformationStruct* symmetricPair;   /* the right speaker of a symmetric L/R speaker pair */
  SP_PAIR_TYPE pairType;    /* the type of pair for compact speaker configurations */
} SpeakerInformation;

typedef struct {
  INT minGain;
  INT maxGain;
  INT precisionLevel;
  INT rawCodingNonzeros;
  INT gainLGRParam;
  FIXP_DBL history[DMX_MATRIX_CODER_STATE_COUNT_MAX];
  INT historyCount;
  FIXP_DBL gainTable[DMX_MATRIX_GAIN_TABLE_SIZE_MAX];
  INT gainTableSize;
} CoderState;

typedef struct
{
  // unsigned int                                 isExternalSetup;   /* 1 = user gave local setup, 0 = no local setup given */

  UINT                           nTargetSignals;
  CICP2GEOMETRY_CHANNEL_GEOMETRY targetGeometries[CDK_MPEGHAUDIO_DEC_MAX_LOUDSPEAKERS];

  /*
  unsigned int                                 enableGainCompensation;
  unsigned int                                 enableDistanceCompensation;
  */

} CDK_MPEGHAUDIO_DEC_LOCAL_SETUP_INFORMATION;

typedef struct _CDK_MPEGHAUDIO_DEC_SIGNAL_GROUP
{
  CICP2GEOMETRY_CHANNEL_GEOMETRY        *channelLayout;
  UINT                                  numSignals;       /* Number of core coder signals in signal group */
} CDK_MPEGHAUDIO_DEC_SIGNAL_GROUP;

typedef struct _IIS_MPEGHAUDIO_DEC_TRANSFER_DATA
{
  CDK_MPEGHAUDIO_DEC_SIGNAL_GROUP               signalGroups[10];
  CDK_MPEGHAUDIO_DEC_LOCAL_SETUP_INFORMATION    targetSpeakerSetup;                                 /* Target speaker setup information after rendering */
} CDK_MPEGHAUDIO_DEC_TRANSFER_DATA, *CDK_MPEGHAUDIO_DEC_TRANSFER_DATA_HANDLE;

/*
typedef struct 
{
  unsigned int                   downmixId;
  unsigned int                   downmixType;
  unsigned int                   CICPspeakerLayoutIdx;
  unsigned int                   downmixMatrixCount;
  unsigned int                   numAssignedGroupIDs[CDK_DOWNMIX_CONFIG_MAX_DMX_MATRICES_PER_ID];

  unsigned int                   signal_groupID[CDK_DOWNMIX_CONFIG_MAX_DMX_MATRICES_PER_ID][CDK_DOWNMIX_CONFIG_MAX_GROUPS_ASSIGNED_TO_DOWNMIX_MATRIX];

  unsigned int                   DmxMatrixLenBits[CDK_DOWNMIX_CONFIG_MAX_DMX_MATRICES_PER_ID];
  char                           DownmixMatrix[CDK_DOWNMIX_CONFIG_MAX_DMX_MATRICES_PER_ID][CDK_DOWNMIX_CONFIG_MAX_DMX_MATRIX_SIZE];

} CDK_DOWNMIX_CONFIG_MATRIX_SET;
*/

typedef struct 
{
  unsigned int                    downmixConfigType;
  unsigned int                    passiveDownmixFlag;
  unsigned int                    phaseAlignStrength;
  unsigned int                    immersiveDownmixFlag;
  unsigned int                    downmixIdCount;
  CDK_DOWNMIX_CONFIG_MATRIX_SET   *downmixMatrixSet;

} cdk_DownmixConfig, *cdk_DownmixConfigPtr;

typedef struct cdk_mpeghaudio_dec_dmxcontrolunit* CDK_MPEGHAUDIO_DEC_DMXCONTROLUNIT_HANDLE;

typedef struct cdk_mpeghaudio_dec_drc_control_unit* CDK_MPEGHAUDIO_DEC_DRCCONTROLUNIT_HANDLE;

typedef struct cdk_mpeghaudio_dec_userinteraction* CDK_MPEGHAUDIO_DEC_UIMODULE_HANDLE;

typedef struct CDK_UIMANAGER_userinteraction* CDK_UIMANAGER_HANDLE;

typedef struct _CDK_MPEGHAUDIO_MPEGHAUDIO_CONFIG_IO* CDK_MPEGHAUDIO_DEC_CONFIG_IO_HANDLE;

/*
  ReadRange() 
*/

UINT ReadRange(HANDLE_CDK_BITSTREAM hBs, UINT alphabetSize);

/*
  EqualizerConfig() subroutine for equalizers
*/

INT EqualizerConfig(HANDLE_CDK_BITSTREAM hBs, SpeakerInformation *inputConfig, UINT inputCount, eqConfigStruct* eqConfig);

/*
  DownmixMatrix() subroutine for downmix matrix.
*/

void ConvertToCompactConfig( SpeakerInformation* inputConfig,
                             INT inputCount,
                             INT* compactInputCount,
                             SpeakerInformation* compactInputConfig[]);

void DecodeFlatCompactMatrix(HANDLE_CDK_BITSTREAM hBs, signed char* flatCompactMatrix, int totalCount);

FIXP_DBL DecodeGainValue(HANDLE_CDK_BITSTREAM hBs, CoderState* cs);

void CoderStateGenerateGainTable(CoderState* cs);

signed char* FindCompactTemplate(int inputIndex, int outputIndex);

void CoderStateInit(CoderState* cs);

INT DecodeDownmixMatrix( INT inputIndex, INT inputCount, SpeakerInformation *inputConfig, INT outputIndex, INT outputCount, SpeakerInformation *outputConfig, HANDLE_CDK_BITSTREAM hBs, FIXP_SGL* downmixMatrix, eqConfigStruct* eqConfig);


INT _setDownmixIdFromRuleSet(CICP2GEOMETRY_CHANNEL_GEOMETRY* outGeo, unsigned int numChannels, unsigned int numLFEs, cdk_DownmixConfig downmixConfig, int* downmixId);

INT CDK_DmxControlUnit_GetDownmixId( CDK_MPEGHAUDIO_DEC_DMXCONTROLUNIT_HANDLE   hInstance,
                                     INT                                        cicpIndex,
                                     CICP2GEOMETRY_CHANNEL_GEOMETRY*            targetGeo,
                                     UINT                                       numChannels,
                                     INT*                                       downmixId );

INT CDK_GetDownmixID( CDK_DOWNMIX_CONFIG_MATRIX_SET                *DownmixMtxSet,
                      INT                                          targetLayout,
                      INT                                          downmixIdCount,
                      INT*                                         downmixId );

INT DownmixMatrixSet( HANDLE_CDK_BITSTREAM hBs, CDK_DOWNMIX_GROUPS_MATRIX_SET *groupsDownmixMatrixSet, const INT targetLayout, const INT downmixConfigType, INT *downmixId, HANDLE_DRC_DECODER hUniDrcDec);

INT DownmixMatrixSelect( CDK_DOWNMIX_CONFIG_MATRIX_SET *downmixMatrixSet, CDK_DOWNMIX_GROUPS_MATRIX_SET *groupsDownmixMatrixSet, const INT targetLayout, INT downmixConfigType, INT downmixIdCount, INT downmixId );

INT downmixConfig(HANDLE_CDK_BITSTREAM hBs);

