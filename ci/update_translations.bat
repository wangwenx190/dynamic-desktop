@echo off
if exist build rd /s /q build
md build
cd build
lupdate -no-obsolete "%~dp0..\src\launcher\launcher.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\launcher\launcher.pro"
lupdate -no-obsolete "%~dp0..\src\player\player.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\player\player.pro"
lupdate -no-obsolete "%~dp0..\src\qsimpleupdater\qsimpleupdater.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\qsimpleupdater\qsimpleupdater.pro"
cd ..
rd /s /q build
