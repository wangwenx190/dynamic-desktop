:: Usage: call build.bat "mkspec" "config"
:: Eg: call build.bat "win32-icc" "release static_dd small ltcg"
:: IMPORTANT: Double quotation marks are indispensable and
:: remember to call "vcvarsall.bat" and add Qt's directories
:: to your environment variables!
@echo off
cd /d "%~dp0"
if exist build rd /s /q build
md build
cd build
md tmp
cd tmp
set _mkspec=
set _config=
if defined CI (
    set _mkspec=win32-msvc
    set _config=release
    goto start_build
)
set _mkspec=%1
if defined _mkspec (
    set _mkspec=%_mkspec:~1,-1%
) else (
    set _mkspec=win32-msvc
)
set _config=%2
if defined _config (
    set _config=%_config:~1,-1%
) else (
    set _config=release
)
goto start_build
:start_build
qmake "%~dp0dynamic-desktop.pro" -spec %_mkspec% "CONFIG+=%_config%"
set _buildtool=jom
where %_buildtool%
if %ERRORLEVEL% neq 0 set _buildtool=nmake
%_buildtool% qmake_all
%_buildtool% && %_buildtool% install
cd "%~dp0"
rd /s /q build\tmp
