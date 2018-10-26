[![Build status](https://ci.appveyor.com/api/projects/status/7n23thxiormp6oar/branch/develop?svg=true)](https://ci.appveyor.com/project/wangwenx190/dynamic-desktop/branch/develop)
[![Code quality](https://api.codacy.com/project/badge/Grade/b41d1d384abe4f81a9613945cf654ff6)](https://www.codacy.com/app/wangwenx190/dynamic-desktop?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=wangwenx190/dynamic-desktop&amp;utm_campaign=Badge_Grade)
[![Bugs](https://img.shields.io/github/issues/wangwenx190/dynamic-desktop/bug.svg)](https://github.com/wangwenx190/dynamic-desktop/issues?utf8=✓&q=is%3Aissue+is%3Aopen+label%3Abug)
[![Latest release](https://img.shields.io/github/downloads/wangwenx190/dynamic-desktop/total.svg)](https://github.com/wangwenx190/dynamic-desktop/releases/latest)

[![Download dynamic desktop](https://sourceforge.net/sflogo.php?type=16&group_id=3035605)](https://sourceforge.net/p/dynamic-desktop/)
[![Download dynamic desktop](https://img.shields.io/sourceforge/dt/dynamic-desktop.svg)](https://sourceforge.net/projects/dynamic-desktop/files/latest/download)
[![Download dynamic desktop](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/dynamic-desktop/files/latest/download)

## Overview
A small tool that can play videos on your desktop. Only for Microsoft Windows 7 ~ 10(with GUI desktop). It may have some limitations because it's just a media player, putting it's window under your desktop icons. Maybe Wallpaper Engine's principle is totally different with this tool.

I created this tool only to play my favorite animation clips on my desktop, so I will never make it just like Wallpaper Engine. This tool will stay simple and will only play videos. It doesn't have as many functions as Wallpaper Engine has, but that's indeed what I want. Although this tool has the ability to show executables and web pages on your desktop, I don't want to add this kind of functions. However, if you really need it, you can fork this repository and do it yourself.

**Suggestions are welcome. And I'll be very happy if anyone reports any issue to me.**

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
- Clone or download this repository
- Download [Qt5](http://download.qt.io/archive/qt/) and install it.
- Download [QtAV](https://github.com/wang-bin/QtAV)'s prebuilt SDK and install it, or build yourself.
- Download [FFmpeg](https://github.com/wang-bin/avbuild)'s prebuilt binaries or build yourself.
- Open "dynamic-desktop.pro" and start compiling or call "build.bat". But remember to call "vcvarsall.bat" and add Qt's directories to your path variables before calling "build.bat".

**IMPORTANT NOTES**
- You can also use Intel C++ Compiler(ICC), Clang or MinGW to compile it, just remember to set up environment variables. Cross compile on other platforms may also work, but it will never run on platforms other than MS Windows.
- Submodule *launcher* depends on *Qt Windows Extras* module, *ipc* depends on *Qt Remote Objects* module, please make sure your Qt has these modules. However, you could also change the source code to remove these dependences.

### How to build installer
1. Download and install the latest version of [Inno Setup](http://jrsoftware.org/isdl.php).
2. Create a plain text file named **.qmake.conf** in the root directory of this repository and write:
   ```text
   inno_dir = C:/Program Files (x86)/Inno Setup 5
   CONFIG *= build_installer
   ```
   You should change `C:/Program Files (x86)/Inno Setup 5` to your own Inno Setup directory. **Jom/Nmake may report errors while it's installing files if you enabled this feature, currently don't know why, you can use ".\ci\build_installer.bat" instead temporarily.**
3. Call `build.bat` or run qmake and jom/nmake manually.
4. You can find the generated installer in *./build*.

## Licenses
- [Wallpaper](/src/3rdparty/wallpaper): some code is copied from https://github.com/ThomasHuai/Wallpaper , Apache License 2.0, thanks to [ThomasHuai](https://github.com/ThomasHuai)!
- [QtAV](https://github.com/wang-bin/QtAV) and [FFmpeg](https://github.com/wang-bin/avbuild): LGPL, thanks to [wang bin](https://github.com/wang-bin)!
- [Qt Nice Frameless Window](/src/3rdparty/qtniceframelesswindow): some code is copied from https://github.com/Bringer-of-Light/Qt-Nice-Frameless-Window , MIT, thanks to [Bringer of Light](https://github.com/Bringer-of-Light)!
- [Qt](http://download.qt.io/archive/qt/) and [Qt Service](/src/3rdparty/qtservice): some code is copied from https://github.com/qtproject/qt-solutions , LGPL, thanks to [the Qt Company](https://www.qt.io/)!
- [src/resources/stylesheets/Default.css](/src/resources/stylesheets/Default.css): copied from https://github.com/u8sand/Baka-MPlayer , GPLv2, thanks to [Daniel Clarke](https://github.com/u8sand)!
- [src/resources/stylesheets/Greenery.css](/src/resources/stylesheets/Greenery.css), [src/resources/stylesheets/Marsala.css](/src/resources/stylesheets/Marsala.css), [src/resources/stylesheets/Rose&nbsp;Quartz&nbsp;&amp;&nbsp;Serenity.css](/src/resources/stylesheets/Rose%20Quartz%20&%20Serenity.css), [src/resources/stylesheets/Ultra&nbsp;Violet.css](/src/resources/stylesheets/Ultra%20Violet.css): created by [Yuriko](https://github.com/GA-1101), thanks a lot!
- [src/resources/icons/color_palette.ico](/src/resources/icons/color_palette.ico) and [src/resources/icons/color_palette.svg](/src/resources/icons/color_palette.svg): downloaded from https://www.easyicon.net/
- [src/resources/icons/minimize.svg](/src/resources/icons/minimize.svg), [src/resources/icons/close.svg](/src/resources/icons/close.svg): downloaded from http://www.svgnfont.cn/
- [src/resources/icons/checked.svg](/src/resources/icons/checked.svg), [src/resources/icons/down-arrow.svg](/src/resources/icons/down-arrow.svg), [src/resources/icons/selected.svg](/src/resources/icons/selected.svg): copied from https://github.com/u8sand/Baka-MPlayer
- Dynamic Desktop: [GPLv3](/LICENSE.md)
