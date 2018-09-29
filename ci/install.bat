if exist depends rd /s /q depends
appveyor DownloadFile "https://sourceforge.net/projects/qtav/files/depends/QtAV-depends-windows-x86+x64.7z/download" -FileName qtavdepends.7z
7z x qtavdepends.7z
del /f qtavdepends.7z
move QtAV-depends* depends
xcopy /s /y /i depends\include %QTDIR%\include
xcopy /s /y /i depends\lib\x64 %QTDIR%\lib
rd /s /q depends
if exist av rd /s /q av
md av
cd av
appveyor DownloadFile "https://sourceforge.net/projects/qtav/files/ci/QtAV-Qt5.9-VS2017x64-release-%QTAVID%.7z/download" -FileName qtavci.7z
7z x qtavci.7z
xcopy /s /y /i include %QTDIR%\include
copy /y lib\*Qt*AV*.lib %QTDIR%\lib
copy /y lib\QtAV1.lib %QTDIR%\lib\Qt5AV.lib
copy /y lib\QtAVWidgets1.lib %QTDIR%\lib\Qt5AVWidgets.lib
xcopy /s /y /i bin\QtAV %QTDIR%\qml\QtAV
xcopy /s /y /i mkspecs %QTDIR%\mkspecs
cd ..
rd /s /q av
