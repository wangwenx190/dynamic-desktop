if exist "%~dp0build" rd /s /q "%~dp0build"
lupdate -no-obsolete dd.pro
lrelease -idbased -compress -nounfinished -removeidentical dd.pro
qmake dd.pro -spec win32-msvc CONFIG+=release
jom qmake_all
jom && jom install
if exist "%~dp0build\bin" (
    cd /d "%~dp0build\bin"
    if exist plugins rd /s /q plugins
    windeployqt --plugindir "%~dp0build\bin\plugins" --release --force --no-translations --compiler-runtime --angle --no-opengl-sw dd.exe
)
if exist "%~dp0build\bin64" (
    cd /d "%~dp0build\bin64"
    if exist plugins rd /s /q plugins
    windeployqt --plugindir "%~dp0build\bin64\plugins" --release --force --no-translations --compiler-runtime --angle --no-opengl-sw dd64.exe
)
cd /d "%~dp0"
