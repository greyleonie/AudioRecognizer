# Microsoft Developer Studio Project File - Name="AudioRecognizer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AudioRecognizer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AudioRecognizer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AudioRecognizer.mak" CFG="AudioRecognizer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AudioRecognizer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AudioRecognizer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AudioRecognizer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "AudioRecognizer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "AudioRecognizer - Win32 Release"
# Name "AudioRecognizer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\bank.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cfg.c
# End Source File
# Begin Source File

SOURCE=.\src\D.cpp
# End Source File
# Begin Source File

SOURCE=".\src\data-base.cpp"
# End Source File
# Begin Source File

SOURCE=".\src\make-wav.cpp"
# End Source File
# Begin Source File

SOURCE=".\src\make-zip.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SWIrecSample.c
# End Source File
# Begin Source File

SOURCE=".\src\xml-parser.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\include\bank.h
# End Source File
# Begin Source File

SOURCE=.\src\include\cfg.h
# End Source File
# Begin Source File

SOURCE=.\src\include\D.h
# End Source File
# Begin Source File

SOURCE=.\src\include\NewSig.h
# End Source File
# Begin Source File

SOURCE=.\src\include\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\src\include\SWIepAPI.h
# End Source File
# Begin Source File

SOURCE=.\src\include\SWIrecAPI.h
# End Source File
# Begin Source File

SOURCE=.\src\include\SWIrecAudio.h
# End Source File
# Begin Source File

SOURCE=.\src\include\Tc08a32.h
# End Source File
# Begin Source File

SOURCE=.\src\include\VXIcache.h
# End Source File
# Begin Source File

SOURCE=.\src\include\VXIcompilerMsvc.h
# End Source File
# Begin Source File

SOURCE=.\src\include\VXIheaderPrefix.h
# End Source File
# Begin Source File

SOURCE=.\src\include\VXIheaderSuffix.h
# End Source File
# Begin Source File

SOURCE=.\src\include\VXIinet.h
# End Source File
# Begin Source File

SOURCE=.\src\include\VXItypes.h
# End Source File
# Begin Source File

SOURCE=.\src\include\VXIvalue.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\resource\bank.ico
# End Source File
# Begin Source File

SOURCE=.\src\resource\Bank.rc
# End Source File
# Begin Source File

SOURCE=.\src\resource\resource.h
# End Source File
# End Group
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\lib\SRtrc.lib
# End Source File
# Begin Source File

SOURCE=.\src\lib\VXIvalue.lib
# End Source File
# Begin Source File

SOURCE=.\src\lib\djcvt.lib
# End Source File
# Begin Source File

SOURCE=.\src\lib\NewSig.lib
# End Source File
# Begin Source File

SOURCE=.\src\lib\Tc08a32.lib
# End Source File
# Begin Source File

SOURCE=.\src\lib\SWIttp.lib
# End Source File
# Begin Source File

SOURCE=.\src\lib\SWIep.lib
# End Source File
# Begin Source File

SOURCE=.\src\lib\SWIrec.lib
# End Source File
# End Group
# Begin Group "Grammar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\grammar\building.grxml
# End Source File
# Begin Source File

SOURCE=.\grammar\department.grxml
# End Source File
# Begin Source File

SOURCE=.\grammar\name.grxml
# End Source File
# Begin Source File

SOURCE=.\grammar\number.grxml
# End Source File
# Begin Source File

SOURCE=.\grammar\place.grxml
# End Source File
# Begin Source File

SOURCE=.\grammar\trouble.grxml
# End Source File
# Begin Source File

SOURCE=.\grammar\trouble_report.grxml
# End Source File
# End Group
# Begin Source File

SOURCE=.\recognizer.cfg
# End Source File
# End Target
# End Project
