@echo off
setlocal
cd /d "%~dp0"

where cl >nul 2>nul && goto :build
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (echo vswhere.exe not found - install Visual Studio with the C++ workload & exit /b 1)
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS=%%i"
if not defined VS (echo MSVC not found - install the "Desktop development with C++" workload & exit /b 1)
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul || exit /b 1

:build
if not exist build mkdir build
cl /nologo /O1 /Os /GS- /GR- /EHs-c- /Zl /utf-8 /W4 /WX /DUNICODE /D_UNICODE ^
   /Fobuild\ src\keyswitcher.cpp ^
   /link /NODEFAULTLIB /ENTRY:Entry /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /MANIFEST:EMBED ^
   kernel32.lib user32.lib shell32.lib gdi32.lib advapi32.lib /OUT:build\keyswitcher.exe
exit /b %ERRORLEVEL%
