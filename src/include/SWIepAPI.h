/* ****************************License************************************
** Copyright 1994-2007.  Nuance Communications, Inc.  All rights reserved.
**  
** Use of this software is subject to certain restrictions and limitations set
** forth in a license agreement entered into between Nuance Communications,
** Inc. and the licensee of this software.  Please refer to the license
** agreement for license use rights and restrictions.
**
** Portions of the Nuance Recognizer Software are subject to copyrights of AT&T
** Corp., E-Speech Corporation, Bell Communications Research, Inc., European
** Telecommunications Standards Institute and GlobeTrotter Software, Inc.
**    
** SpeechWorks and OpenSpeech are registered trademarks, and SpeechWorks Here,
** OpenSpeech DialogModules, DialogModules and the SpeechWorks logo are
** trademarks of Nuance Communications, Inc.  in the United States and other
** countries.
*/

#ifndef _SWIEP_API_H_
#define _SWIEP_API_H_
#include "SWIrecAudio.h"

/*****************************************************************************
 *****************************************************************************
 *
 * API definition for Nuance Endpointer
 *
 *****************************************************************************
 ****************************************************************************/

#if defined(_MSC_VER)            /* Microsoft Visual C++ */
  #if !defined(ALTAPI)
    #define ALTAPI __stdcall
  #endif
  #pragma pack(push, 8)
#elif defined(__BORLANDC__)      /* Borland C++ */
  #if !defined(ALTAPI)
    #define ALTAPI __stdcall
  #endif
  #pragma option -a8
#elif defined(__WATCOMC__)       /* Watcom C++ */
  #if !defined(ALTAPI)
    #define ALTAPI __stdcall
  #endif
  #pragma pack(push, 8)
#else                            /* Any other including Unix */
  #if !defined(ALTAPI)
    #define ALTAPI
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

 /*
  *  Opaque data types
  */

typedef struct SWIepDetector { void *internal; } SWIepDetector;

/*
 *  The enumeration SWIepFuncResult enumerates possible return values
 *  SWIep_SUCCESS successful return
 *  SWIep_UNKNOWN_PARAMETER tried to use a parameter that does not exist
 *  SWIep_INIT_STATE_FAILED initialization of the endpointer state failed
 *  SWIep_OUT_OF_MEMORY could not allocate memory
 *  SWIep_COLLECTION_ERROR an error occurred while processing speech samples
 *  SWIep_BUFFER_OVERFLOW acoustic state buffer too small (during save)
 */
typedef enum {
  SWIep_RESULT_SUCCESS = 0,
  SWIep_ERROR_INIT_STATE_FAILED = -1,
  SWIep_ERROR_OUT_OF_MEMORY = -2,
  SWIep_ERROR_COLLECTION_ERROR = -3,
  SWIep_ERROR_BUFFER_OVERFLOW = -4,
  SWIep_ERROR_BUSY = -5,
  SWIep_ERROR_INVALID_PARAMETER = -6,
  SWIep_ERROR_INACTIVE = -7,
  SWIep_ERROR_INVALID_DATA = -8,
  SWIep_ERROR_NO_LICENSE = -9,
  SWIep_ERROR_LICENSE_ALLOCATED = -10,
  SWIep_ERROR_LICENSE_FREED = -11,
  SWIep_ERROR_UNSUPPORTED= -12,
  SWIep_ERROR_SYSTEM_ERROR = -13,
  SWIep_ERROR_NO_DATA = -14,
  SWIep_ERROR_INVALID_PARAMETER_VALUE = -15,
  SWIep_ERROR_INVALID_SYSTEM_CONFIGURATION = -16,
  SWIep_ERROR_INVALID_MEDIA_TYPE = -17,
  SWIep_ERROR_LICENSE_COMPROMISE = -18,
  SWIep_ERROR_NO_SESSION_NAME = -19,
  SWIep_ERROR_GENERIC_FAILURE = 0x0FFFF,
  SWIep_ERROR_GENERIC_ERROR = 0x1FFFF,
  SWIep_ERROR_GENERIC_FATAL_ERROR = 0x2FFFF
} SWIepFuncResult;

/*
 * @enum   SWIepStopCode
 * @brief  SWIepStop requires a stop code indicating why the speech detection 
 *         action was terminated. The reason code is used for better
 *         adaptation within a call boundary and to log information
 *         for tuning.
 */
typedef enum {
  SWIep_STOP_SPEECH  = 0,       /* Caller used speech */
  SWIep_STOP_DTMF    = 1,       /* Caller used DTMF */
  SWIep_STOP_HANGUP  = 2,       /* Caller hung up */
  SWIep_STOP_TIMEOUT = 4,       /* No speech detected; timeout */
  SWIep_STOP_OTHER   = 8        /* Other reason not covered above */
} SWIepStopCode;

typedef struct SWIepAudioSamples
{
  /**
   * Audio sample data
   *
   * @param samples Sample buffer
   * @param len Length of sample buffer, in bytes
   * @param type MEDIA type of samples
   *   "audio/basic" 8-bit 8 KHz u-law encoding [unsigned char *]
   *   "audio/x-alaw-basic" 8-bit 8 KHz A-law encoding [unsigned char *]
   *   "audio/L16;rate=8000" 16-bit 8 KHz linear encoding [unsigned short *]
   *   "audio/x-suppressed" suppressed samples (len = # suppressed) [void]
   *   "audio/x-lost" lost samples (len = # lost) [void]
   * @param status Sample status: lost, suppressed
   */
  void *samples;
  unsigned int len;
  const wchar_t *type;
  int status;
} SWIepAudioSamples;

typedef enum {
  SWIep_BEGIN_END = 0,
  SWIep_BEGIN_ONLY = 1,  /* never returns end */
  SWIep_END_ONLY = 2,    /* currently same as BEGIN_END */
  SWIep_MAGIC_WORD = 4,
  SWIep_SELECTIVE_BARGEIN = 5,
  SWIep_DISABLED = -1
} SWIepMode;


/* start of new definitions */

  /*
   * The following are generic errors that should always be checked for:
   * SWIep_ERROR_SYSTEM_ERROR if a system error has occurred 
   * SWIep_ERROR_INVALID_PARAMETER_VALUE if any required parameters are NULL or zero-length strings, etc
   * SWIep_ERROR_OUT_OF_MEMORY if a memory allocation failed
   * SWIep_ERROR_GENERIC_FAILURE unspecified failure
   * SWIep_ERROR_GENERIC_ERROR unspecified error
   * SWIep_ERROR_GENERIC_FATAL_ERROR unspecified fatal error
   * SWIep_ERROR_LICENSE_COMPROMISE license server has been unreachable for
     greater than the allowed "downtime" window (61-89 seconds). Correct the problem by
     by destroying all endpointer instances, shutting down the process using SWIepTerminate(), 
     resolving the broken connection to the license server, and restarting the process.
  */



  /** Global initialization; should be invoked once at process start-up
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_GENERIC_FATAL_ERROR if error in startup
   * @return SWIep_ERROR_NO_LICENSE if no licenses are available
   * @return SWIep_ERROR_LICENSE_ALLOCATED id licenses have been allocated, but not the full amount requested
   */
  SWIepFuncResult ALTAPI SWIepInit(void);

  /** Global shutdown; should be invoked once at process termination
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_BUSY if hasn't been initialized
   */
  SWIepFuncResult ALTAPI SWIepTerminate(void);

  /**
   * Allocates endpointer state and structures and initializes the state
   * @param handle Double pointer to detector handle (returned)
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_INIT_STATE_FAILED if license object couldn't be created
   *    (most likely memory allocation or config-related problem)
   * @return SWIep_ERROR_NO_LICENSE if all licenses are in use
   * @return SWIep_ERROR_GENERIC_ERROR unspecified error
   */
  SWIepFuncResult ALTAPI SWIepDetectorCreate(SWIepDetector **handle);


  /**
   * Destroys the endpointer state and buffer structures
   * @param handle Pointer to detector handle
   *
   * @return SWIep_RESULT_SUCCESS on success
   */
  SWIepFuncResult ALTAPI SWIepDetectorDestroy(SWIepDetector *handle);

  /** Reset acoustic state; must be called at start of each new call
   * @param handle Pointer to detector handle
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_BUSY if endpointer has already been started
   */
  SWIepFuncResult ALTAPI SWIepAcousticStateReset(SWIepDetector *handle);

  /** Query size of acoustic state
   * @param handle Pointer to detector handle
   * @param size Returned size
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_BUSY if endpointer has already been started
   */
  SWIepFuncResult ALTAPI SWIepAcousticStateQuerySize(SWIepDetector *handle,
                                                     unsigned int *size);

  /**
   * Read acoustic state from a memory buffer
   * @param handle Pointer to detector handle
   * @param buffer Pointer to buffer (of size len) that contains data
   * @param len Length of buffer
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_BUSY if endpointer has already been started
   * @return SWIep_ERROR_BUFFER_OVERFLOW if buffer is too small to fit the data
   * @return SWIep_ERROR_INVALID_DATA if data is invalid
   */
  SWIepFuncResult ALTAPI SWIepAcousticStateLoad(SWIepDetector *handle, 
                                                void *buffer, 
                                                int len);

  /**
   * Save acoustic state to a memory buffer
   * @param handle Pointer to detector handle
   * @param buffer Pointer to buffer (of size len) that contains data
   * @param len Length of buffer
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_BUSY if endpointer has already been started
   * @return SWIep_ERROR_BUFFER_OVERFLOW if buffer is too small to fit the data
   */
  SWIepFuncResult ALTAPI SWIepAcousticStateSave(SWIepDetector *handle,
                                                void *buffer,
                                                unsigned int len);

  /**
   * Resets utt-specific state, prepares endpointer for a new utterance
   * @param handle Pointer to detector handle
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_BUSY if endpointer has already been started
   * @return SWIep_ERROR_NO_LICENSE if can't obtain a new license
   *         or no license previously allocated
   */
  SWIepFuncResult ALTAPI SWIepStart(SWIepDetector *handle);

  /**
   * Indicate that the current utterance is complete.
   * @param handle Pointer to detector handle
   * @param code   reason code for terminating the ongoing speech detection 
   *               action
   *
   * @param hints  RESERVED. Pass NULL.
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_INACTIVE if endpointer is not active
   *
   */
  SWIepFuncResult ALTAPI SWIepStop(SWIepDetector *handle, 
                                   SWIepStopCode code, 
                                   const wchar_t* hints);


  /**
   * Called when the prompt is done playing; affects endpointer thresholds
   * @param handle Pointer to detector handle
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_INACTIVE if endpointer is not active
   */
  SWIepFuncResult ALTAPI SWIepPromptDone(SWIepDetector *handle);

  /**
   * Set one of the endpointer parameters wchar_t
   * @param handle Pointer to detector handle
   * @param name name of parameter to set
   * @param value value to set to
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_BUSY if endpointer has already been started
   * @return SWIep_ERROR_INVALID_PARAMETER_VALUE
   *    - value contains non-ascii chars
   *    - value contains data of wrong type (eg chars instead of ints)
   *    - value is out of range for parameter
   * @return SWIep_ERROR_INVALID_PARAMETER if 'name' is unrecognized param
   */
  SWIepFuncResult ALTAPI SWIepSetParameter(SWIepDetector *handle,
                                           const wchar_t *name,
                                           const wchar_t *value);

  /**
   * Get the value for one of the endpointer parameters
   * @param handle Pointer to detector handle
   * @param name name of parameter to set
   * @param value buffer to return parameter value
   * @param len length of buffer
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_BUSY if endpointer has already been started
   * @return SWIep_ERROR_BUFFER_OVERFLOW if buffer isn't large enough
   * @return SWIep_ERROR_INVALID_PARAMETER if 'name' is unrecognized param
   */
  SWIepFuncResult ALTAPI SWIepGetParameter(SWIepDetector *handle,
                                           wchar_t const *name,
                                           wchar_t *value, unsigned int *len);

  /*
   *  The enumeration SWIepState contains the current endpointer state
   *  SWIep_LOOKING_FOR_SPEECH Have not yet found the beginning of speech
   *  SWIep_IN_SPEECH Have found the beginning, but not the end of speech
   *  SWIep_AFTER_SPEECH Have found the beginning and end of speech
   *  SWIep_AUDIO_ERROR The endpointer has encountered a serious error
   */
  typedef enum {
    SWIep_LOOKING_FOR_SPEECH = 0,
    SWIep_IN_SPEECH = 1,
    /* SWIep_NEW_START_SPEECH = 2 removed but need to be consistent with OSR1.1 */
    SWIep_AFTER_SPEECH = 3,
    SWIep_TIMEOUT = 4,
    SWIep_AUDIO_ERROR = 5,
    SWIep_MAX_SPEECH = 6,
    SWIep_IDLE = 7  // internal state after stop and before start
  } SWIepState;

  /**
   * Send in some speech samples; gets back the current endpointer
   * state.  At utterance start, beginSample and endSample are set to -1;
   * whenever they are known, they are set to the proper values, relative to
   * the start of the utterance
   * The amount of begin- and end-of-speech padding used is determined
   * by the relevant endpointer parameters
   *
   * @param handle Pointer to detector handle
   * @param samples audio samples
   * @param state endpointer state (returned)
   * @param beginSample BOS (returned)
   * @param endSample   EOS (returned)
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_BUFFER_OVERFLOW if # of bytes in packet exceeds max of 
   *         4000
   * @return SWIep_ERROR_INVALID_MEDIA_TYPE if samples->type is not valid
   * @return SWIep_ERROR_INACTIVE if endpointer is not active
   * @return SWIep_ERROR_GENERIC_ERROR unspecified error
   */
SWIepFuncResult ALTAPI SWIepWrite(SWIepDetector *handle,
                                  SWIepAudioSamples *samples,
                                  SWIepState *state,
                                  int *beginSample,
                                  int *endSample);

  /**
   * Get speech samples from endpointer's internal buffer
   * @param handle Pointer to detector handle
   * @param samples audio samples (returned)
   * @param state endpointer state (returned)
   * @param maximum length of sample data returned
   *
   * @return SWIep_RESULT_SUCCESS on success
   * @return SWIep_ERROR_INVALID_MEDIA_TYPE if samples->type is not valid
   * @return SWIep_ERROR_BUFFER_OVERFLOW if # of bytes in packet exceeds max_len
  */
  SWIepFuncResult ALTAPI SWIepRead(SWIepDetector *handle,
                                   SWIrecAudioSamples *samples,
                                   SWIepState *state,
                                   int max_len);
  /**
   * Marks the beggining of a new call boundary.
   * 
   * @param channelName specifies the value for the CHAN token in the call log 
   *                    in all records.  This argument is mandatory,
   *                    it may not be NULL or empty string, and must
   *                    be unique for all Recognizer/Detector handles
   *                    in the process.
   *                    
   * @param params      RESERVED. Pass NULL.
   */

  SWIepFuncResult ALTAPI SWIepSessionStart(SWIepDetector *handle,
					   const wchar_t *channelName,
					   const wchar_t *params);


  /**
   * Marks the end of a call boundary. Event information is written to
   * the call logs for analysis.
   */
  SWIepFuncResult ALTAPI SWIepSessionEnd(SWIepDetector *ep);
  
#ifdef __cplusplus
}
#endif

#endif  /* include guard */

