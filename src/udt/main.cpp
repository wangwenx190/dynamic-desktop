#include "forms/mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSettings>
#include <QDir>
#ifndef BUILD_DD_STATIC
#include <QLibraryInfo>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("Dynamic Desktop Updater"));
    QCoreApplication::setApplicationVersion(QStringLiteral(DD_VERSION));
    QCoreApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
    QApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop Updater"));
#ifdef BUILD_DD_STATIC
    QString qmDir = QStringLiteral(":/i18n");
#else
    QString qmDir = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
    const QString iniPath = QApplication::applicationDirPath() + QDir::separator() + QStringLiteral("config.ini");
    QSettings settings(iniPath, QSettings::IniFormat);
    QString language = settings.value(QStringLiteral("dd/language"), QStringLiteral("auto")).toString();
    QTranslator translator;
    if (language == QStringLiteral("auto"))
    {
        if (translator.load(QLocale(), QStringLiteral("udt"), QStringLiteral("_"), qmDir))
            QApplication::installTranslator(&translator);
    }
    else
    {
        language = QStringLiteral("udt_%0.qm").arg(language);
        if (translator.load(language, qmDir))
            QApplication::installTranslator(&translator);
    }
    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("Dynamic Desktop Updater"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption checkOption(QStringLiteral("check"),
                                   QApplication::translate("main", "Start checking update."));
    parser.addOption(checkOption);
    QCommandLineOption hideOption(QStringLiteral("hide"),
                                  QApplication::translate("main", "Check update without showing any window. But if there is any new version, the main window will show."));
    parser.addOption(hideOption);
    parser.process(app);
    MainWindow mainWindow;
    if (!parser.isSet(hideOption))
        mainWindow.show();
    else
        app.setQuitOnLastWindowClosed(false);
    if (parser.isSet(checkOption) || parser.isSet(hideOption))
        mainWindow.requestUpdate();
    return QApplication::exec();
}
