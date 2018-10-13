#include "settingsmanager.h"
#include "skinmanager.h"
#include "utils.h"
#include "mainwindow.h"

#include <Windows.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QTranslator>
#include <QLocale>
#ifndef BUILD_DD_STATIC
#include <QLibraryInfo>
#endif
#include <QSysInfo>
#include <QVersionNumber>
#include <QCommandLineParser>
#include <QCommandLineOption>

static bool windowMode = false;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("Dynamic Desktop"));
    QApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop"));
    QCoreApplication::setApplicationVersion(QStringLiteral(DD_VERSION));
    QCoreApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
#ifndef _DEBUG
    qInstallMessageHandler(Utils::fileLogger);
#endif
#ifdef BUILD_DD_STATIC
    QString qmDir = QStringLiteral(":/i18n");
#else
    QString qmDir = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
    QString language = SettingsManager::getInstance()->getLanguage();
    QTranslator translator;
    if (language == QStringLiteral("auto"))
    {
        if (translator.load(QLocale(), QStringLiteral("dd"), QStringLiteral("_"), qmDir))
            QApplication::installTranslator(&translator);
    }
    else
    {
        language = QStringLiteral("dd_%0.qm").arg(language);
        if (translator.load(language, qmDir))
            QApplication::installTranslator(&translator);
    }
    int suffixIndex;
    QVersionNumber currentVersion = QVersionNumber::fromString(QSysInfo::kernelVersion(), &suffixIndex);
    QVersionNumber win7Version(6, 1, 7600);
    if (currentVersion < win7Version)
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("This application only supports Windows 7 and newer."));
        Utils::Exit(-1);
    }
    Utils::mutex = CreateMutex(nullptr, FALSE, TEXT(DD_MUTEX));
    if ((Utils::mutex != nullptr) && (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("There is another instance running. Please do not run twice."));
        ReleaseMutex(Utils::mutex);
        return 0;
    }
    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("A tool that make your desktop alive."));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption windowModeOption(QStringLiteral("window"),
                                        QApplication::translate("main", "Show a normal window instead of placing it under the desktop icons."));
    parser.addOption(windowModeOption);
    QCommandLineOption skinOption(QStringLiteral("skin"),
                                  QApplication::translate("main", "Set skin. The value is the file name of the skin file, excluding the file extension. If it's not under the \"skins\" folder, please give the absolute path of the file."),
                                  QApplication::translate("main", "Skin file name"));
    parser.addOption(skinOption);
    QCommandLineOption urlOption(QStringLiteral("url"),
                                 QApplication::translate("main", "Play the given url. It can be a local file or a valid web url. Default is empty."),
                                 QApplication::translate("main", "url"));
    parser.addOption(urlOption);
    /*QCommandLineOption keepRatioOption(QStringLiteral("keepratio"),
                                        QApplication::translate("main", "Make the output image keep original video aspect ratio instead of fitting the whole renderer window."));
    parser.addOption(keepRatioOption);*/
    QCommandLineOption imageQualityOption(QStringLiteral("quality"),
                                          QApplication::translate("main", "Set the quality of the output image. It can be default/best/fastest. Default is fastest. Case insensitive."),
                                          QApplication::translate("main", "Image quality"));
    parser.addOption(imageQualityOption);
    QCommandLineOption rendererOption(QStringLiteral("renderer"),
                                      QApplication::translate("main", "Set rendering engine. It can be opengl/gl/qt/gdi/d2d. Default is gl. Case insensitive."),
                                      QApplication::translate("main", "renderer"));
    parser.addOption(rendererOption);
    QCommandLineOption volumeOption(QStringLiteral("volume"),
                                    QApplication::translate("main", "Set volume. It must be a positive integer between 0 and 99. Default is 9."),
                                    QApplication::translate("main", "volume"));
    parser.addOption(volumeOption);
    parser.process(app);
    windowMode = parser.isSet(windowModeOption);
    QString skinOptionValue = parser.value(skinOption);
    if (!skinOptionValue.isEmpty())
        if (skinOptionValue != SettingsManager::getInstance()->getSkin())
            SettingsManager::getInstance()->setSkin(skinOptionValue);
    QString urlOptionValue = parser.value(urlOption);
    if (!urlOptionValue.isEmpty())
        if (urlOptionValue != SettingsManager::getInstance()->getUrl())
            SettingsManager::getInstance()->setUrl(urlOptionValue);
    /*bool keepRatioOptionValue = parser.isSet(keepRatioOption);
    if (keepRatioOptionValue != !SettingsManager::getInstance()->getFitDesktop())
        SettingsManager::getInstance()->setFitDesktop(!keepRatioOptionValue);*/
    QString imageQualityOptionValue = parser.value(imageQualityOption).toLower();
    if (!imageQualityOptionValue.isEmpty())
        if (((imageQualityOptionValue == QStringLiteral("default")) ||
                (imageQualityOptionValue == QStringLiteral("best")) ||
                (imageQualityOptionValue == QStringLiteral("fastest"))) &&
                (imageQualityOptionValue != SettingsManager::getInstance()->getImageQuality()))
            SettingsManager::getInstance()->setImageQuality(imageQualityOptionValue);
    QString rendererOptionValue = parser.value(rendererOption).toLower();
    if (!rendererOptionValue.isEmpty())
        if ((rendererOptionValue == QStringLiteral("opengl")) &&
                (SettingsManager::getInstance()->getRenderer() != QtAV::VideoRendererId_OpenGLWidget))
            SettingsManager::getInstance()->setRenderer(QtAV::VideoRendererId_OpenGLWidget);
        else if ((rendererOptionValue == QStringLiteral("gl")) &&
                 (SettingsManager::getInstance()->getRenderer() != QtAV::VideoRendererId_GLWidget2))
            SettingsManager::getInstance()->setRenderer(QtAV::VideoRendererId_GLWidget2);
        else if ((rendererOptionValue == QStringLiteral("qt")) &&
                 (SettingsManager::getInstance()->getRenderer() != QtAV::VideoRendererId_Widget))
            SettingsManager::getInstance()->setRenderer(QtAV::VideoRendererId_Widget);
        else if ((rendererOptionValue == QStringLiteral("gdi")) &&
                 (SettingsManager::getInstance()->getRenderer() != QtAV::VideoRendererId_GDI))
            SettingsManager::getInstance()->setRenderer(QtAV::VideoRendererId_GDI);
        else if ((rendererOptionValue == QStringLiteral("d2d")) &&
                 (SettingsManager::getInstance()->getRenderer() != QtAV::VideoRendererId_Direct2D))
            SettingsManager::getInstance()->setRenderer(QtAV::VideoRendererId_Direct2D);
    QString volumeOptionValue = parser.value(volumeOption);
    if (!volumeOptionValue.isEmpty())
    {
        int volumeOptionValueInt = volumeOptionValue.toInt();
        if (volumeOptionValueInt < 0)
            volumeOptionValueInt = 0;
        if (volumeOptionValueInt > 99)
            volumeOptionValueInt = 99;
        if (static_cast<unsigned int>(volumeOptionValueInt) != SettingsManager::getInstance()->getVolume())
            SettingsManager::getInstance()->setVolume(static_cast<unsigned int>(volumeOptionValueInt));
    }
    SkinManager::getInstance()->setSkin(SettingsManager::getInstance()->getSkin());
    MainWindow mainWindow;
    const Qt::WindowFlags rendererWindowFlags = Qt::FramelessWindowHint;
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(&mainWindow);
    if (!windowMode)
    {
        mainWindow.setWindowFlags(rendererWindowFlags);
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
        HWND hwnd = Utils::getWorkerW(currentVersion < win10Version);
        if (hwnd != nullptr)
            SetParent(reinterpret_cast<HWND>(mainWindow.winId()), hwnd);
    }
    else
    {
        mainWindow.resize(QSize(1280, 720));
        Utils::moveToCenter(&mainWindow);
    }
    if (!SettingsManager::getInstance()->getUrl().isEmpty())
    {
        if (mainWindow.isHidden())
            mainWindow.show();
        mainWindow.play(SettingsManager::getInstance()->getUrl());
    }
    else
        mainWindow.showOptions();
    return Utils::ExitProgram(QApplication::exec());
}
