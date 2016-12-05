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
 * Settings which should start all public headers
 *
 ************************************************************************
 */

/* (1) Platform specific macro which handles symbol exports & imports */

#ifndef _VXIHEADERPREFIX_ONE_TIME_
#ifdef WIN32
  #ifdef __cplusplus
    #define SYMBOL_EXPORT_DECL extern "C" __declspec(dllexport)
    #define SYMBOL_IMPORT_DECL extern "C" __declspec(dllimport)
    #define SYMBOL_EXPORT_CPP_DECL extern __declspec(dllexport)
    #define SYMBOL_IMPORT_CPP_DECL extern __declspec(dllimport)
    #define SYMBOL_EXPORT_CPP_CLASS_DECL __declspec(dllexport)
    #define SYMBOL_IMPORT_CPP_CLASS_DECL __declspec(dllimport)
  #else
    #define SYMBOL_EXPORT_DECL __declspec(dllexport)
    #define SYMBOL_IMPORT_DECL __declspec(dllimport)
  #endif
#else
  #ifdef __cplusplus
    #define SYMBOL_EXPORT_DECL extern "C"
    #define SYMBOL_IMPORT_DECL extern "C"
    #define SYMBOL_EXPORT_CPP_DECL extern
    #define SYMBOL_IMPORT_CPP_DECL extern
    #define SYMBOL_EXPORT_CPP_CLASS_DECL
    #define SYMBOL_IMPORT_CPP_CLASS_DECL
  #else
    #define SYMBOL_EXPORT_DECL extern
    #define SYMBOL_IMPORT_DECL extern
  #endif
#endif

#if !defined(SYMBOL_EXPORT_DECL) || !defined(SYMBOL_IMPORT_DECL)
#error Symbol import/export pair not defined.
#endif
#endif /* end of (1) */


/* Define structure packing conventions */

#ifdef WIN32
#if defined(_MSC_VER)            /* Microsoft Visual C++ */
  #pragma pack(push, 8)
#elif defined(__BORLANDC__)      /* Borland C++ */
  #pragma option -a8
#elif defined(__WATCOMC__)       /* Watcom C++ */
  #pragma pack(push, 8)
#else                            /* Anything else */
  #error Review the settings for your compiler.
#endif

/* Do other (one time) compiler specific work */

#ifndef _VXIHEADERPREFIX_ONE_TIME_
  #if defined(_MSC_VER)
    #include "VXIcompilerMsvc.h"
  #endif
#endif

#endif /*win32*/

/* Define compiler guard for one-time instructions */

#if !defined(_VXIHEADERPREFIX_ONE_TIME_)
#define _VXIHEADERPREFIX_ONE_TIME_
#endif
