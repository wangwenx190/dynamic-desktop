#include "forms/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Dynamic Desktop Skin Editor"));
    QApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop Skin Editor"));
    QApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
    MainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}
