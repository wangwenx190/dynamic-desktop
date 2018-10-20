@echo off
if exist build rd /s /q build
md build
cd build
lupdate -no-obsolete "%~dp0..\src\core\core.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\core\core.pro"
cd ..
rd /s /q build
