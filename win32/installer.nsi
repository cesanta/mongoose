!define VERSION "2.9"
!define MENUDIR "Mongoose web server"
!define	SVC "Mongoose ${VERSION}"

OutFile mongoose-${VERSION}.install.exe
Name "Mongoose ${VERSION}"
InstallDir C:\mongoose-${VERSION}

Page components
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "Mongoose files (required)"
  SectionIn RO
  SetOutPath $INSTDIR
  File ..\mongoose.exe
  File ..\_mongoose.dll
  File ..\_mongoose.lib
  File mongoose.conf
  File README.txt
  File srvany.exe
  WriteUninstaller uninstall.exe
SectionEnd

Section "SSL files"
  SectionIn RO
  File ssl_cert.pem

  # Following lines add full path to the certificate file in the mongoose.conf
  FileOpen $0 mongoose.conf a
  FileRead $0 $1
  FileRead $0 $1
  FileRead $0 $1
  FileRead $0 $1
  FileRead $0 $1
  FileRead $0 $1
  FileWrite $0 "ssl_certificate $INSTDIR\ssl_cert.pem"
  FileClose $0
SectionEnd

Section "Run Mongoose as service"
  ExecWait 'sc create "${SVC}" binpath= $INSTDIR\srvany.exe start= auto depend= Tcpip'
  ExecWait 'sc description "${SVC}" "Web server"'
  WriteRegStr HKLM "System\CurrentControlSet\Services\${SVC}\Parameters" "Application" "$INSTDIR\mongoose.exe"
  WriteRegStr HKLM "System\CurrentControlSet\Services\${SVC}\Parameters" "AppDirectory" "$INSTDIR"
  ExecWait 'sc start "${SVC}"'
SectionEnd

Section "Create menu shortcuts"
  CreateDirectory "$SMPROGRAMS\${MENUDIR}"
  CreateShortCut "$SMPROGRAMS\${MENUDIR}\Start in console.lnk" "$INSTDIR\mongoose.exe"
  CreateShortCut "$SMPROGRAMS\${MENUDIR}\Edit config.lnk" "notepad" "$INSTDIR\mongoose.conf"
  CreateShortCut "$SMPROGRAMS\${MENUDIR}\Stop service.lnk" "sc" 'stop "Mongoose ${VERSION}"'
  CreateShortCut "$SMPROGRAMS\${MENUDIR}\Start service.lnk" "sc" 'start "Mongoose ${VERSION}"'
  CreateShortCut "$SMPROGRAMS\${MENUDIR}\uninstall.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

Section "Uninstall"
  ExecWait 'sc stop "${SVC}"'
  ExecWait 'sc delete "${SVC}"'
  Delete "$INSTDIR\*.*"
  Delete "$SMPROGRAMS\${MENUDIR}\*.*"
  RMDir "$SMPROGRAMS\${MENUDIR}"
  RMDir "$INSTDIR"
SectionEnd
