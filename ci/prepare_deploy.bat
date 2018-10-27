@echo off
cd /d "%~dp0..\build"
set "DD_ZIP_BASENAME=dd-git-%APPVEYOR_REPO_COMMIT:~0,7%"
if exist bin\ddmain.exe (
    move bin %DD_ZIP_BASENAME%-x86
    7z a %APPVEYOR_BUILD_FOLDER%\%DD_ZIP_BASENAME%-x86.7z %DD_ZIP_BASENAME%-x86 > NUL
)
if exist bin64\ddmain.exe (
    move bin64 %DD_ZIP_BASENAME%-x64
    7z a %APPVEYOR_BUILD_FOLDER%\%DD_ZIP_BASENAME%-x64.7z %DD_ZIP_BASENAME%-x64 > NUL
)
