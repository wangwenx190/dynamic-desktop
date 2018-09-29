cd /d "%~dp0"
if exist build rd /s /q build
md build
cd build
set _mkspec=%1
if /i "%_mkspec%" == "" set _mkspec=win32-msvc
set _config=%2
if /i "%_config%" == "" set _config=release
qmake "%~dp0dynamic-desktop.pro" -spec %_mkspec% "CONFIG+=%_config%"
set _buildtool=jom
where %_buildtool%
if %ERRORLEVEL% neq 0 set _buildtool=nmake
%_buildtool% qmake_all
%_buildtool% && %_buildtool% install
cd "%~dp0"
rd /s /q build
