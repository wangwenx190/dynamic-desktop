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

**My native language is Chinese and I'm not good at English. I use Google Translate to translate Chinese into English. I'm not sure about the accuracy of the translated texts. So if there is anything wrong in English translation, please inform me through [GitHub Issues](https://github.com/wangwenx190/dynamic-desktop/issues). Thanks a lot.**

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
Microsoft Windows 7 ~ 10, desktop editions, both 32 bit and 64 bit systems.

Can't run on systems without GUI desktop, for example, some editions of Windows Server. Linux and macOS will never be supported due to the limitation of the technique I used. Mobile systems are not supported as well for the same reason.

## Screenshots
![Preferences Dialog](/docs/images/preferencesdialog.jpg)

## Downloads
For official builds and third-party packages please see https://sourceforge.net/p/dynamic-desktop/ .

## Bug reports and suggestions
If you find any issues or have any feature requests, please inform me through [**GitHub Issues**](https://github.com/wangwenx190/dynamic-desktop/issues). It would be very nice if you are willing to follow the template provided by GitHub.

## Important notes
- Windows XP and Vista are not supported currently. Windows 2000 and even older systems will never be supported.
- You have to enable **Windows Aero** first if you want to run this tool.
- Make sure you have installed the latest DirectX.
- To experience *Hardware Accelerated Decoding* better, make sure this tool runs with your high performance GPU(on Optimus).
- The CPU and memory usage will increase enormously if you are using software decoding and that's normal, all media players will, don't worry.
- It would be very nice if you update your graphics card driver to the latest version.
- It's not recommended to run this tool in virtual machines, because the graphics card drivers of virtual machines are usually out-dated and even not completed.
- If there is no picture but sound, please switch the hardware decoding algorithm or just use software decoding. Because hardware decoding has many limitations while software decoding has not. In one word, **IF YOU ENCOUNTER WITH ANY STRANGE PHENOMENA, TURN OFF HARDWARE DECODING**.
- It's not recommended to change rendering engine again and again while it is playing videos, otherwise this tool is very likely to crash and I'm not able to help you in this kind of situation.
- Currently, hardware decoding only supports traditional video formats such as AVC/H.264, so please do not attempt to play HEVC videos if you are using hardware decoding.

## Compilation
- Clone or download(remember to download all the submodules together!) this repository:
   - First of all, download **Git** from https://git-for-windows.github.io/ and install it.
   - Run:
      ```bash
      git clone --recursive https://github.com/wangwenx190/dynamic-desktop.git
      ```
      or:
      ```bash
      git clone https://github.com/wangwenx190/dynamic-desktop.git
      git submodule update --init --recursive
      ```
      If a submodule update fails, try running:
      ```bash
      git submodule foreach --recursive git fetch --tags
      ```
      then run the update again:
      ```bash
      git submodule update --init --recursive
      ```
      Note that you can add **`-b master`** to the **`git clone`** command if you want to get the latest stable version instead of the latest development version.
- Download **Qt** at least *5.6.3* from http://download.qt.io/archive/qt/ and install it. Using the latest version is highly recommended.
- Download **FFmpeg** SDK and extract to **`ffmpeg`**. Of course, you can extract to anywhere you want, just add **`ffmpeg_dir = your own FFmpeg dir path`** to **`user.conf`**. Add **`CONFIG *= static_ffmpeg`** to it if you want to link against FFmpeg statically. Using the latest *Git build* is highly recommended.
   - Zeranoe builds (recommended for shared builds): https://ffmpeg.zeranoe.com/builds/

      Git and stable versions, shared libs only, full builds only
   - QtAV builds: https://sourceforge.net/projects/avbuild/files/windows-desktop/

      Git and stable versions, shared and static libs, full and lite builds
   - Shift Media Project builds (recommended for static builds): https://github.com/ShiftMediaProject/FFmpeg/releases/latest

      Git and stable versions, shared and static libs, full builds only
- Use [Qt Creator](http://download.qt.io/official_releases/qtcreator/) to open [dynamic-desktop.pro](/dynamic-desktop.pro) and start compiling or call [build.bat](/build.bat) that will do everything for you. But there are some rules to use it:
   - It requires your Visual Studio version no older than 15(VS2017), Qt version no older than 5.9, because their paths are hard coded in it and some test functions used in .pro files are not introduced until Qt 5.9. Actually, the source code is compatible with all MSVC compilers that support C++11 standard(for lambda function support) and all Qt versions newer than 5.5.
   - parameters: "build.bat" [mkspec] [CONFIG] [Target architecture] [Qt version] [Qt directory]
   - The position of each parameter can't be changed and they can't be skipped, for example, if you want to set Qt version, you will have to give "mkspec", "CONFIG" and "Target architecture" all together. You can run this batch script without any parameters, it means default parameters are used.
   - mkspec: can be "win32-msvc", "win32-icc", "win32-g++", "win32-clang-msvc" or "win32-clang-g++". Default is "win32-msvc". Double quotation marks are indispensable for this parameter.
   - CONFIG: Qt CONFIG and project specific CONFIG, can be any valid CONFIG variables. Default is "release silent". Double quotation marks are indispensable for this parameter.
   - Target architecture: can be x86 or x64. Default is x64. Double quotation marks are not needed.
   - Qt version: can be any valid Qt version, but no older than 5.9 series. Currently default is 5.12.0. Double quotation marks are not needed.
   - Qt directory: if you didn't install Qt in it's default location (C:\Qt), you should pass your own Qt path to the batch script, for example, "D:\Program Files(x86)\Qt\Qt5.12.0\5.12.0\msvc2017_64". Double quotation marks are indispensable for this parameter.

### IMPORTANT NOTES
- You can also use Intel C++ Compiler(ICC), Clang or MinGW to compile it, just remember to set up environment variables. Cross compile on other platforms may also work, but it will never run on platforms other than MS Windows.
- You will need to build the static version of Qt by yourself if you want to get rid of the Qt dlls. Currently, this project only needs 3 repositories: **`qtbase`**(Qt modules needed: core, gui, widgets, network and opengl), **`qtsvg`**(SVG support) and **`qtwinextras`**(Taskbar Progress support). You can skip all other repositories to speed up the compiling process.
- How to link against **`LibASS`** and **`OpenAL`** libraries:
   - You can download the latest **LibASS** SDK for Windows from **Shift Media Project**: https://github.com/ShiftMediaProject/libass/releases/latest . They have both shared and static libraries. They compiled it through MSVC compilers, so it's compatible with my source code. Add the following config to **`user.conf`** to enable LibASS (dynamically load):
      ```text
      CONFIG *= enable_libass
      ```
      If you want to link against LibASS statically, add the following config:
      ```text
      CONFIG *= static_libass
      ```
      If you want to link against *libass.dll* directly, add the following config (compatible with static linking):
      ```text
      CONFIG *= enable_libass_link
      ```
   - You can download the latest **OpenAL** SDK for Windows from **OpenAL Soft**: http://openal-soft.org/#download . Official builds only contains shared libraries. If you want to link against it statically, you have to build static libraries yourself. You can access it's source code on GitHub: https://github.com/kcat/openal-soft . Add the following config to **`user.conf`** to enable OpenAL (dynamically load):
      ```text
      CONFIG *= enable_openal
      ```
      If you want to link against OpenAL statically, add the following config:
      ```text
      CONFIG *= static_openal
      ```
      If you want to link against *OpenAL32.dll* directly, add the following config (compatible with static linking):
      ```text
      CONFIG *= enable_openal_link
      ```
- Use **`qmake -r -tp vc dynamic-desktop.pro`** to generate Visual Studio project files if you want to use VS instead of Qt Creator.

## Licenses
- [Wallpaper](/src/3rdparty/wallpaper): some code is copied from https://github.com/NoisyWinds/Wallpaper , Apache License 2.0, thanks to [NoisyWind](https://github.com/NoisyWinds)!
- [FFmpeg](http://ffmpeg.org/): LGPLv3/GPLv3, thanks to [the FFmpeg project](http://ffmpeg.org/)!
- [QtAV](https://github.com/wangwenx190/qtav-lite): LGPLv3/GPLv3, thanks to [wang bin](https://github.com/wang-bin)!
- [Qt Nice Frameless Window](/src/3rdparty/qtniceframelesswindow): some code is copied from https://github.com/Bringer-of-Light/Qt-Nice-Frameless-Window , MIT, thanks to [Bringer of Light](https://github.com/Bringer-of-Light)!
- [Qt](http://download.qt.io/archive/qt/) and [QtSingleApplication](/src/3rdparty/qtsingleapplication): some code is copied from https://github.com/qtproject/qt-solutions , LGPLv3/GPLv3, thanks to [the Qt Company](https://www.qt.io/)!
- [src/resources/stylesheets/Default.css](/src/resources/stylesheets/Default.css): original file was copied from https://github.com/u8sand/Baka-MPlayer , GPLv2, thanks to [Daniel Clarke](https://github.com/u8sand)! A lot of changes have been made that it is a whole new file now.
- [src/resources/stylesheets/Greenery.css](/src/resources/stylesheets/Greenery.css), [src/resources/stylesheets/Marsala.css](/src/resources/stylesheets/Marsala.css), [src/resources/stylesheets/Rose&nbsp;Quartz&nbsp;&amp;&nbsp;Serenity.css](/src/resources/stylesheets/Rose%20Quartz%20&%20Serenity.css), [src/resources/stylesheets/Ultra&nbsp;Violet.css](/src/resources/stylesheets/Ultra%20Violet.css): all based on the original "Default.css", created by [Yuriko](https://github.com/GA-1101), thanks a lot!
- [src/resources/icons/color_palette.ico](/src/resources/icons/color_palette.ico), [src/resources/icons/color_palette.svg](/src/resources/icons/color_palette.svg): downloaded from https://www.easyicon.net/
- [src/resources/icons/minimize.svg](/src/resources/icons/minimize.svg), [src/resources/icons/close.svg](/src/resources/icons/close.svg), [src/resources/icons/circle-light.svg](/src/resources/icons/circle-light.svg), [src/resources/icons/exit-light.svg](/src/resources/icons/exit-light.svg), [src/resources/icons/info.svg](/src/resources/icons/info.svg), [src/resources/icons/mute-light.svg](/src/resources/icons/mute-light.svg), [src/resources/icons/next-light.svg](/src/resources/icons/next-light.svg), [src/resources/icons/options-light.svg](/src/resources/icons/options-light.svg), [src/resources/icons/pause-light.svg](/src/resources/icons/pause-light.svg), [src/resources/icons/play-light.svg](/src/resources/icons/play-light.svg), [src/resources/icons/playlist-light.svg](/src/resources/icons/playlist-light.svg), [src/resources/icons/previous-light.svg](/src/resources/icons/previous-light.svg), [src/resources/icons/sound-light.svg](/src/resources/icons/sound-light.svg): downloaded from http://www.iconfont.cn/
- [src/resources/icons/checked.svg](/src/resources/icons/checked.svg), [src/resources/icons/down-arrow.svg](/src/resources/icons/down-arrow.svg), [src/resources/icons/selected.svg](/src/resources/icons/selected.svg): copied from https://github.com/u8sand/Baka-MPlayer
