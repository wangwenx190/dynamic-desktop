## When building Qt
1. Optimize for size instead of speed

   **`-optimize-size`** or use **`-O1`** instead of **`-O2`**, **`-Ox`** or **`-O3`**

2. Skip unneeded repositories

   **`-skip <qt repository name>`**

   Qt 5.12 example:
   ```bat
   -skip qt3d -skip qtactiveqt -skip qtandroidextras -skip qtcanvas3d -skip qtcharts -skip qtconnectivity -skip qtdatavis3d -skip qtdeclarative -skip qtdoc -skip qtgamepad -skip qtgraphicaleffects -skip qtimageformats -skip qtlocation -skip qtmacextras -skip qtmultimedia -skip qtnetworkauth -skip qtpurchasing -skip qtquickcontrols -skip qtquickcontrols2 -skip qtremoteobjects -skip qtscript -skip qtscxml -skip qtsensors -skip qtserialbus -skip qtserialport -skip qtspeech -skip qttools -skip qttranslations -skip qtvirtualkeyboard -skip qtwayland -skip qtwebchannel -skip qtwebengine -skip qtwebglplugin -skip qtwebsockets -skip qtwebview -skip qtx11extras -skip qtxmlpatterns
   ```

3. Disable unneeded features

   **`-no-feature-<qt feature name>`**

   The easiest way is to use [**Qt Configuration Tool**](https://doc.qt.io/QtForDeviceCreation/qt-configuration-tool.html), but it's a **COMMERCIAL ONLY** tool which means you will have to buy a commercial license from [the Qt Company](https://www.qt.io/) if you want to use it.

4. Disable Whole Program Optimization/Link Time Code Generation

   Default. No actions needed.

5. Link to MSVCRT dynamically

   Default. No actions needed.

## When building FFmpeg
1. Optimize for size instead of speed

   **`--enable-small`**

2. Disable unneeded features

   See https://github.com/wang-bin/avbuild/blob/master/config-lite.sh for more information.

## When building this project
0. All configurations should be written to **`.qmake.conf`** in the root directory of this project.

1. Optimize for size instead of speed

   **`CONFIG *= enable_small`**

2. Enable Whole Program Optimization/Link Time Code Generation

   **`CONFIG *= enable_ltcg`**

## Finally
1. Use [UPX](https://upx.github.io/) to compress all binary files(exe and dll)

   **`--ultra-brute --force`** or **`--best --force`**. Not recommended, it will make your applications startup much more slower.
