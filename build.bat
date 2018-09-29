cd /d "%~dp0"
if exist build rd /s /q build
md build
cd build
qmake "%~dp0dynamic-desktop.pro" -spec win32-msvc "CONFIG+=release"
nmake qmake_all
nmake && nmake install
cd "%~dp0"
rd /s /q build
