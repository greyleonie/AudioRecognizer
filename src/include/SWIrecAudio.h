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

#ifndef SWIREC_AUDIO_H
#define SWIREC_AUDIO_H

#ifdef __cplusplus
#include <cwchar>
#else
#include <wchar.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  SWIrecSampleStatus indicates how the sample buffer should be handled
 *  SWIrec_SAMPLE_FIRST The sample buffer is the start of audio
 *                      If recognizer was already recognizing, it will discard
 *                      audio received to date and re-start the recognition
 *  SWIrec_SAMPLE_CONTINUE The sample buffer is continuing audio
 *  SWIrec_SAMPLE_LAST The sample buffer is the end of audio
 *                     The recognizer will cease processing audio and
 *                     return results
 *  SWIrec_SAMPLE_SUPPRESSED The sample buffer contains suppressed samples
 *                           Buffer may be empty; length should indicate
 *                           number of bytes of suppressed data
 *  SWIrec_SAMPLE_LOST The sample buffer contains lost samples
 *                     Buffer may be empty; length should indicate number of
 *                     bytes of lost data
 *  SWIrec_SAMPLE_NEW_CHUNK The sample buffer is the start of a new chunk
 *                          (used for magic_word and selective_barge_in)
 *  SWIrec_SAMPLE_END_CHUNK The sample buffer is the end of a chunk
 *                          (used for magic_word and selective_barge_in)
 *  Note that sample statii can be combined; for example, for file-based input
 *  the entire file can be written with SAMPLE_FIRST | SAMPLE_LAST as the
 *  status.
 *  Other flags may be added in future to indicate other special audio
 *  conditions such as the presence of AGC
 */
#define SWIrec_SAMPLE_FIRST      0x01
#define SWIrec_SAMPLE_CONTINUE   0x02
#define SWIrec_SAMPLE_LAST       0x04
#define SWIrec_SAMPLE_SUPPRESSED 0x08
#define SWIrec_SAMPLE_LOST       0x10
#define SWIrec_SAMPLE_NEW_CHUNK  0x20
#define SWIrec_SAMPLE_END_CHUNK  0x40

#define SWIrec_SAMPLE_VALIDBITS  0x7f // to validate the value of sample->status

typedef struct SWIrecAudioSamples
{
  /**
   * Audio sample data.  An audio sample of zero length can be used to
   * indicate last buffer status without including any additional audio
   * samples.
   *
   * @param samples Sample buffer
   * @param len Length of sample buffer, in bytes
   * @param type MEDIA type of samples
   *   "audio/basic" 8-bit 8 KHz u-law encoding [unsigned char *]
   *   "audio/x-alaw-basic" 8-bit 8 KHz A-law encoding [unsigned char *]
   *   "audio/L16;rate=8000" 16-bit 8 KHz linear encoding [short *]
   * @param status Sample status: first buffer, last buffer
   */
  void *samples;
  unsigned int len;
  const wchar_t *type;
  int status;
} SWIrecAudioSamples;

#ifdef __cplusplus
}
#endif

#endif

