@ECHO OFF
COLOR
TITLE Building Qt from source code
CLS
SETLOCAL
CALL "%~dp0config.bat"
CALL "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" %TARGET_ARCH%
SET "PATH=%SRC_DIR%\qtbase\bin;%SRC_DIR%\gnuwin32\bin;%PATH%"
CD /D "%SRC_DIR%"
IF EXIST build RD /S /Q build
MD build
CD build
CALL "%SRC_DIR%\configure.bat" -opensource -confirm-license -platform win32-msvc -make-tool jom -silent -nomake examples -nomake tests -opengl dynamic -prefix "%INSTALL_DIR%" %EXTRA_CONFIG%
IF %ERRORLEVEL% NEQ 0 GOTO ErrHappen
jom && jom install
IF %ERRORLEVEL% NEQ 0 GOTO ErrHappen
IF EXIST "%ProgramFiles(x86)%\Windows Kits\10\Redist\D3D\%TARGET_ARCH%\d3dcompiler_47.dll" COPY /Y "%ProgramFiles(x86)%\Windows Kits\10\Redist\D3D\%TARGET_ARCH%\d3dcompiler_47.dll" "%INSTALL_DIR%\bin\d3dcompiler_47.dll"
> "%INSTALL_DIR%\bin\qt.conf" (
    @ECHO [Paths]
    @ECHO Prefix=..
    @ECHO Documentation=../../Docs/Qt-%TARGET_VER%
    @ECHO Examples=../../Examples/Qt-%TARGET_VER%
)
> "%INSTALL_DIR%\bin\qtenv2.bat" (
    @ECHO @echo off
    @ECHO echo Setting up environment for Qt usage...
    @ECHO set PATH=%%~dp0;%%PATH%%
    @ECHO cd /D %%~dp0..
    @ECHO echo Remember to call vcvarsall.bat to complete environment setup!
)
CD /D "%SRC_DIR%"
RD /S /Q build
TITLE Compiling process finished
CLS
ECHO Compiling process have finished successfully
ECHO All binaries have been installed to: %INSTALL_DIR%
GOTO Fin
:ErrHappen
TITLE Compiling process aborted
ECHO ============================================================
ECHO Something wrong happened during the compiling process
ECHO and the process have aborted because of this
ECHO Please check the log file for more information
ECHO Press any key to exit this program
GOTO Fin
:Fin
ENDLOCAL
IF %ERRORLEVEL% NEQ 0 (
PAUSE
EXIT /B
)
