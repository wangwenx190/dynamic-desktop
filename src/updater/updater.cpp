#include <Utils>
#include <QSimpleUpdater>

#include <QApplication>
#include <QFileInfo>

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
    QObject::connect(updater, &QSimpleUpdater::updateAvailable, [=](bool hasUpdates)
    {
        QStringList launcherArguments = arguments;
        QString launcherPath = QApplication::applicationDirPath() + QStringLiteral("/launcher");
#ifdef _DEBUG
        launcherPath += QStringLiteral("d");
#endif
        launcherPath += QStringLiteral(".exe");
        if (hasUpdates)
        {
            launcherPath = updater->getDownloadDir(updateUrl) + QLatin1Char('/') + updater->getDownloadFileName(updateUrl);
            launcherArguments = QStringList() << QStringLiteral("/ARGS=\"") + arguments.join(QLatin1Char(' ')) + QLatin1Char('"');
            launcherArguments << QStringLiteral("/SILENT") << QStringLiteral("/DIR=\"") + QDir::toNativeSeparators(QApplication::applicationDirPath()) + QLatin1Char('"');
        }
        if (autoUpdate)
            if (Utils::run(launcherPath, launcherArguments))
                exit(0);
            else
                exit(-1);
    });
    return QApplication::exec();
}
