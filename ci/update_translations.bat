@echo off
if exist build rd /s /q build
md build
cd build
lupdate -no-obsolete -locations none -no-ui-lines "%~dp0..\src\ddmain\ddmain.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\ddmain\ddmain.pro"
cd ..
rd /s /q build
