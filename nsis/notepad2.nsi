# notepad2.nsi
#
# Notepad2 Nullsoft Installer Script
# Author: Dan Savilonis <djs@n-cube.org>

!include WinVer.nsh

Name "Notepad2"
Caption "Notepad2 Setup"
Icon "..\res\Notepad2.ico"
UninstallIcon "..\res\Notepad2.ico"
OutFile "notepad2-install.exe"

XPStyle on
SetCompressor /SOLID /FINAL lzma

InstallDir "$PROGRAMFILES\Notepad2"
InstallDirRegKey HKLM "Software\Notepad2" "Install_Dir"

LicenseData "..\License.txt"

RequestExecutionLevel admin

;--------------------------------

Page license
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

!ifndef NOINSTTYPES
  InstType "Typical"
  InstType "Minimal"
!endif

;--------------------------------


Section "Notepad2"

SectionIn 1 2 RO

  SetOutPath $INSTDIR

  File ..\bin\notepad2.exe

  WriteRegStr HKLM SOFTWARE\Notepad2 "Install_Dir" "$INSTDIR"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Notepad2" "DisplayName" "Notepad2"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Notepad2" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Notepad2" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Notepad2" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  # Windows 7 Jumplists support
  ${If} ${AtLeastWin7}
    WriteRegStr HKCR "*\OpenWithList\notepad2.exe" "" ""
    WriteRegStr HKCR "Applications\notepad2.exe" "AppUserModelID" "Notepad2"
    WriteRegStr HKCR "Applications\notepad2.exe\shell\open\command" "" '"$INSTDIR\notepad2.exe" %1'
  ${EndIf}

SectionEnd

Section "Replace Notepad"

SectionIn 1

  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\notepad.exe" "Debugger" "$INSTDIR\notepad2.exe /z"

SectionEnd

Section "Start Menu"

SectionIn 1 2

  CreateDirectory "$SMPROGRAMS\Notepad2"
  CreateShortCut "$SMPROGRAMS\Notepad2\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Notepad2\Notepad2.lnk" "$INSTDIR\notepad2.exe" "" "$INSTDIR\notepad2.exe" 0

SectionEnd


;--------------------------------

; Uninstaller

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Notepad2"
  DeleteRegKey HKLM "SOFTWARE\Notepad2"

  DeleteRegKey HKCR "*\OpenWithList\notepad2.exe"
  DeleteRegKey HKCR "Applications\notepad2.exe"
  DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\notepad.exe"

  ; Remove files and uninstaller
  Delete $INSTDIR\notepad2.exe
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Notepad2\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Notepad2"
  RMDir "$INSTDIR"

SectionEnd
