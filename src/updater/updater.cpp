#include <Utils>
#include <QSimpleUpdater>
#include <SettingsManager>

#include <QApplication>
#include <QFileInfo>
#include <QTranslator>
#include <QLocale>
#ifndef BUILD_DD_STATIC
#include <QLibraryInfo>
#endif

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Dynamic Desktop Updater"));
    QApplication::setApplicationVersion(QStringLiteral(DD_VERSION));
    QApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
    QApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop Updater"));
#ifdef BUILD_DD_STATIC
    QString qmDir = QStringLiteral(":/i18n");
#else
    QString qmDir = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
    QString language = SettingsManager::getInstance()->getLanguage();
    QTranslator translator;
    if (language == QStringLiteral("auto"))
    {
        if (translator.load(QLocale(), QStringLiteral("udt"), QStringLiteral("_"), qmDir))
            QApplication::installTranslator(&translator);
    }
    else
    {
        language = QStringLiteral("udt_%0").arg(language);
        if (translator.load(language, qmDir))
            QApplication::installTranslator(&translator);
    }
    QStringList arguments = QApplication::arguments();
    arguments.takeFirst();
    bool autoUpdate = arguments.contains(QStringLiteral("--auto-update"), Qt::CaseInsensitive);
    if (autoUpdate)
        arguments.removeAll(QStringLiteral("--auto-update"));
    if (!arguments.contains(QStringLiteral("--launch"), Qt::CaseInsensitive))
        arguments << QStringLiteral("--launch");
    const QString updateUrl = QStringLiteral("https://raw.githubusercontent.com/wangwenx190/dynamic-desktop/develop/src/updates.json");
    QSimpleUpdater *updater = QSimpleUpdater::getInstance();
    updater->setModuleName(updateUrl, QStringLiteral("Dynamic Desktop"));
    updater->setModuleVersion(updateUrl, QStringLiteral(DD_VERSION));
    updater->setNotifyOnUpdate(updateUrl, true);
    updater->setNotifyOnFinish(updateUrl, !autoUpdate);
    updater->setAutoUpdate(updateUrl, autoUpdate);
    updater->setUseCustomInstallProcedures(updateUrl, true);
    updater->checkForUpdates(updateUrl);
    QObject::connect(updater, &QSimpleUpdater::checkingFinished, [=](const QString& url)
    {
        if (!updater->getUpdateAvailable(url) && autoUpdate)
        {
            QString launcherPath = QApplication::applicationDirPath() + QStringLiteral("/launcher");
#ifdef _DEBUG
            launcherPath += QStringLiteral("d");
#endif
            launcherPath += QStringLiteral(".exe");
            if (Utils::run(launcherPath, arguments))
                exit(0);
            else
                exit(-1);
        }
    });
    QObject::connect(updater, &QSimpleUpdater::downloadFinished, [=](const QString& url, const QString& filePath)
    {
        Q_UNUSED(url)
        QStringList installerArguments;
        installerArguments << QStringLiteral("/SILENT") << QStringLiteral("/DIR=\"") + QDir::toNativeSeparators(QApplication::applicationDirPath()) + QLatin1Char('"');
        if (!arguments.isEmpty())
            installerArguments << QStringLiteral("/ARGS=\"") + arguments.join(QLatin1Char(' ')) + QLatin1Char('"');
        if (autoUpdate)
            if (Utils::run(filePath, installerArguments))
                exit(0);
            else
                exit(-1);
    });
    return QApplication::exec();
}
