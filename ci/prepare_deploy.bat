@echo off
cd /d "%~dp0..\build"
set "DD_ZIP_BASENAME=dd-%APPVEYOR_REPO_COMMIT:~0,7%"
if exist bin\launcher.exe (
    move bin %DD_ZIP_BASENAME%-x86
    7z a %DD_ZIP_BASENAME%-x86 > NUL
)
if exist bin64\launcher.exe (
    move bin64 %DD_ZIP_BASENAME%-x64
    7z a %DD_ZIP_BASENAME%-x64 > NUL
)
