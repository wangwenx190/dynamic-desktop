[![Build status](https://ci.appveyor.com/api/projects/status/7n23thxiormp6oar/branch/develop?svg=true)](https://ci.appveyor.com/project/wangwenx190/dynamic-desktop/branch/develop)
[![Code quality](https://api.codacy.com/project/badge/Grade/b41d1d384abe4f81a9613945cf654ff6)](https://www.codacy.com/app/wangwenx190/dynamic-desktop?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=wangwenx190/dynamic-desktop&amp;utm_campaign=Badge_Grade)
[![License](https://img.shields.io/badge/license-GPLv3-red.svg)](/LICENSE.md)
[![Bugs](https://img.shields.io/github/issues/wangwenx190/dynamic-desktop/bug.svg)](https://github.com/wangwenx190/dynamic-desktop/issues?utf8=✓&q=is%3Aissue+is%3Aopen+label%3Abug)
[![Latest release](https://img.shields.io/sourceforge/dt/dynamic-desktop.svg)](https://sourceforge.net/projects/dynamic-desktop/files/latest/download)

[![SourceForge](https://sourceforge.net/sflogo.php?type=16&group_id=3035605)](https://sourceforge.net/p/dynamic-desktop/)
[![Download latest release](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/dynamic-desktop/files/latest/download)

## Overview
A small tool that can play videos on your desktop. Only for Microsoft Windows 7 ~ 10(with GUI desktop). It may have some limitations because it's just a media player, putting it's window under your desktop icons. Maybe Wallpaper Engine's principle is totally different with this tool.

I created this tool only to play my favorite animation clips on my desktop, so I will never make it just like Wallpaper Engine. This tool will stay simple and will only play videos. It doesn't have as many functions as Wallpaper Engine has, but that's indeed what I want. Although this tool has the ability to show executables and web pages on your desktop, I don't want to add this kind of functions. However, if you really need it, you can fork this repository and do it yourself.

**Suggestions and pull requests are both welcome.**

**My native language is Chinese and I'm not good at English. I use Google Translate to translate Chinese into English. I'm not sure about the accuracy of the translated texts. So if there is anything wrong in English translation, please inform me using [GitHub Issues](https://github.com/wangwenx190/dynamic-desktop/issues). Thanks a lot.**

## Features
- Relatively small
- Multilingual support(**translators needed**)
- Supports almost all kinds of media files
- Supports play online media streams
- **Hardware Accelerated Decoding** support: **CUDA**, **D3D11** and **DXVA**
- Low CPU and memory usage
- High performance
- High video quality
- HiDPI support
- Multiscreen support
- Support automatically run when the system starts
- Support auto update
- Various beautiful skins
- Abundant options
- Pure green: no need to install, no write to registry and no files left in your system
- No ads, free of charge, open source, etc...

## Supported platforms
Microsoft Windows 7 ~ 10(with GUI desktop), both 32 bit and 64 bit systems.

## Downloads
For official builds and third-party packages please see https://sourceforge.net/p/dynamic-desktop/ .

## Important notes
- Windows Vista and XP are not supported.
- This tool won't run if Windows Aero is disabled.
- Win7 users only: make sure you have installed XAudio2(Win 8/8.1/10 have installed it already by default).
- To experience *Hardware Accelerated Decoding* better, make sure this tool runs with your high performance GPU(on Optimus).
- The CPU and memory usage will increase enormously if you are using software decoding and that's normal, all media players will, don't worry.
- It would be very nice if you update your graphics card driver to the latest version.
- It's not recommended to run this tool in virtual machines, because the graphics card drivers of virtual machines are usually out-dated and even not completed.
- If there is no picture but sound, please switch the hardware decoding algorithm or just use software decoding. Because hardware decoding has many limitations while software decoding has not. In one word, **IF YOU ENCOUNTER WITH ANY STRANGE PHENOMENA, TURN OFF HARDWARE DECODING**.
- It's not recommended to change rendering engine again and again while it is playing videos, otherwise this tool is very likely to crash and I'm not able to help you in this kind of situation.
- Currently, hardware decoding only supports traditional video formats such as AVC/H.264, so please do not attempt to play HEVC videos if you are using hardware decoding.

## Compilation
- Clone or download this repository.
- Download [Qt5](http://download.qt.io/archive/qt/) and install it.
- Download [QtAV](https://github.com/wang-bin/QtAV)'s prebuilt SDK and install it, or build yourself.
- Download [FFmpeg](https://ffmpeg.zeranoe.com/builds/)'s prebuilt binaries or build yourself.
- Open "dynamic-desktop.pro" and start compiling or call "build.bat". But remember to call "vcvarsall.bat" and add Qt's directories to your path variables before calling "build.bat".

### IMPORTANT NOTES
- You can also use Intel C++ Compiler(ICC), Clang or MinGW to compile it, just remember to set up environment variables. Cross compile on other platforms may also work, but it will never run on platforms other than MS Windows.
- You will need to build the static version of Qt by yourself if you want to get rid of the Qt dlls. Currently, this project only needs 3 repositories: **`qtbase`**(Qt modules needed: core, gui, widgets, network and opengl), **`qtsvg`**(SVG support) and **`qtwinextras`**(Taskbar Progress support). You can skip all other repositories to speed up the compiling process.
- How to link to static FFmpeg - save or add the following content to **`.qmake.conf`**:
   ```text
   contains(QT_ARCH, x86_64) {
       LIBS *= -L$$PWD/ffmpeg-static/lib/x64
   } else {
       LIBS *= -L$$PWD/ffmpeg-static/lib/x86
   }
   INCLUDEPATH *= $$PWD/ffmpeg-static/include
   LIBS *= -lVfw32 -lgdiplus -llibass -llibmfx -lSecur32 -lBcrypt -llegacy_stdio_definitions -lavcodec -lavdevice -lavfilter -lavformat -lavresample -lavutil -lswresample -lswscale
   CONFIG *= static_ffmpeg
   ```
   - You should change `$$PWD/ffmpeg-static` to your own static ffmpeg libraries' directory, `libmfx` to your mfx dispatch library file name. If you don't have *libavresample*, please remove `-lavresample`.
   - Download static version of FFmpeg: https://sourceforge.net/projects/avbuild/files/windows-desktop/
   - Download static version of libmfx: https://sourceforge.net/projects/avbuild/files/dep/
   - Download static version of LibASS: https://github.com/ShiftMediaProject/libass/releases/latest

## Licenses
- [Wallpaper](/src/3rdparty/wallpaper): some code is copied from https://github.com/ThomasHuai/Wallpaper , Apache License 2.0, thanks to [ThomasHuai](https://github.com/ThomasHuai)!
- [FFmpeg](https://ffmpeg.zeranoe.com/builds/): LGPLv3/GPLv3, thanks to [FFmpeg project](http://ffmpeg.org/) and [Zeranoe](https://github.com/zeranoe)!
- [QtAV](https://github.com/wang-bin/QtAV): LGPLv3/GPLv3, thanks to [wang bin](https://github.com/wang-bin)!
- [Qt Nice Frameless Window](/src/3rdparty/qtniceframelesswindow): some code is copied from https://github.com/Bringer-of-Light/Qt-Nice-Frameless-Window , MIT, thanks to [Bringer of Light](https://github.com/Bringer-of-Light)!
- [Qt](http://download.qt.io/archive/qt/) and [QtSingleApplication](/src/3rdparty/qtsingleapplication): some code is copied from https://github.com/qtproject/qt-solutions , LGPLv3/GPLv3, thanks to [the Qt Company](https://www.qt.io/)!
- [src/resources/stylesheets/Default.css](/src/resources/stylesheets/Default.css): copied from https://github.com/u8sand/Baka-MPlayer , GPLv2, thanks to [Daniel Clarke](https://github.com/u8sand)!
- [src/resources/stylesheets/Greenery.css](/src/resources/stylesheets/Greenery.css), [src/resources/stylesheets/Marsala.css](/src/resources/stylesheets/Marsala.css), [src/resources/stylesheets/Rose&nbsp;Quartz&nbsp;&amp;&nbsp;Serenity.css](/src/resources/stylesheets/Rose%20Quartz%20&%20Serenity.css), [src/resources/stylesheets/Ultra&nbsp;Violet.css](/src/resources/stylesheets/Ultra%20Violet.css): created by [Yuriko](https://github.com/GA-1101), thanks a lot!
- [src/resources/icons/color_palette.ico](/src/resources/icons/color_palette.ico), [src/resources/icons/color_palette.svg](/src/resources/icons/color_palette.svg): downloaded from https://www.easyicon.net/
- [src/resources/icons/minimize.svg](/src/resources/icons/minimize.svg), [src/resources/icons/close.svg](/src/resources/icons/close.svg), [src/resources/icons/circle-light.svg](/src/resources/icons/circle-light.svg), [src/resources/icons/exit-light.svg](/src/resources/icons/exit-light.svg), [src/resources/icons/info.svg](/src/resources/icons/info.svg), [src/resources/icons/mute-light.svg](/src/resources/icons/mute-light.svg), [src/resources/icons/next-light.svg](/src/resources/icons/next-light.svg), [src/resources/icons/options-light.svg](/src/resources/icons/options-light.svg), [src/resources/icons/pause-light.svg](/src/resources/icons/pause-light.svg), [src/resources/icons/play-light.svg](/src/resources/icons/play-light.svg), [src/resources/icons/playlist-light.svg](/src/resources/icons/playlist-light.svg), [src/resources/icons/previous-light.svg](/src/resources/icons/previous-light.svg), [src/resources/icons/sound-light.svg](/src/resources/icons/sound-light.svg): downloaded from http://www.iconfont.cn/
- [src/resources/icons/checked.svg](/src/resources/icons/checked.svg), [src/resources/icons/down-arrow.svg](/src/resources/icons/down-arrow.svg), [src/resources/icons/selected.svg](/src/resources/icons/selected.svg): copied from https://github.com/u8sand/Baka-MPlayer
- [src/resources/images/colorful.png](/src/resources/images/colorful.png): downloaded from https://ibaotu.com/
