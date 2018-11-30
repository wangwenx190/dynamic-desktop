:: Usage: call build.bat "mkspec" "config"
:: Eg: call build.bat "win32-icc" "release static_ffmpeg enable_libass"
:: IMPORTANT: Double quotation marks are indispensable and
:: remember to call "vcvarsall.bat" and add Qt's directories
:: to your environment variables!
@echo off
color
setlocal
cls
cd /d "%~dp0"
if exist build rd /s /q build
md build
cd build
md tmp
cd tmp
set _mkspec=
set _config=
if defined CI (
    set "_mkspec=win32-msvc"
    set "_config=release"
    goto start_build
)
set "_target_arch=%3"
if not defined _target_arch set "_target_arch=x64"
set "_qt_bin_dir=msvc2017"
if /i "%_target_arch%" == "x64" set "_qt_bin_dir=%_qt_bin_dir%_64"
set "_qt_ver=%4"
if not defined _qt_ver set "_qt_ver=5.12.0"
set "_qt_dir=%5"
if not defined _qt_dir set "_qt_dir=C:\Qt\Qt%_qt_ver%\%_qt_ver%\%_qt_bin_dir%"
set "_qt_bin_dir=%_qt_dir%\bin"
if not exist "%_qt_bin_dir%" echo Cannot find Qt's bin directory, if you didn't install Qt in it's default location, please pass a parameter && goto build_finish
set "PATH=%_qt_dir%;%_qt_bin_dir%;%PATH%"
set _vs_dev_cmd_path=
set _vs_2017_path=
for /f "delims=" %%A in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -property installationPath -latest -requires Microsoft.Component.MSBuild Microsoft.VisualStudio.Component.VC.Tools.x86.x64') do set "_vs_2017_path=%%A"
set "_vs_dev_cmd_path=%_vs_2017_path%\Common7\Tools\VsDevCmd.bat"
if not exist "%_vs_dev_cmd_path%" echo Cannot find [VsDevCmd.bat], if you did't install VS2017 in it's default location, please change this script && goto build_finish
set "_vs_arch=%_target_arch%"
if /i "%_target_arch%" == "x64" set "_vs_arch=amd64"
CALL "%_vs_dev_cmd_path%" -no_logo -arch=%_vs_arch%
set "_mkspec=%1"
if defined _mkspec (
    set "_mkspec=%_mkspec:~1,-1%"
) else (
    set "_mkspec=win32-msvc"
)
set "_config=%2"
if defined _config (
    set "_config=%_config:~1,-1%"
) else (
    set "_config=release silent"
)
goto start_build
:start_build
qmake "%~dp0dynamic-desktop.pro" -spec %_mkspec% "CONFIG+=%_config%"
set "_buildtool=jom"
where %_buildtool%
if %ERRORLEVEL% neq 0 set "_buildtool=nmake"
%_buildtool% qmake_all
%_buildtool%
%_buildtool% install
cd "%~dp0"
rd /s /q build\tmp
if exist build\lib rd /s /q build\lib
if exist build\lib64 rd /s /q build\lib64
goto build_finish
:build_finish
endlocal
if not defined CI pause
exit /b
