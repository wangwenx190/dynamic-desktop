@echo off
cd /d "%~dp0.."
if not exist build exit /b
cd build
where ISCC
if %ERRORLEVEL% neq 0 set "PATH=%ProgramFiles(x86)%\Inno Setup 5;%PATH%"
if not defined DDVersion set "DDVersion=1.0.0.0"
set "iss_path=%~dp0..\src\installer\installer.iss"
if exist bin (
    ISCC /Qp /DDDVersion=%DDVersion% /O"%CD%" /Fdd_v%DDVersion%_setup_x86 "%iss_path%"
)
if exist bin64 (
    ISCC /Qp /DWIN64 /DDDVersion=%DDVersion% /O"%CD%" /Fdd_v%DDVersion%_setup_x64 "%iss_path%"
)
