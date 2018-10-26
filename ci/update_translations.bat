@echo off
if exist build rd /s /q build
md build
cd build
lupdate -no-obsolete -locations none -no-ui-lines "%~dp0..\src\launcher\launcher.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\launcher\launcher.pro"
lupdate -no-obsolete -locations none -no-ui-lines "%~dp0..\src\player\player.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\player\player.pro"
cd ..
rd /s /q build
