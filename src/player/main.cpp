#include <QtSingleApplication>
#include <SettingsManager>
#include <Utils>

#include <Windows.h>

#include <QMessageBox>
#include <QTranslator>
#ifndef BUILD_DD_STATIC
#include <QLibraryInfo>
#endif

static HANDLE mutex = nullptr;

int main(int argc, char *argv[])
{
    QtSingleApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QtSingleApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QtSingleApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QtSingleApplication::setAttribute(Qt::AA_UseOpenGLES);
    QtSingleApplication app(QStringLiteral(DD_MUTEX), argc, argv);
    QtSingleApplication::setApplicationName(QStringLiteral("Dynamic Desktop Player"));
    QtSingleApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop Player"));
    QtSingleApplication::setApplicationVersion(QStringLiteral(DD_VERSION));
    QtSingleApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QtSingleApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
#ifndef _DEBUG
    const QStringList arguments = QtSingleApplication::arguments();
    if (!QApplication::arguments().contains(QStringLiteral("--runfromlauncher"), Qt::CaseInsensitive))
        return 0;
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
        if (translator.load(QLocale(), QStringLiteral("player"), QStringLiteral("_"), qmDir))
            QtSingleApplication::installTranslator(&translator);
    }
    else
    {
        language = QStringLiteral("player_%0.qm").arg(language);
        if (translator.load(language, qmDir))
            QtSingleApplication::installTranslator(&translator);
    }
    mutex = CreateMutex(nullptr, FALSE, TEXT(DD_MUTEX));
    if ((mutex != nullptr) && (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("There is another instance running. Please do not run twice."));
        ReleaseMutex(mutex);
        return 0;
    }
    return 0;
}
