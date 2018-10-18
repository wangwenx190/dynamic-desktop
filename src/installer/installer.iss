#define MyAppName "Dynamic Desktop"
#define MyAppVersion "1.0.0.0"
#define MyAppPublisher "wangwenx190"
#define MyAppURL "https://github.com/wangwenx190/dynamic-desktop"
#define MyAppExeName "launcher.exe"

[Setup]
AppId={{45875EE3-D96F-422D-80D8-56E450579FD8}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={src}\{#MyAppName}
DisableProgramGroupPage=yes
OutputDir=.\Build
OutputBaseFilename=dd_setup
SetupIconFile=..\resources\icons\color_palette.ico
Compression=lzma2/ultra64
SolidCompression=yes
AppCopyright=GPLv3
AppMutex=wangwenx190.DynamicDesktop.Launcher.1000.AppMutex
PrivilegesRequired=lowest
RestartIfNeededByRun=False
TimeStampsInUTC=True
ShowLanguageDialog=no
Uninstallable=no
VersionInfoVersion=1.0.0.0
VersionInfoCompany=wangwenx190
VersionInfoDescription=Dynamic Desktop Installer
VersionInfoTextVersion=1.0.0.0
VersionInfoCopyright=GPLv3
VersionInfoProductName=Dynamic Desktop
VersionInfoProductVersion=1.0.0.0
VersionInfoProductTextVersion=1.0.0.0
InternalCompressLevel=ultra64
MinVersion=0,6.1
DisableReadyPage=True
DisableReadyMemo=True
DisableDirPage=no
DirExistsWarning=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\..\bin64\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
