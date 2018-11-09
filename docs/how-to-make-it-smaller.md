## When building Qt
1. Optimize for size instead of speed: **`-optimize-size`** or use **`-O1`** instead of **`-O2`**, **`-Ox`** or **`-O3`**
2. Disable 3rd-party libraries: **`-no-<library name>`**

   Qt 5.12 example:
   ```bat
   -no-zlib -no-libjpeg -no-freetype -no-harfbuzz
   ```
3. Skip unneeded repositories: **`-skip <qt repository name>`**

   Qt 5.12 example:
   ```bat
   -skip qt3d -skip qtactiveqt -skip qtandroidextras -skip qtcanvas3d -skip qtcharts -skip qtconnectivity -skip qtdatavis3d -skip qtdeclarative -skip qtdoc -skip qtgamepad -skip qtgraphicaleffects -skip qtimageformats -skip qtlocation -skip qtmacextras -skip qtmultimedia -skip qtnetworkauth -skip qtpurchasing -skip qtquickcontrols -skip qtquickcontrols2 -skip qtremoteobjects -skip qtscript -skip qtscxml -skip qtsensors -skip qtserialbus -skip qtserialport -skip qtspeech -skip qttools -skip qttranslations -skip qtvirtualkeyboard -skip qtwayland -skip qtwebchannel -skip qtwebengine -skip qtwebglplugin -skip qtwebsockets -skip qtwebview -skip qtx11extras -skip qtxmlpatterns
   ```
4. Disable unneeded features: **`-no-feature-<qt feature name>`**

   The easiest way is to use [**Qt Configuration Tool**](https://doc.qt.io/QtForDeviceCreation/qt-configuration-tool.html), but it's a **COMMERCIAL ONLY** tool which means you will have to buy a commercial license from [the Qt Company](https://www.qt.io/) if you want to use it.

   Qt 5.12 example:
   ```bat
   -no-feature-animation -no-feature-calendarwidget -no-feature-colordialog -no-feature-commandlinkbutton -no-feature-concurrent -no-feature-datetimeedit -no-feature-dial -no-feature-dockwidget -no-feature-fontcombobox -no-feature-fontdialog -no-feature-future -no-feature-gestures -no-feature-im -no-feature-keysequenceedit -no-feature-lcdnumber -no-feature-mdiarea -no-feature-pdf -no-feature-progressdialog -no-feature-sha3-fast -no-feature-splashscreen -no-feature-statusbar -no-feature-statustip -no-feature-syntaxhighlighter -no-feature-textodfwriter -no-feature-tuiotouch -no-feature-undocommand -no-feature-undogroup -no-feature-undostack -no-feature-undoview -no-feature-whatsthis -no-feature-wizard -no-feature-contextmenu -no-feature-effects -no-feature-errormessage -no-feature-mainwindow -no-feature-toolbar -no-feature-toolbox -no-feature-tooltip -no-feature-printdialog -no-feature-printer -no-feature-printpreviewdialog -no-feature-printpreviewwidget -no-feature-imageformatplugin -no-feature-cups -no-feature-iconv -no-feature-graphicseffect -no-feature-sharedmemory -no-feature-processenvironment -no-feature-process -no-feature-filesystemwatcher -no-feature-filesystemiterator -no-feature-movie -no-feature-imageformat_bmp -no-feature-imageformat_ppm -no-feature-imageformat_xbm -no-feature-multiprocess -no-feature-treewidget -no-feature-tablewidget -no-feature-imageformat_jpeg -no-feature-image_heuristic_mask -no-feature-image_text -no-feature-sqlmodel -no-feature-paint_debug -no-feature-tabbar -no-feature-tabwidget -no-feature-textbrowser -no-feature-sessionmanager -no-feature-draganddrop -no-feature-picture -no-feature-desktopservices -no-feature-stringlistmodel -no-feature-statemachine -no-feature-timezone -no-feature-datetimeparser -no-feature-columnview -no-feature-tableview -no-feature-datawidgetmapper -no-feature-completer -no-feature-fscompleter -no-feature-listwidget -no-feature-resizehandler -no-feature-rubberband -no-feature-sizegrip -no-feature-systemsemaphore
   ```
5. Static Qt: Disable Whole Program Optimization/Link Time Code Generation: Default. No actions needed.

   Dynamic Qt: Enable Whole Program Optimization/Link Time Code Generation: **`-ltcg`**
6. Link to MSVCRT dynamically: Default. No actions needed.
## When building FFmpeg
1. Optimize for size instead of speed: **`--enable-small`**
2. Disable unneeded features: See https://github.com/wang-bin/avbuild/blob/master/config-lite.sh for more information.
## When building this project
0. All configurations should be written to **`.qmake.conf`** in the root directory of this project.
1. Optimize for size instead of speed: **`CONFIG *= enable_small`**
2. Enable Whole Program Optimization/Link Time Code Generation: **`CONFIG *= enable_ltcg`**
## Finally
1. Use [UPX](https://upx.github.io/) to compress all binary files(exe and dll): **`--ultra-brute --force`** or **`--best --force`**. Not recommended, because it will make your applications startup much more slower.
