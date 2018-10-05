@echo off
if exist qtavdepends.7z del /f qtavdepends.7z
appveyor DownloadFile "https://sourceforge.net/projects/qtav/files/depends/QtAV-depends-windows-x86+x64.7z/download" -FileName qtavdepends.7z
if exist depends rd /s /q depends
7z x qtavdepends.7z > NUL
del /f qtavdepends.7z
move QtAV-depends* depends
xcopy /s /y /i depends\include %QTDIR%\include > NUL
xcopy /s /y /i depends\lib\x64 %QTDIR%\lib > NUL
rd /s /q depends
if exist av rd /s /q av
md av
cd av
appveyor DownloadFile "https://sourceforge.net/projects/qtav/files/ci/QtAV-Qt%QTVER%-VS2017x64-release-%QTAVID%.7z/download" -FileName qtavci.7z
7z x qtavci.7z > NUL
xcopy /s /y /i include %QTDIR%\include > NUL
copy /y lib\*Qt*AV*.lib %QTDIR%\lib > NUL
copy /y lib\QtAV1.lib %QTDIR%\lib\Qt5AV.lib > NUL
copy /y lib\QtAVWidgets1.lib %QTDIR%\lib\Qt5AVWidgets.lib > NUL
xcopy /s /y /i bin\QtAV %QTDIR%\qml\QtAV > NUL
xcopy /s /y /i mkspecs %QTDIR%\mkspecs > NUL
cd ..
rd /s /q av
