/*********************************License***************************************
 * Copyright © 1994-2007.  Nuance Communications.  All rights reserved.
 *
 * Use of this software is subject to certain restrictions and limitations set
 * forth in a license agreement entered into between Nuance Communications and
 * the licensee of this software.  Please refer to the license agreement for
 * license use rights and restrictions.
 *
 * Portions of the Nuance Recognizer Software are subject to copyrights of AT&T
 * Corp., E-Speech Corporation, Bell Communications Research, Inc., European
 * Telecommunications Standards Institute and GlobeTrotter Software, Inc.
 *    
 * SpeechWorks and OpenSpeech are registered trademarks, and SpeechWorks
 * Here,OpenSpeech DialogModules, DialogModules and the SpeechWorks logo are
 * trademarks of Nuance Communications in the United States and other countries.
 */

#ifndef _SWIREC_API_H
#define _SWIREC_API_H

#include "SWIrecAudio.h"

/*****************************************************************************
 *****************************************************************************
 *
 * API definition for Nuance Recognizer
 *
 *****************************************************************************
 ****************************************************************************/

/*
 * C/C++ wchar_t support
 */

#ifdef __cplusplus
#include <cwchar>
#else
#include <wchar.h>
#endif

/*
 * VXIinet interface definition
 */

#include "VXIinet.h"
#include "VXIcache.h"

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

typedef struct SWIrecRecognizer { void *internal; } SWIrecRecognizer;
typedef struct SWIrecResultData { void *internal; } SWIrecResultData;


/*
 *  The enumeration SWIrecError enumerates possible error codes
 *  SWIrec_SUCCESS function completed successfully
 *  SWIrec_ERROR_OUT_OF_MEMORY out of memory
 *  SWIrec_ERROR_SYSTEM_ERROR general system error
 *  SWIrec_ERROR_UNSUPPORTED operation unsupported
 *  SWIrec_ERROR_BUSY the recognizer is busy
 *  SWIrec_ERROR_INITPROC failure during process initialization
 *  SWIrec_ERROR_INVALID_PARAMETER invalid parameter
 *  SWIrec_ERROR_INVALID_PARAMETER_VALUE invalid parameter value
 *  SWIrec_ERROR_INVALID_DATA input data is not valid
 *  SWIrec_ERROR_INVALID_RULE grammar contains invalid rule
 *  SWIrec_ERROR_INVALID_WORD grammar contains invalid word
 *  SWIrec_ERROR_INVALID_NBEST_INDEX input nbestIndex is out of range
 *  SWIrec_ERROR_URI_NOT_FOUND input URI is not found
 *  SWIrec_ERROR_GRAMMAR_ERROR error during grammar parsing (often ECMAscript 
 *   bug)
 *  SWIrec_ERROR_AUDIO_OVERFLOW audio buffer overflow
 *  SWIrec_ERROR_BUFFER_OVERFLOW grammar/string/data buffer overflow
 *  SWIrec_ERROR_NO_ACTIVE_GRAMMARS recognition is started with no grammars
 *   active
 *  SWIrec_ERROR_GRAMMAR_NOT_ACTIVATED can't deactivate a grammar that has
 *   not been activated
 *  SWIrec_ERROR_NO_SESSION_NAME recognition can't start until session names
 *   are defined
 *  SWIrec_ERROR_INACTIVE recognition has not begun
 *  SWIrec_ERROR_NO_DATA no data is present
 *
 *  SWIrec_ERROR_NO_LICENSE a call to SWIrecRecognizerStart() or
 *   SWIrecParseDTMFResults()was attempted but the recognizer does not have
 *   a valid license checked out from the license server -OR- calls to
 *   SWIrecResourceAllocate/Free() fail because no license server is
 *   available
 *  SWIrec_ERROR_LICENSE_ALLOCATED tried to allocate a license to recognizer
 *   object but it already has a license allocated to it
 *  SWIrec_ERROR_LICENSE_FREED tried to free a license from recognizer object
 *   but no license is currently allocated to it
 *  SWIrec_ERROR_LICENSE_COMPROMISE license server has been unreachable for
 *   greater than the allowed "downtime" window (61-89 seconds)
 *
 *  SWIrec_ERROR_INVALID_MEDIA_TYPE an api or server supplied media type is
 *   invalid
 *
 *  SWIrec_ERROR_URI_TIMEOUT a uri fetch resulted in a timeout
 *  SWIrec_ERROR_URI_FETCH_ERROR a uri fetch resulted in an error
 *
 *  SWIrec_ERROR_INVALID_SYSTEM_CONFIGURATION baseline is invalid
 *  SWIrec_ERROR_INVALID_LANGUAGE a language is not supported
 *  SWIrec_ERROR_DUPLICATE_GRAMMAR an activate of the same grammar is
 *   attempted twice
 *
 *  SWIrec_ERROR_INVALID_CHAR an invalid character is passed in an api call
 *   (for example, all uri's must use ascii characters also surrogate pairs
 *    are illegal)
 *
 *  SWIrec_ERROR_GENERIC_FAILURE generic failure condition, retry is possible
 *  SWIrec_ERROR_GENERIC_ERROR generic recoverable error occurred
 *  SWIrec_ERROR_GENERIC_FATAL_ERROR generic fatal error occurred
 *
 *  The following are no longer used, but the return codes have not been 
 *  removed to keep the numbering of subsequent return codes unchanged in case
 *  the application depended on specific values and not the enum name:
 *    SWIrec_ERROR_SERVER_UNAVAILABLE              = 21,
 *    SWIrec_ERROR_SERVER_CONNECTION_FAILED        = 22,
 *    SWIrec_ERROR_SERVER_NO_CHANNELS_AVAILABLE    = 23,
 *    SWIrec_ERROR_SERVER_NOT_ALLOCATED            = 35,
 *    SWIrec_ERROR_SERVER_ALLOCATED                = 36,
 *    SWIrec_ERROR_SERVER_FREED                    = 37,
 *
 */

typedef enum {
  SWIrec_SUCCESS                               = 0,
  SWIrec_ERROR_OUT_OF_MEMORY                   = 1,
  SWIrec_ERROR_SYSTEM_ERROR                    = 2, 
  SWIrec_ERROR_UNSUPPORTED                     = 3,
  SWIrec_ERROR_BUSY                            = 4,
  SWIrec_ERROR_INITPROC                        = 5,
  SWIrec_ERROR_INVALID_PARAMETER               = 6,
  SWIrec_ERROR_INVALID_PARAMETER_VALUE         = 7,
  SWIrec_ERROR_INVALID_DATA                    = 8,
  SWIrec_ERROR_INVALID_RULE                    = 9,
  SWIrec_ERROR_INVALID_WORD                    = 10,
  SWIrec_ERROR_INVALID_NBEST_INDEX             = 11,
  SWIrec_ERROR_URI_NOT_FOUND                   = 12,
  SWIrec_ERROR_GRAMMAR_ERROR                   = 13,
  SWIrec_ERROR_AUDIO_OVERFLOW                  = 14,
  SWIrec_ERROR_BUFFER_OVERFLOW                 = 15,
  SWIrec_ERROR_NO_ACTIVE_GRAMMARS              = 16,
  SWIrec_ERROR_GRAMMAR_NOT_ACTIVATED           = 17,
  SWIrec_ERROR_NO_SESSION_NAME                 = 18,
  SWIrec_ERROR_INACTIVE                        = 19,
  SWIrec_ERROR_NO_DATA                         = 20,
  SWIrec_ERROR_SERVER_UNAVAILABLE              = 21, /* Not used */
  SWIrec_ERROR_SERVER_CONNECTION_FAILED        = 22, /* Not used */
  SWIrec_ERROR_SERVER_NO_CHANNELS_AVAILABLE    = 23, /* Not used */
  SWIrec_ERROR_NO_LICENSE                      = 24,
  SWIrec_ERROR_LICENSE_ALLOCATED               = 25,
  SWIrec_ERROR_LICENSE_FREED                   = 26,
  SWIrec_ERROR_LICENSE_COMPROMISE              = 27,

  SWIrec_ERROR_INVALID_MEDIA_TYPE              = 28,

  SWIrec_ERROR_URI_TIMEOUT                     = 29,
  SWIrec_ERROR_URI_FETCH_ERROR                 = 30,

  SWIrec_ERROR_INVALID_SYSTEM_CONFIGURATION    = 31,
  SWIrec_ERROR_INVALID_LANGUAGE                = 32,
  SWIrec_ERROR_DUPLICATE_GRAMMAR               = 33,

  SWIrec_ERROR_INVALID_CHAR                    = 34,

  SWIrec_ERROR_SERVER_NOT_ALLOCATED            = 35, /* Not used */
  SWIrec_ERROR_SERVER_ALLOCATED                = 36, /* Not used */
  SWIrec_ERROR_SERVER_FREED                    = 37, /* Not used */

  SWIrec_ERROR_GENERIC_FAILURE                 = 0x0FFFF, /* 65535 */
  SWIrec_ERROR_GENERIC_ERROR                   = 0x1FFFF, /* 131071 */
  SWIrec_ERROR_GENERIC_FATAL_ERROR             = 0x2FFFF  /* 196607 */

} SWIrecFuncResult;

  /* -- start docme interface -- */

  /*
   * The following are generic errors that should always be checked for:
   * SWIrec_SYSTEM_ERROR if SWIrecInit hasn't been called or if some
   *   non-memory-allocation OS request fails
   * SWIrec_INVALID_PARAMETER_VALUE if any required parameters are NULL or
   *   zero-length strings, etc
   * SWIrec_OUT_OF_MEMORY if a memory allocation failed
   *
   * SWIrec_ERROR_INVALID_CHAR if invalid characters are used in strings
   *
   * SWIrec_ERROR_GENERIC_FAILURE unspecified failure
   * SWIrec_ERROR_GENERIC_ERROR unspecified error
   * SWIrec_ERROR_GENERIC_FATAL_ERROR unspecified fatal error
   */

/**
 * Global initialization.
 * Should be invoked once at process start-up.
 * This function can only be called once per process.
 *
 * @param uri An object containing the global config URI and fetch params
 *            If NULL, uses default system URI specified at install time
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY
 *         if it has been called previously.
 * @return SWIrec_ERROR_INVALID_LANGUAGE if default language is invalid
 * @return SWIrec_ERROR_INITPROC 
 *      any unspecified error during initialization (e.g. license failure)
 */
SWIrecFuncResult ALTAPI SWIrecInit(const wchar_t *uri);


/**
 * Global shutdown.
 * Should be invoked once at process termination.
 * This function can only be called once per process.
 * This function can be called even when there are outstanding recognitions.
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY
 *         if it has been called previously or SWIrecInit was never called
 */
SWIrecFuncResult ALTAPI SWIrecTerminate(void);


/**
 * Reset acoustic state of a recognizer.
 * Must be called at start of each new call.
 *
 * @param rec Recognizer handle
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 */
SWIrecFuncResult ALTAPI SWIrecAcousticStateReset(SWIrecRecognizer *rec);


/**
 * Query size of acoustic state.
 * Should be used to query buffer size for AcousticStateSave.
 *
 * @param rec Recognizer handle
 * @param size Size of acoustic state object
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 */
SWIrecFuncResult ALTAPI SWIrecAcousticStateQuerySize(SWIrecRecognizer *rec,
                                                     unsigned int *size);


/**
 * Read the acoustic state from a memory buffer.
 *
 * @param rec Recognizer handle
 * @param buffer Pointer to memory
 * @param len Length of buffer
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUFFER_OVERFLOW
 *         if buffer is too small
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 * @return SWIrec_ERROR_INVALID_DATA if buffer
 *         contents do not represent acoustic state information.
 */
SWIrecFuncResult ALTAPI SWIrecAcousticStateLoad(SWIrecRecognizer *rec,
                                                void *buffer,
                                                unsigned int len);


/**
 * Write the acoustic state to a memory buffer.
 *
 * @param rec Recognizer handle
 * @param buffer Pointer to memory
 * @param len Space available in buffer, in bytes
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUFFER_OVERFLOW
 *         if buffer is too small
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 */
SWIrecFuncResult ALTAPI SWIrecAcousticStateSave(SWIrecRecognizer *rec,
                                                void *buffer,
                                                unsigned int len);


/**
 * Create a recognizer resource.
 *
 * @param rec Recognizer handle
 * @param inet Interface to VXIinet implementation (for grammar caching)
 *             If NULL, default SpeechWorks implementation will be used
 *
 * @return SWIrec_ERROR_NO_LICENSE
 *         in "default" licensing mode, no more licenses are available
 * @return SWIrec_SUCCESS on success
 */
SWIrecFuncResult ALTAPI SWIrecRecognizerCreate(SWIrecRecognizer **rec,
                                               VXIinetInterface *inet,
                                               VXIcacheInterface *cache);


/**
 * Destroy and de-register a recognizer resource.
 *
 * @param rec Recognizer handle
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY
 *         if called while recognition is active.
 */
SWIrecFuncResult ALTAPI SWIrecRecognizerDestroy(SWIrecRecognizer *rec);


typedef struct SWIrecGrammarData
{
  /**
   * Grammar identifier
   *
   * @param type Grammar type ("uri", "uri/2.0", "string", or "string/2.0")
   * @param data 
   *        type uri, uri/2.0: a pointer to the uri
   *        type string: a pointer to the string grammar
   *        type string/2.0: NULL
   * @param properties URI caching properties for VXIinet to use-can be set 
   *                   to NULL
   *        Properties understood only by SWIrec:
   *        - SWIREC_DISK_CACHE_LOCK :  When Loading or Compiling a grammar, 
   *                 lock the cached grammar into the disk cache.
   * @param media_type 
   *        type uri,string: should be NULL
   *        type string/2.0: "text/xml", "application/srgs+xml", 
   *                         "application/x-swi-grammar"
   *        type uri/2.0: all of the string/2.0 types + NULL
   * @param binary_data
   *        type uri,uri/2.0,string: should be NULL
   *        type string/2.0: a pointer to the data
   * @param length length of binary_data (if set)
   */
  wchar_t *type;
  wchar_t *data;
  const VXIMap *properties;
  wchar_t *media_type;
  void *binary_data;
  unsigned int length;

} SWIrecGrammarData;


/**
 * Property to indicate that grammar should be locked into the disk cache
 * when loaded successfully.  This is an integer property, set to non-zero
 * to lock the grammar into the disk cache, set to zero or do not specify
 * for normal operation. VXIInteger
 */
#define SWIREC_DISK_CACHE_LOCK L"swirec_disk_cache_lock"

/**
 * Property to indicate that grammar should be locked into the memory cache
 * when loaded successfully.  This is an integer property, set to non-zero
 * to lock the grammar into the disk cache, set to zero or do not specify
 * for normal operation.  VXIInteger
 */
#define SWIREC_MEMORY_CACHE_LOCK L"swirec_memory_cache_lock"


/**
 * Property to indicate if a grammar should be flushed from memory
 * immediately when SWIrecGrammarFree() is called.  VXIInteger
 */
#define SWIREC_MEM_FLUSH_ON_FREE L"swirec_mem_flush_on_free"


/**
 * Property to indicate what language to use for the grammar. VXIString
 */
#define SWIREC_LANGUAGE L"swirec_language"

/**
 * Property to indicate that grammar should always be disk cached, whether
 * or not its a local grammar binary file. (By default local grammar
 * binaries are not cached). VXIInteger
 */
#define SWIREC_DISK_CACHE_FORCE L"swirec_disk_cache_force"

/**
 * Load a complete grammar from a URI or string.
 *    The grammar has to be activated.
 *    The grammar is identified by use of the same URI/string identifier.
 *    There is support for recursive loading of grammars
 *       (i.e. a grammar definition that includes another grammar).
 *    This recursively is handled at the recognizer level (URL fetching).
 *    There is no support for cross-reference grammars
 *    Support for VoiceXML built-in types (e.g., grammar
 *            src=builtin:grammar/digits?length=5).
 *
 * @param rec Recognizer handle
 * @param grammar An object containing the URI and fetch params, or string
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 * @return SWIrec_ERROR_NO_SESSION_NAME if recognizer doesn't have a session 
 *         name
 * @return SWIrec_ERROR_INVALID_PARAMETER
 *         if grammar type is not valid
 * @return SWIrec_ERROR_INVALID_PARAMETER_VALUE if the grammar
 *         sets a config parameter to an invalid value via a <meta> tag".
 * @return SWIrec_ERROR_INVALID_DATA if data is
 *         not a valid grammar string (for string grammars);
 *         if value of <meta> parameter in the grammar is invalid
 * @return SWIrec_ERROR_URI_NOT_FOUND,URI_TIMEOUT,URI_FETCH_ERROR
 *         A URI fetch that was necessary to complete the task was not found,
 *         or resulted in a timeout or error
 * @return SWIrec_ERROR_INVALID_DATA if URI
 *         does not point to a valid grammar or if any import URIs do not
 *         point to a valid grammar.
 * @return SWIrec_ERROR_INVALID_MEDIA_TYPE
 *         if media type supplied in structure or by server is invalid
 * @return SWIrec_ERROR_INVALID_LANGUAGE
 *         if language specified in grammar or any imports is invalid
 */
SWIrecFuncResult ALTAPI SWIrecGrammarLoad(SWIrecRecognizer *rec,
                                          const SWIrecGrammarData *grammar);


/**
 * Compile a grammar to a memory buffer.
 * See GrammarLoad for error return codes when there is an error in the
 * grammar specification.
 *
 * @param rec Recognizer handle
 * @param grammar An object containing the URI and fetch params, or string
 * @param buffer Pointer to compiled grammar
 * @param len Length (in bytes) of buffer passed in;
 *             if too short, required length will be returned here
 *             and SWIrec_ERROR_BUFFER_OVERFLOW will be returned.
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY if called while recognition is active.
 * @return SWIrec_ERROR_BUFFER_OVERFLOW if buffer is too small
 * @return SWIrec_ERROR_INVALID_PARAMETER
 *         if grammar type is not valid
 * @return SWIrec_ERROR_INVALID_DATA if data is
 *         not a valid grammar string (for string grammars)
 * @return SWIrec_ERROR_URI_NOT_FOUND,URI_TIMEOUT,URI_FETCH_ERROR
 *         A URI fetch that was necessary to complete the task was not found,
 *         or resulted in a timeout or error
 * @return SWIrec_ERROR_INVALID_DATA if URI
 *         does not point to a valid grammar or if any import URIs do not
 *         point to a valid grammar.
 * @return SWIrec_ERROR_INVALID_MEDIA_TYPE
 *         if media type supplied in structure or by server is invalid
 * @return SWIrec_ERROR_INVALID_LANGUAGE
 *         if language specified in grammar or any imports is invalid
 */
SWIrecFuncResult ALTAPI SWIrecGrammarCompile(SWIrecRecognizer *rec,
                                             const SWIrecGrammarData *grammar,
                                             void *buffer,
                                             unsigned int *len);


/**
 * Activate a grammar for subsequent recognition calls.
 * More than one grammar can be activated at any given time.
 * The grammar must have been loaded with GrammarLoad().
 * See GrammarLoad for error return codes when there is an error in the grammar
 * specification.
 *
 * @param rec Recognizer handle
 * @param grammar An object containing the URI and fetch params, or string
 * @param weight Relative weight to assign to this grammar vs. other activated
 *               grammars
 * @param grammarID The ID that is returned for the SWI_grammarName key to
 *               identify which grammar each nbest answer the answer came from
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 * @return SWIrec_DUPLICATE_GRAMMAR
 *         if the same grammar has already been activated
 * @return SWIrec_ERROR_NO_SESSION_NAME if recognizer doesn't have session name
 * @return GrammarLoad-errors 
 *         Grammar activation may cause a grammar load so all those errors may 
 *         be returned.
 */
SWIrecFuncResult ALTAPI SWIrecGrammarActivate(SWIrecRecognizer *rec,
                                              const SWIrecGrammarData *grammar,
                                              unsigned int weight,
                                              const char *grammarID);


/**
 * Deactivate a loaded grammar for subsequent recognition calls.
 * See GrammarLoad for error return codes when there is an error in the
 * grammar specification.
 *
 * @param rec Recognizer handle
 * @param grammar An object containing the URI and fetch params, or string
 *                NULL deactivates all grammars
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 * @return SWIrec_GRAMMAR_NOT_ACTIVATED
 *         if the grammar is not currently active.
 */
SWIrecFuncResult ALTAPI SWIrecGrammarDeactivate(SWIrecRecognizer *rec,
                                                const SWIrecGrammarData *grammar);


/**
 * Signal rec interface that grammar is no longer needed.
 * The interface can recover resources as it sees fit.
 *
 * @param rec Recognizer handle
 * @param grammar An object containing the URI and fetch params, or string
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 * @return SWIrec_ERROR_NO_SESSION_NAME if recognizer doesn't have a session 
 *         name
 */
SWIrecFuncResult ALTAPI SWIrecGrammarFree(SWIrecRecognizer *rec,
                                          const SWIrecGrammarData *grammar);


/**
 * Set recognition parameters
 * Parameter values are valid for the during of a single recognition.
 * Must be called before RecognizerStart() and will take effect for the next
 * recognition.
 * Refer to the reference documentation for parameter descriptions
 * and legal values.
 *
 * @param rec Recognizer handle
 * @param param Parameter name
 * @param value String value to set
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 * @return SWIrec_ERROR_INVALID_PARAMETER
 *         if param is not a valid parameter.
 * @return SWIrec_ERROR_INVALID_PARAMETER_VALUE
 *         if value is not valid.
 */
SWIrecFuncResult ALTAPI SWIrecRecognizerSetParameter(SWIrecRecognizer *rec,
                                                     const wchar_t *param,
                                                     const wchar_t *value);


/**
 * Get recognition parameters
 * Parameter values are valid for the during of a single recognition.
 * Not all parameter values may be retrieved
 * Refer to the reference documentation for parameter descriptions
 * and legal values.
 *
 * @param rec Recognizer handle
 * @param param Parameter name
 * @param value Pointer to parameter string retrieved
 * @param len Length of string buffer passed in;
 *             if too short, required length will be returned here
 *             and SWIrec_ERROR_BUFFER_OVERFLOW will be set
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUFFER_OVERFLOW when size of value string is too small
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 * @return SWIrec_ERROR_INVALID_PARAMETER if
 *         param is not a valid parameter.
 */
SWIrecFuncResult ALTAPI SWIrecRecognizerGetParameter(SWIrecRecognizer *rec,
                                                     const wchar_t *param,
                                                     wchar_t *value,
                                                     unsigned int *len);


/**
 * Marks the begining of a call session.
 * 
 * @param channelName specifies the value for the CHAN token in the call log 
 *                    in all records.  This argument is mandatory, it may not be
 *                    NULL or empty string, and must be unique for all
 *                    Recognizer/Detector handles in the process.
 * @param params      Not currently used.  Pass NULL.
 */
SWIrecFuncResult ALTAPI SWIrecSessionStart(SWIrecRecognizer *rec, 
                                           const wchar_t *channelName, 
                                           const wchar_t *params);


/**
 * Marks the end of the call session that began in SWIrecSessionStart().
 */
SWIrecFuncResult ALTAPI SWIrecSessionEnd(SWIrecRecognizer *rec);


/**
 * Start recognition using currently active grammars, variable bindings,
 * and property values.  This function returns immediately.  Results can
 * be computed by calling RecognizerCompute().
 *
 * @param rec Recognizer handle
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_NO_ACTIVE_GRAMMARS if no grammars have been activated.
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active (RecognizerStart has already
 *         been called without RecognizerStop being called or normal
 *         termination of recognition).
 * @return SWIrec_ERROR_NO_SESSION_NAME if SetSessionName has not been called.
 * @return SWIrec_ERROR_NO_LICENSE
 *         if license checkout request fails (JIT mode) or no license checked
 *         out (explicit mode)
 */
SWIrecFuncResult ALTAPI SWIrecRecognizerStart(SWIrecRecognizer *rec);


/**
 * Send audio samples to recognizer during recognition.
 * This function returns immediately.
 * The recognizer will return an appropriate audio status event via
 * RecognizerCompute() as soon as one of the following conditions
 * is detected:
 *  - The audio input is definitely not speech.
 *  - The audio input is definitely speech.
 *  - An end-of-speech or max-speech condition has been reached.
 * Only one of the first two statii will be returned per recognition.
 * AudioWrite can be called before RecognizerStart.  When RecognizerStart
 * is called, audio buffers are ignored until one is found with
 * SWIrec_SAMPLE_FIRST.
 *
 * @param rec Recognizer handle
 * @param samples Sample data object
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_INVALID_PARAMETER
 *         if a. invalid status specified b. samples->len < 0 
 *         c. invalid combination of samples->len, samples->samples, and audio
 *         type
 * @return SWIrec_ERROR_NO_SESSION_NAME if recognizer doesn't have a session 
 *         name
 * @return SWIrec_ERROR_BUFFER_OVERFLOW samples->len > 4000
 * @return SWIrec_ERROR_INVALID_MEDIA_TYPE 
 *         if audio-media type is invalid
 */
SWIrecFuncResult ALTAPI SWIrecAudioWrite(SWIrecRecognizer *rec,
                                         SWIrecAudioSamples *samples);


/*
 *  The enumeration SWIrecRecognizerStatus contains the recognition status
 *  SWIrec_STATUS_SUCCESS successful recognition with complete results
 *  SWIrec_STATUS_NO_MATCH recognition failure or rejected magic word candidate
 *  SWIrec_STATUS_INCOMPLETE recognizer needs more time to compute results
 *                           partial results may be returned
 *  SWIrec_STATUS_NON_SPEECH_DETECTED input to date has been identified
 *                                    as not being speech
 *  SWIrec_STATUS_SPEECH_DETECTED input to date has been identified as speech
 *  SWIrec_STATUS_SPEECH_COMPLETE recognizer will not process any more speech
 *  SWIrec_STATUS_MAX_CPU_TIME CPU time limit exceeded
 *  SWIrec_STATUS_MAX_SPEECH maximum speech length exceeded
 *                           complete results may be returned
 *  SWIrec_STATUS_STOPPED recognition was stopped
 *  SWIrec_STATUS_REJECTED magic word chunk rejected due to low confidence
 */
typedef enum {
  SWIrec_STATUS_SUCCESS = 0,
  SWIrec_STATUS_NO_MATCH = 1,
  SWIrec_STATUS_INCOMPLETE = 2,
  SWIrec_STATUS_NON_SPEECH_DETECTED = 3,
  SWIrec_STATUS_SPEECH_DETECTED = 4,
  SWIrec_STATUS_SPEECH_COMPLETE = 5,
  SWIrec_STATUS_MAX_CPU_TIME = 6,
  SWIrec_STATUS_MAX_SPEECH = 7,
  SWIrec_STATUS_STOPPED = 8,
  SWIrec_STATUS_REJECTED = 9,
  SWIrec_STATUS_NO_SPEECH_FOUND = 10,
} SWIrecRecognizerStatus;


#define SWIrec_STATUS_FAILURE SWIrec_STATUS_NO_MATCH

/*
 *  The enumeration SWIrecResultType enumerates possible result types
 *  SWIrec_RESULT_TYPE_COMPLETE complete results from full recognition of
 *    audio
 *  SWIrec_RESULT_TYPE_PARTIAL partial results if there is a match to grammar
 *   (must be enabled with grammar parameter swirec_allow_partial_results)
 *  SWIrec_RESULT_TYPE_NONE no results at this time
 */
typedef enum {
  SWIrec_RESULT_TYPE_COMPLETE = 0,
  SWIrec_RESULT_TYPE_PARTIAL = 1,
  SWIrec_RESULT_TYPE_NONE = 2
} SWIrecResultType;


/**
 * Compute results for current recognition.
 * Blocks until recognizer produces complete or partial recognition results
 * (as requested); or is halted; or returns an audio status.
 *
 * The results data are valid until the next call to RecognizerStart(),
 * RecognizerCompute(), or ParseDTMFResults().  If the status returned by
 * RecognizerCompute() is SUCCESS, FAILURE, STOPPED, MAX_SPEECH, or
 * MAX_CPU_TIME, and complete results were requested, then subsequent calls
 * to RecognizerCompute() return the same status value until the next call to
 * RecognizerStart().
 *
 * @param rec Recognizer handle
 * @param maxComputeTime Maximum time to compute results during this call
 *                       (in ms of real time); -1 indicates block until
 *                       completion or change in state; 
 * @param status Status of recognition upon completion
 * @param type Type of results returned.
 * @param resultData  Result vector containing results of current
 *                    recognition including, recognized string,
 *                    confidence, key/value list w/ confidences.
 *                    Results are only returned if result type is
 *                    PARTIAL or COMPLETE.
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_GRAMMAR_ERROR
 *         run-time error during grammar parsing (usually ECMAscript bug)
 * @return SWIrec_ERROR_INVALID_PARAMETER_VALUE
 *         input parameter maxComputeTime is 0
 * @return SWIrec_ERROR_INACTIVE if
 *         RecognizerStart has not been called.
 */
SWIrecFuncResult ALTAPI SWIrecRecognizerCompute(SWIrecRecognizer *rec,
                                                int maxComputeTime,
                                                SWIrecRecognizerStatus *status,
                                                SWIrecResultType *type,
                                                SWIrecResultData **resultData);


/**
 * Retrieve the waveform for the last recognition utterance
 * This function will succeed only if swirec_return_waveform was set for this
 * recognition
 * The waveform is in 8 Khz u-law format
 *
 * @param resultData Pointer to results data structure
 * @param type Parameter-value pair indicating what kind of transformation to 
 *             perform on waveform, e.g. 
 *             "silence_suppression=begin+end+interword"
 * @param waveform Pointer to waveform buffer within results data
 * @param len Length of waveform in bytes
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_NO_DATA
 *         if waveform saving was not enabled or recognition failed
 */
SWIrecFuncResult ALTAPI SWIrecGetWaveform(SWIrecResultData *resultData,
                                          const wchar_t *type,
                                          void **waveform,
                                          unsigned int *len);


/*
 *  The enumeration SWIrecParseStatus contains parser status
 *   (for parsing DTMF strings)
 *  SWIrec_PARSE_COMPLETE valid parse, no further input possible
 *  SWIrec_PARSE_VALID_PREFIX valid parse, further input possible
 *  SWIrec_PARSE_INCOMPLETE invalid parse, valid parse possible with
 *                          further input
 *  SWIrec_PARSE_INVALID valid parse not possible
 */
typedef enum {
  SWIrec_PARSE_COMPLETE = 0,
  SWIrec_PARSE_VALID_PREFIX = 1,
  SWIrec_PARSE_INCOMPLETE = 2,
  SWIrec_PARSE_INVALID = 3
} SWIrecParseStatus;


/**
 * Return an XML result
 *
 * As with other result functions, the results are valid until the
 * next call to RecognizerStart(), RecognizerCompute(), or this
 * function again
 *
 * @param resultData  Result vector containing results of current recognition 
 *                    including, recognized string, confidence, key/value list
 *                    w/ confidences
 * @param format      Format of the XML grammar
 * @param xmlResult   XML result to be returned
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_INVALID_MEDIA_TYPE
 *         if format is invalid
 */
SWIrecFuncResult ALTAPI SWIrecGetXMLResult(SWIrecResultData *result_data,
                                           const wchar_t *format,
                                           wchar_t **xmlResult);


/**
 * Parse a DTMF string against the currently active grammars
 * At least one active grammar must specify rules for parsing DTMF input
 * Aborts recognition of speech and discards all audio input
 *
 * The results data are valid until the next call to RecognizerStart(),
 * RecognizerCompute(), or ParseDTMFResults().
 *
 * @param rec Recognizer handle
 * @param str DTMF string
 * @param status Status of parse
 * @param resultData  Result vector containing results of current
 *                    recognition including, recognized string,
 *                    confidence, key/value list w/ confidences
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_BUSY if
 *         called while recognition is active.
 * @return SWIrec_ERROR_INVALID_PARAMETER
 *         if str is not a valid DTMF string.
 */
SWIrecFuncResult ALTAPI SWIrecParseDTMFResults(SWIrecRecognizer *rec,
                                               const char *str,
                                               SWIrecParseStatus *status,
                                               SWIrecResultData **resultData);


/**
 * Write an event and related information into the recognizer event log
 *
 * @param rec Recognizer handle
 * @param event Event name
 * @param value String to be logged.  This value is recommended to be composed 
 *              of a token/value list which matches the recognizer logging
 *              format.
 *
 * @return SWIrec_SUCCESS on success
 */
SWIrecFuncResult ALTAPI SWIrecLogEvent(SWIrecRecognizer *rec,
                                       const wchar_t *event,
                                       const wchar_t *value);


/**
 * SWIrecStopCode specifies reasons why the recognizer was stopped
 * 
 * @enum   SWIrecStopCode
 * @brief  The reason code is used for better adaptation within a call boundary 
 *         and to log information for tuning.
 */
typedef enum {
  SWIrec_STOP_SPEECH  = 0,  /* Caller used speech */
  SWIrec_STOP_DTMF    = 1,  /* Caller used DTMF */
  SWIrec_STOP_HANGUP  = 2,  /* Caller hung up */
  SWIrec_STOP_TIMEOUT = 4,  /* No speech detected; timeout */
  SWIrec_STOP_OTHER   = 8   /* Other reason not covered above */
} SWIrecStopCode;


/**
 * Stop current recognition.
 * Aborts recognition of speech, discards all audio input, and discard all
 * temporary recognition storage for this utterance.
 *
 * This function should be called to interrupt a currently active
 * SWIrecRecognizerCompute() or after SWIrecRecognizerCompute() returns if no
 * more recognition is required on the current utterance.
 * This function must be called before RecognizerStart() can be called unless
 * RecognizerCompute() returned a status (NOT return code) of:
 * SUCCESS, FAILURE, MAX_SPEECH, or MAX_CPU_TIME.
 * Subsequent calls to RecognizerCompute() will return a STOPPED status until
 * RecognizerStart() is called.  Subsequent calls to ParseDTMFResults() are
 * still valid.
 *
 * @param rec Recognizer handle
 * @param code Reason for the stop.  This should be one of: SWIrec_STOP_SPEECH,
 *             SWIrec_STOP_DTMF, SWIrec_STOP_HANGUP, SWIrec_STOP_TIMEOUT, 
 *             SWIrec_STOP_OTHER
 *
 * @return SWIrec_SUCCESS on success
 * @return SWIrec_ERROR_INACTIVE if recognition is not active.
 */
SWIrecFuncResult ALTAPI SWIrecRecognizerStop(SWIrecRecognizer *rec, 
                                             SWIrecStopCode code);


/**
 * Clean up memory allocation for thread.  Releases any thread-specific storage.
 */
SWIrecFuncResult ALTAPI SWIrecThreadCleanup(void);


/*
 * Sets the supplied cookie jar on the inet implementation.  The read-only
 * string is assumed to be a newline-delimited set of Set-Cookie or Set-Cookie2
 * HTTP-like headers.  For example:
 * 
 * Set-Cookie:name=<cookie-name>;value=<cookie-value>;expires=<cookie-expiration>;...
 * Set-Cookie2:name=<cookie-name>;value=<cookie-value>;expires=<cookie-expiration>;...
 *
 * Note that in both the Cookie and Cookie2 cases, only the name and value
 * attributes are required.  The remaining attributes are optional as usual.
 * The complete list of supported key-value pairs can be found in the VXIinet
 * header file.

 */
SWIrecFuncResult ALTAPI SWIrecSetCookieJar(SWIrecRecognizer *rec,
                                           const wchar_t *cookies);


/*
 * Populates the supplied cookie jar buffer.  This buffer is assumed to be
 * large enough to hold the complete cookie set, and will result in an
 * non-zero return code and invalid buffer if this is not the case.
 *
 * The format of the return string is identical to the SWIrecSetCookieJar()
 * function which itself is similar to HTTP headers.
 */
SWIrecFuncResult ALTAPI SWIrecGetCookieJar(SWIrecRecognizer *rec,
                                           wchar_t *cookies);


#ifdef __cplusplus
}
#endif

/* Reset the structure packing alignments for different compilers. */

#if defined(_MSC_VER)            /* Microsoft Visual C++ */
  #pragma pack(pop)
#elif defined(__BORLANDC__)      /* Borland C++ */
  #pragma option -a.
#elif defined(__WATCOMC__)       /* Watcom C++ */
  #pragma pack(pop)
#endif

#endif  /* include guard */
