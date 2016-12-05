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
 *
 ************************************************************************
 *
 * Basic VXI Types
 *
 * The following VXI types are used in the VXI interfaces and
 * implementation to improve portability. The basic VXI types are:
 *
 *   VXIchar        Locality dependent char type
 *   VXIbool        Boolean with values TRUE or FALSE
 *   VXIbyte        Single byte
 *   VXIint         Native platform int
 *   VXIunsigned    Native platform unsigned int
 *   VXIint32       32-bit int
 *   VXIint64       64-bit int
 *   VXIuint64      64-bit unsigned int
 *   VXIlong        Native platform long
 *   VXIulong       Native platform unsigned long
 *   VXIflt32       32-bit IEEE float
 *   VXIflt64       64-bit IEEE float
 *   VXIptr         Untyped pointer
 *
 ************************************************************************
 */

#ifndef _VXITYPES_H
#define _VXITYPES_H

#ifdef __cplusplus
#include <cwchar>         /* C++ wchar_t support */
#else
#include <wchar.h>        /* C wchar_t support */
#endif
#ifndef WIN32
#include <pthread.h>      /* For pthread_t */
#endif

#include "VXIheaderPrefix.h"

 /**
  * @name VXItypes
  * @memo VXI type definitions
  * @version 1.0
  * @doc
  * VXI prefixed primitive types to ensure portability across a wide
  * variety of operating systems. These types are used throughout all
  * the VXI interfaces as well as within implementations of those
  * interfaces.  
  */

/*@{*/

/**
 * i386-* bindings
 */


typedef unsigned int   VXIbool;
typedef unsigned char  VXIbyte;
typedef wchar_t        VXIchar;
typedef int            VXIint;
typedef unsigned int   VXIunsigned;
typedef int            VXIint32;
#ifndef WIN32
typedef long long              VXIint64;
typedef unsigned long long     VXIuint64;
#else
typedef __int64              VXIint64;
typedef unsigned __int64     VXIuint64;
#endif
typedef long           VXIlong;
typedef unsigned long  VXIulong;
typedef float          VXIflt32;
typedef double         VXIflt64;
typedef void *         VXIptr;
#ifdef WIN32
typedef VXIlong        VXIthreadID;
#else
typedef pthread_t      VXIthreadID;
#endif

/**
 * Common MIME content types used for multiple interfaces
 */
#define VXI_MIME_SRGF              L"application/grammar+xml"
#define VXI_MIME_SSML              L"application/synthesis+ssml"
#define VXI_MIME_VXML              L"application/voicexml+xml"

#define VXI_MIME_TEXT              L"text/plain"
#define VXI_MIME_UNICODE_TEXT      L"text/plain;charset=wchar_t"
#define VXI_MIME_UTF16_TEXT        L"text/plain;charset=UTF-16"
#define VXI_MIME_XML               L"text/xml"

#define VXI_MIME_ALAW              L"audio/x-alaw-basic"
#define VXI_MIME_LINEAR            L"audio/L8;rate=8000"
#define VXI_MIME_LINEAR_16         L"audio/L16;rate=8000"
#define VXI_MIME_LINEAR_16_16KHZ   L"audio/L16;rate=16000"
#define VXI_MIME_ULAW              L"audio/basic"
#define VXI_MIME_VOX               L"audio/x-dialogic-vox"
#define VXI_MIME_WAV               L"audio/x-wav"

/**
 * Current VXI interface version
 */
#define VXI_CURRENT_VERSION     0x00020000  /* version 1.1 */
#define VXI_MAJOR_VERSION(x)    (((x) >> 16) & 0xFFFF)
#define VXI_MINOR_VERSION(x)    ((x) & 0xFFFF)
#define VXI_CURRENT_VERSION_STR L"2.0"

/**
 * True and false for VXIbool values
 */
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/*@}*/

#include "VXIheaderSuffix.h"

#endif /* end of include guard */
