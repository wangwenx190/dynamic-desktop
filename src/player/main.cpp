#include <SettingsManager>
#include <Utils>
#include <Wallpaper>
#include <IPCClient>
#include "mainwindow.h"

#include <Windows.h>

#include <QMessageBox>
#include <QTranslator>
#ifndef BUILD_DD_STATIC
#include <QLibraryInfo>
#endif
#include <QApplication>
#include <QVersionNumber>
#include <QDesktopWidget>
#include <QSysInfo>
#include <QLocale>

static bool windowMode = false;
static HANDLE mutex = nullptr;

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Dynamic Desktop Player"));
    QApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop Player"));
    QApplication::setApplicationVersion(QStringLiteral(DD_VERSION));
    QApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
    const QStringList arguments = QApplication::arguments();
#ifndef _DEBUG
    if (!arguments.contains(QStringLiteral("--runfromlauncher"), Qt::CaseInsensitive))
        return 0;
    qInstallMessageHandler(Utils::fileLogger);
#endif
    windowMode = arguments.contains(QStringLiteral("--window"), Qt::CaseInsensitive);
#ifdef BUILD_DD_STATIC
    QString qmDir = QStringLiteral(":/i18n");
#else
    QString qmDir = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
    QString language = SettingsManager::getInstance()->getLanguage();
    QTranslator translator;
    if (language == QStringLiteral("auto"))
    {
        if (translator.load(QLocale(), QStringLiteral("player"), QStringLiteral("_"), qmDir))
            QApplication::installTranslator(&translator);
    }
    else
    {
        language = QStringLiteral("player_%0.qm").arg(language);
        if (translator.load(language, qmDir))
            QApplication::installTranslator(&translator);
    }
    int suffixIndex;
    QVersionNumber currentVersion = QVersionNumber::fromString(QSysInfo::kernelVersion(), &suffixIndex);
    QVersionNumber win7Version(6, 1, 7600);
    if (currentVersion < win7Version)
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("This application only supports Windows 7 and newer."));
        Utils::Exit(-1, true, mutex);
    }
    mutex = CreateMutex(nullptr, FALSE, TEXT("wangwenx190.DynamicDesktop.Player.1000.AppMutex"));
    if ((mutex != nullptr) && (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("There is another instance running. Please do not run twice."));
        ReleaseMutex(mutex);
        return 0;
    }
    MainWindow mainWindow;
    IPCClient ipcClient;
    QObject::connect(&ipcClient, &IPCClient::serverMessage, &mainWindow, &MainWindow::parseCommand);
    QObject::connect(&mainWindow, &MainWindow::sendCommand, &ipcClient, &IPCClient::clientMessage);
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [=, &ipcClient]
    {
        emit ipcClient.clientMessage(qMakePair(QStringLiteral("quit"), true));
    });
    const Qt::WindowFlags windowFlags = Qt::FramelessWindowHint;
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(&mainWindow);
    if (!windowMode)
    {
        app.setQuitOnLastWindowClosed(false);
        mainWindow.setWindowFlags(windowFlags);
        // Why is Direct2D image too large?
        mainWindow.setGeometry(screenGeometry);
        QVersionNumber win10Version(10, 0, 10240); // Windows 10 Version 1507
        // How to place our window under desktop icons:
        // Use "Program Manager" as our parent window in Win7/8/8.1.
        // Use "WorkerW" as our parent window in Win10.
        // Use "Program Manager" as our parent window in
        // Win10 is also OK, but our window will come
        // to front if we press "Win + Tab" and it will
        // also block our desktop icons, however using
        // "WorkerW" as our parent window will not result
        // in this problem, I don't know why. It's strange.
        Wallpaper::setWallpaper(reinterpret_cast<HWND>(mainWindow.winId()), currentVersion < win10Version);
    }
    else
    {
        app.setQuitOnLastWindowClosed(true);
        mainWindow.resize(QSize(1280, 720));
        Utils::moveToCenter(&mainWindow);
        if (mainWindow.isHidden())
            mainWindow.show();
    }
    if (!SettingsManager::getInstance()->getUrl().isEmpty())
    {
        if (mainWindow.isHidden())
            mainWindow.show();
        mainWindow.urlChanged(SettingsManager::getInstance()->getUrl());
    }
    return Utils::Exit(QApplication::exec(), false, mutex, Wallpaper::getWorkerW());
}
