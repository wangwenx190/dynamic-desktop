## When building Qt
1. Optimize for size instead of speed

   **`-optimize-size`** or use **`-O1`** instead of **`-O2`**, **`-Ox`** or **`-O3`**

2. Skip unneeded repositories

   **`-skip <repo name>`**

   Qt 5.12 example:
   ```bat
   -skip qt3d -skip qtactiveqt -skip qtandroidextras -skip qtcanvas3d -skip qtcharts -skip qtconnectivity -skip qtdatavis3d -skip qtdeclarative -skip qtdoc -skip qtgamepad -skip qtgraphicaleffects -skip qtlocation -skip qtmacextras -skip qtmultimedia -skip qtnetworkauth -skip qtpurchasing -skip qtquickcontrols -skip qtquickcontrols2 -skip qtremoteobjects -skip qtscript -skip qtscxml -skip qtsensors -skip qtserialbus -skip qtserialport -skip qtspeech -skip qttools -skip qttranslations -skip qtvirtualkeyboard -skip qtwayland -skip qtwebchannel -skip qtwebengine -skip qtwebglplugin -skip qtwebsockets -skip qtwebview -skip qtx11extras -skip qtxmlpatterns
   ```

3. Disable unneeded features

   **`-no-feature-<feature name>`**

   Qt 5.12 example(not working currently):
   ```bat
   -no-feature-action -no-feature-animation -no-feature-bearermanagement -no-feature-buttongroup -no-feature-calendarwidget -no-feature-clipboard -no-feature-colordialog -no-feature-columnview -no-feature-commandlinkbutton -no-feature-completer -no-feature-concurrent -no-feature-contextmenu -no-feature-cups -no-feature-cursor -no-feature-datawidgetmapper -no-feature-datetimeedit -no-feature-desktopservices -no-feature-dial -no-feature-dialogbuttonbox -no-feature-dirmodel -no-feature-dnslookup -no-feature-dockwidget -no-feature-dom -no-feature-draganddrop -no-feature-dtls -no-feature-effects -no-feature-errormessage -no-feature-filesystemiterator -no-feature-filesystemmodel -no-feature-filesystemwatcher -no-feature-fontcombobox -no-feature-fontdialog -no-feature-formlayout -no-feature-fscompleter -no-feature-ftp -no-feature-future -no-feature-gestures -no-feature-graphicseffect -no-feature-graphicsview -no-feature-http -no-feature-iconv -no-feature-identityproxymodel -no-feature-im -no-feature-image_heuristic_mask -no-feature-image_text -no-feature-imageformat_ppm -no-feature-imageformat_xbm -no-feature-imageformat_xpm -no-feature-imageformatplugin -no-feature-itemmodel -no-feature-itemmodeltester -no-feature-itemviews -no-feature-keysequenceedit -no-feature-lcdnumber -no-feature-library -no-feature-listview -no-feature-listwidget -no-feature-localserver -no-feature-mainwindow -no-feature-mdiarea -no-feature-menu -no-feature-menubar -no-feature-movie -no-feature-multiprocess -no-feature-networkdiskcache -no-feature-networkinterface -no-feature-networkproxy -no-feature-paint_debug -no-feature-pdf -no-feature-printer -no-feature-printdialog -no-feature-printpreviewwidget -no-feature-process -no-feature-processenvironment -no-feature-progressbar -no-feature-progressdialog -no-feature-proxymodel -no-feature-scrollarea -no-feature-scrollbar -no-feature-scroller -no-feature-sessionmanager -no-feature-sha3-fast -no-feature-sharedmemory -no-feature-shortcut -no-feature-socks5 -no-feature-sortfilterproxymodel -no-feature-splitter -no-feature-sqlmodel -no-feature-stackedwidget -no-feature-standarditemmodel -no-feature-statemachine -no-feature-statusbar -no-feature-statustip -no-feature-stringlistmodel -no-feature-syntaxhighlighter -no-feature-systemsemaphore -no-feature-tabbar -no-feature-tabletevent -no-feature-tableview -no-feature-tablewidget -no-feature-tabwidget -no-feature-temporaryfile -no-feature-textbrowser -no-feature-textedit -no-feature-texthtmlparser -no-feature-textodfwriter -no-feature-timezone -no-feature-toolbar -no-feature-toolbox -no-feature-tooltip -no-feature-topleveldomain -no-feature-treeview -no-feature-treewidget -no-feature-tuiotouch -no-feature-udpsocket -no-feature-undocommand -no-feature-undogroup -no-feature-undostack -no-feature-undoview -no-feature-validator -no-feature-whatsthis -no-feature-widgettextcontrol -no-feature-wizard -no-feature-xmlstream -no-feature-xmlstreamreader -no-feature-xmlstreamwriter
   ```

4. Disable Whole Program Optimization/Link Time Code Generation

   Default. No actions needed.

## When building FFmpeg
1. Optimize for size instead of speed

   **`--enable-small`**

2. Disable unneeded features

   See https://github.com/wang-bin/avbuild/blob/master/config-lite.sh for more information.

## When building this project
1. Optimize for size instead of speed

   **`CONFIG *= enable_small`**

2. Enable Whole Program Optimization/Link Time Code Generation

   **`CONFIG *= enable_ltcg`**
