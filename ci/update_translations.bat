@echo off
if exist build rd /s /q build
md build
cd build
lupdate -no-obsolete -locations none -no-ui-lines "%~dp0..\src\controller\controller.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\controller\controller.pro"
lupdate -no-obsolete -locations none -no-ui-lines "%~dp0..\src\player\player.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\player\player.pro"
lupdate -no-obsolete -locations none -no-ui-lines "%~dp0..\src\tray\tray.pro"
lrelease -compress -nounfinished -removeidentical "%~dp0..\src\tray\tray.pro"
cd ..
rd /s /q build
