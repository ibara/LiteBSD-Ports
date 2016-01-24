;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

SetCompressor /SOLID lzma

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Links WWW Browser"
  ;Icon "links.ico"
  ;!define MUI_ICON "links.ico"
  OutFile "Links-64bit-install.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\Links"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Links" ""

  RequestExecutionLevel admin

  Var MUI_TEMP
  Var STARTMENU_FOLDER

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "COPYING"
;  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Links" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Links"
  
!insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER

  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "-Default Links browser files" DefaultSection

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...

File BRAILLE_HOWTO
File COPYING
File KEYS
File README
File links.crt
File links.exe
File links-g.exe
File c:\cygwin64\bin\cygEGL-1.dll
File c:\cygwin64\bin\cygGL-1.dll
File c:\cygwin64\bin\cygX11-6.dll
File c:\cygwin64\bin\cygX11-xcb-1.dll
File c:\cygwin64\bin\cygXau-6.dll
File c:\cygwin64\bin\cygXdmcp-6.dll
File c:\cygwin64\bin\cygXext-6.dll
File c:\cygwin64\bin\cygXrender-1.dll
File c:\cygwin64\bin\cygbz2-1.dll
File c:\cygwin64\bin\cygcairo-2.dll
File c:\cygwin64\bin\cygcroco-0.6-3.dll
File c:\cygwin64\bin\cygcrypto-1.0.0.dll
File c:\cygwin64\bin\cygevent-2-0-5.dll
File c:\cygwin64\bin\cygexpat-1.dll
File c:\cygwin64\bin\cygffi-6.dll
File c:\cygwin64\bin\cygfontconfig-1.dll
File c:\cygwin64\bin\cygfreetype-6.dll
File c:\cygwin64\bin\cyggcc_s-seh-1.dll
File c:\cygwin64\bin\cyggdk_pixbuf-2.0-0.dll
File c:\cygwin64\bin\cyggio-2.0-0.dll
File c:\cygwin64\bin\cygglapi-0.dll
File c:\cygwin64\bin\cygglib-2.0-0.dll
File c:\cygwin64\bin\cyggmodule-2.0-0.dll
File c:\cygwin64\bin\cyggobject-2.0-0.dll
File c:\cygwin64\bin\cyggomp-1.dll
File c:\cygwin64\bin\cyggraphite2-3.dll
File c:\cygwin64\bin\cygharfbuzz-0.dll
File c:\cygwin64\bin\cygiconv-2.dll
File c:\cygwin64\bin\cygintl-8.dll
File c:\cygwin64\bin\cygjbig-2.dll
File c:\cygwin64\bin\cygjpeg-8.dll
File c:\cygwin64\bin\cyglzma-5.dll
File c:\cygwin64\bin\cygpango-1.0-0.dll
File c:\cygwin64\bin\cygpangocairo-1.0-0.dll
File c:\cygwin64\bin\cygpangoft2-1.0-0.dll
File c:\cygwin64\bin\cygpcre-1.dll
File c:\cygwin64\bin\cygpixman-1-0.dll
File c:\cygwin64\bin\cygpng16-16.dll
File c:\cygwin64\bin\cygrsvg-2-2.dll
File c:\cygwin64\bin\cygssl-1.0.0.dll
File c:\cygwin64\bin\cygstdc++-6.dll
File c:\cygwin64\bin\cygtiff-6.dll
File c:\cygwin64\bin\cygwin1.dll
File c:\cygwin64\bin\cygxcb-1.dll
File c:\cygwin64\bin\cygxcb-glx-0.dll
File c:\cygwin64\bin\cygxcb-render-0.dll
File c:\cygwin64\bin\cygxcb-shm-0.dll
File c:\cygwin64\bin\cygxml2-2.dll
File c:\cygwin64\bin\cygz.dll
  
  ;Store installation folder
  WriteRegStr HKCU "Software\Links" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

!insertmacro MUI_STARTMENU_WRITE_BEGIN Application

CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Links.lnk" "$INSTDIR\Links.exe"
CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Links Graphics.lnk" "$INSTDIR\Links-g.exe" "-g"
CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
!insertmacro MUI_STARTMENU_WRITE_END
SectionEnd


;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
Delete "$INSTDIR\BRAILLE_HOWTO"
Delete "$INSTDIR\COPYING"
Delete "$INSTDIR\KEYS"
Delete "$INSTDIR\README"
Delete "$INSTDIR\links.crt"
Delete "$INSTDIR\links.exe"
Delete "$INSTDIR\links-g.exe"
Delete "$INSTDIR\cygEGL-1.dll"
Delete "$INSTDIR\cygGL-1.dll"
Delete "$INSTDIR\cygX11-6.dll"
Delete "$INSTDIR\cygX11-xcb-1.dll"
Delete "$INSTDIR\cygXau-6.dll"
Delete "$INSTDIR\cygXdmcp-6.dll"
Delete "$INSTDIR\cygXext-6.dll"
Delete "$INSTDIR\cygXrender-1.dll"
Delete "$INSTDIR\cygbz2-1.dll"
Delete "$INSTDIR\cygcairo-2.dll"
Delete "$INSTDIR\cygcroco-0.6-3.dll"
Delete "$INSTDIR\cygcrypto-1.0.0.dll"
Delete "$INSTDIR\cygevent-2-0-5.dll"
Delete "$INSTDIR\cygexpat-1.dll"
Delete "$INSTDIR\cygffi-6.dll"
Delete "$INSTDIR\cygfontconfig-1.dll"
Delete "$INSTDIR\cygfreetype-6.dll"
Delete "$INSTDIR\cyggcc_s-seh-1.dll"
Delete "$INSTDIR\cyggdk_pixbuf-2.0-0.dll"
Delete "$INSTDIR\cyggio-2.0-0.dll"
Delete "$INSTDIR\cygglapi-0.dll"
Delete "$INSTDIR\cygglib-2.0-0.dll"
Delete "$INSTDIR\cyggmodule-2.0-0.dll"
Delete "$INSTDIR\cyggobject-2.0-0.dll"
Delete "$INSTDIR\cyggomp-1.dll"
Delete "$INSTDIR\cyggraphite2-3.dll"
Delete "$INSTDIR\cygharfbuzz-0.dll"
Delete "$INSTDIR\cygiconv-2.dll"
Delete "$INSTDIR\cygintl-8.dll"
Delete "$INSTDIR\cygjbig-2.dll"
Delete "$INSTDIR\cygjpeg-8.dll"
Delete "$INSTDIR\cyglzma-5.dll"
Delete "$INSTDIR\cygpango-1.0-0.dll"
Delete "$INSTDIR\cygpangocairo-1.0-0.dll"
Delete "$INSTDIR\cygpangoft2-1.0-0.dll"
Delete "$INSTDIR\cygpcre-1.dll"
Delete "$INSTDIR\cygpixman-1-0.dll"
Delete "$INSTDIR\cygpng16-16.dll"
Delete "$INSTDIR\cygrsvg-2-2.dll"
Delete "$INSTDIR\cygssl-1.0.0.dll"
Delete "$INSTDIR\cygstdc++-6.dll"
Delete "$INSTDIR\cygtiff-6.dll"
Delete "$INSTDIR\cygwin1.dll"
Delete "$INSTDIR\cygxcb-1.dll"
Delete "$INSTDIR\cygxcb-glx-0.dll"
Delete "$INSTDIR\cygxcb-render-0.dll"
Delete "$INSTDIR\cygxcb-shm-0.dll"
Delete "$INSTDIR\cygxml2-2.dll"
Delete "$INSTDIR\cygz.dll"
Delete "$INSTDIR\.links\*"
RMDir "$INSTDIR\.links"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

!insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP

Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"
Delete "$SMPROGRAMS\$MUI_TEMP\Links.lnk"
Delete "$SMPROGRAMS\$MUI_TEMP\Links Graphics.lnk"
StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"

startMenuDeleteLoop:
ClearErrors
RMDir $MUI_TEMP
GetFullPathName $MUI_TEMP "$MUI_TEMP\.."

IfErrors startMenuDeleteLoopDone

StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
startMenuDeleteLoopDone:

  DeleteRegKey /ifempty HKCU "Software\Links"

SectionEnd
