!include MUI.nsh

!define VERSION "alpha"
Name "Asteroids"
OutFile "asteroids-demo-${VERSION}.exe"
InstallDir "$PROGRAMFILES\Asteroids"
LicenseData "license.txt"
ShowInstDetails show

Var DirectXSetupError
Var VisualStudioSetupError

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE license.txt
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_LANGUAGE "English"

UninstPage uninstConfirm
UninstPage instfiles

Icon "${NSISDIR}\contrib\graphics\icons\orange-install.ico"
UninstallIcon "${NSISDIR}\contrib\graphics\icons\orange-uninstall.ico"

Section "Main"
  SetOutPath $INSTDIR
  File /r /x *.lib /x *.exp /x *.ilk /x *.pdb /x *resource* ..\Binary*
  File /r /x .hg* ..\Content*
  File /r /x .hg* ..\Lib*

  WriteUninstaller "$INSTDIR\Uninstall.exe"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Asteroids" "DisplayName" "Asteroids Demo"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Asteroids" "Publisher" "Matt Fichman <mfichman@stanford.edu>"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Asteroids" "URLInfoAbout" "http://stanford.edu/~mfichman/asteroids-demo.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Asteroids" "URLUpdateInfo" "http://stanford.edu/~mfichman/asteroids-demo.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Asteroids" "UninstallString" "$INSTDIR\Uninstall.exe"

  CreateShortCut "$DESKTOP\Asteroids Demo.lnk" "$INSTDIR\Binary\game.exe" ""

SectionEnd

Section "DirectX Install" SEC_DIRECTX
 SectionIn RO
 SetOutPath "$TEMP"
 File "dxwebsetup.exe"
 DetailPrint "Running DirectX Setup..."
 ExecWait '"$TEMP\dxwebsetup.exe"' $DirectXSetupError
 DetailPrint "Finished DirectX Setup"
 Delete "$TEMP\dxwebsetup.exe"
 SetOutPath "$INSTDIR"
SectionEnd

Section "Visual Studio 2008 Runtime Install" SEC_VS2008
 SectionIn RO
 SetOutPath "$TEMP"
 File "vcredist_x86.exe"
 DetailPrint "Running Visual Studio 2008 Runtime Setup..."
 ExecWait '"$TEMP\vcredist_x86.exe /Q"' $VisualStudioSetupError
 DetailPrint "Finished Visual Studio 2008 Runtime Setup"
 Delete "$TEMP\vcredist_x86.exe"
 SetOutPath "$INSTDIR"
SectionEnd

Section "un.Main"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Asteroids"
  RMDir /r "$INSTDIR"
SectionEnd

