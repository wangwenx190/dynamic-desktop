#include <SettingsManager>
#include <Utils>
#include "forms/traymenu.h"

#include <QApplication>
#include <QSystemTrayIcon>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Dynamic Desktop Tray"));
    QApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop Tray"));
    QApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
    Utils::installTranslation(SettingsManager::getInstance()->getLanguage(), QStringLiteral("try"));
    app.setQuitOnLastWindowClosed(false);
    TrayMenu trayMenu;
    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(QStringLiteral(":/icons/color_palette.svg")));
    trayIcon.setToolTip(QStringLiteral("Dynamic Desktop"));
    trayIcon.setContextMenu(&trayMenu);
    QObject::connect(&trayMenu, &TrayMenu::onExitClicked, qApp, &QApplication::quit);
    return QApplication::exec();
}
