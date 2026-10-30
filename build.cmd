@echo off
rem Builds keyswitcher.exe with MSVC, without the C runtime.
setlocal
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS=%%i"
if not defined VS (echo MSVC not found & exit /b 1)
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1

cd /d "%~dp0"
cl /nologo /O1 /Os /GS- /GR- /EHs-c- /Zl /utf-8 /W4 /DUNICODE /D_UNICODE keyswitcher.cpp ^
   /link /NODEFAULTLIB /ENTRY:Entry /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /MANIFEST:EMBED ^
   kernel32.lib user32.lib shell32.lib gdi32.lib advapi32.lib /OUT:keyswitcher.exe
set ERR=%ERRORLEVEL%
del /q keyswitcher.obj 2>nul
exit /b %ERR%
