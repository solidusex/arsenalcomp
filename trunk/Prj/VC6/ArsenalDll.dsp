# Microsoft Developer Studio Project File - Name="Arsenal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ArsenalDll - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ArsenalDll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ArsenalDll.mak" CFG="ArsenalDll - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ArsenalDll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ArsenalDll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ArsenalDll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Binary\x86\DLL\Release"
# PROP Intermediate_Dir "..\..\Temp\x86\DLLRelease\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ARSENAL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ARSENAL_EXPORTS" /D "AR_EXPORTS" /D "AR_SHARED_LIB" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "ArsenalDll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Binary\x86\DLL\Debug\"
# PROP Intermediate_Dir "..\..\Temp\x86\DLLDebug\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ARSENAL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ARSENAL_EXPORTS" /D "AR_EXPORTS" /D "AR_SHARED_LIB" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ArsenalDll - Win32 Release"
# Name "ArsenalDll - Win32 Debug"
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Arsenal\Common\common.c
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Common\common.h
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Common\config.h
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Common\platform.h
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Common\spec.c
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Common\spec.h
# End Source File
# End Group
# Begin Group "Lex"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Arsenal\Lex\__inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Lex\dfa.c
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Lex\dfa.h
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Lex\lex.c
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Lex\lex.h
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Lex\node.c
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Lex\node.h
# End Source File
# End Group
# Begin Group "Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Arsenal\Test\lex_test.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Arsenal\Arsenal.c
# End Source File
# Begin Source File

SOURCE=..\..\Arsenal\Arsenal.h
# End Source File
# End Target
# End Project
