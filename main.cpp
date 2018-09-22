#include "wallpaperwidget.h"
#include "forms/preferencesdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    WallpaperWidget wallpaper;
    PreferencesDialog preferencesDialog;
    preferencesDialog.show();
    return QApplication::exec();
}
