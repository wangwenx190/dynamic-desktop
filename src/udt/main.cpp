#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QFileInfo>

#include <QSimpleUpdater.h>

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
    QStringList arguments = QApplication::arguments();
    arguments.takeFirst();
    const QString dir = QApplication::applicationDirPath();
    /*const QString updateUrl = QStringLiteral("https://raw.githubusercontent.com/wangwenx190/dynamic-desktop/develop/updates.json");
    QString ini = dir + QStringLiteral("/config.ini");
    QSettings settings(ini, QSettings::IniFormat);
    QSimpleUpdater *updater = QSimpleUpdater::getInstance();
    updater->setModuleVersion(updateUrl, QStringLiteral(DD_VERSION));
    bool showUI = true;
    if (arguments.contains(QStringLiteral("--no-gui")))
    {
        app.setQuitOnLastWindowClosed(false);
        showUI = false;
    }
    updater->setNotifyOnUpdate(updateUrl, showUI);
    updater->checkForUpdates(updateUrl);*/
    QString path = dir + QStringLiteral("/dd");
#ifdef WIN64
    path += QStringLiteral("64");
#endif
#ifdef _DEBUG
    path += QStringLiteral("d");
#endif
    path += QStringLiteral(".exe");
    if (QFileInfo::exists(path))
    {
        if (arguments.contains(QStringLiteral("--no-gui"), Qt::CaseInsensitive))
            arguments.removeAll(QStringLiteral("--no-gui"));
        arguments << QStringLiteral("--launch");
        if (QProcess::startDetached(QDir::toNativeSeparators(path), arguments, QDir::toNativeSeparators(dir)))
            return 0;
    }
    return QApplication::exec();
}
