#include <Utils>

#include <QApplication>
//#include <QSettings>
#include <QFileInfo>

//#include <QSimpleUpdater.h>

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
    const QString dir = QApplication::applicationDirPath();
    /*const QString updateUrl = QStringLiteral("https://raw.githubusercontent.com/wangwenx190/dynamic-desktop/develop/src/updates.json");
    QString ini = dir + QStringLiteral("/config.ini");
    QSettings settings(ini, QSettings::IniFormat);
    QSimpleUpdater *updater = QSimpleUpdater::getInstance();
    updater->setModuleVersion(updateUrl, QStringLiteral(DD_VERSION));
    updater->setNotifyOnUpdate(updateUrl, true);
    updater->checkForUpdates(updateUrl);*/
    QString path = dir + QStringLiteral("/launcher");
#ifdef _DEBUG
    path += QStringLiteral("d");
#endif
    path += QStringLiteral(".exe");
    if (QFileInfo::exists(path))
    {
        if (arguments.contains(QStringLiteral("--auto-update"), Qt::CaseInsensitive))
            arguments.removeAll(QStringLiteral("--auto-update"));
        if (!arguments.contains(QStringLiteral("--launch"), Qt::CaseInsensitive))
            arguments << QStringLiteral("--launch");
        if (Utils::run(path, arguments))
            return 0;
    }
    return QApplication::exec();
}
