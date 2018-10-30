#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QtGlobal>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QVersionNumber>
#include <QtAV>

AboutDialog::AboutDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->lineEdit_version->setText(QApplication::applicationVersion());
    ui->lineEdit_commit_id->setText(QStringLiteral(DD_COMMIT_ID));
    ui->lineEdit_commit_time->setText(QStringLiteral(DD_COMMIT_TIME));
    const QString qtCompiledVersionText(QStringLiteral(QT_VERSION_STR));
    const QString qtRuntimeVersionText(qVersion());
    int suffixIndex;
    QVersionNumber qtCompiledVersion = QVersionNumber::fromString(qtCompiledVersionText, &suffixIndex);
    QVersionNumber qtRuntimeVersion = QVersionNumber::fromString(qtRuntimeVersionText, &suffixIndex);
    QString qtVersionText(qtCompiledVersionText);
    if (qtCompiledVersion != qtRuntimeVersion)
        qtVersionText = tr("Run-time version: %0 / Compile-time version: %1")
                .arg(qtRuntimeVersionText)
                .arg(qtCompiledVersionText);
    ui->lineEdit_qt->setText(qtVersionText);
    ui->lineEdit_qtav->setText(QtAV_Version_String());
    ui->lineEdit_ffmpeg->setText(QStringLiteral("4.1-git"));
    QString compiler;
#ifdef __clang__
    compiler = QStringLiteral("Clang v%0.%1.%2").arg(__clang_major__).arg(__clang_minor__).arg(__clang_patchlevel__);
#elif defined(_MSC_VER)
    compiler = QStringLiteral("MSVC v%0").arg(_MSC_FULL_VER);
#elif defined(__GNUC__)
    compiler = QStringLiteral("GCC v%0.%1.%2").arg(__GNUC__).arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__);
#else
    compiler = QStringLiteral("Unknown");
#endif
    ui->lineEdit_compiler->setText(compiler);
#ifdef WIN64
    const QString arch = QStringLiteral("x64");
#else
    const QString arch = QStringLiteral("x86");
#endif
    ui->lineEdit_arch->setText(arch);
    ui->lineEdit_build_time->setText(QStringLiteral("%0 %1").arg(QStringLiteral(__DATE__)).arg(QStringLiteral(__TIME__)));
    connect(ui->pushButton_aboutQtAV, &QPushButton::clicked, [=]
    {
        //about();
    });
    connect(ui->pushButton_ok, &QPushButton::clicked, this, &AboutDialog::close);
    connect(ui->pushButton_source, &QPushButton::clicked, this, [=]
    {
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://github.com/wangwenx190/dynamic-desktop")));
    });
    connect(ui->pushButton_issues, &QPushButton::clicked, this, [=]
    {
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://github.com/wangwenx190/dynamic-desktop/issues")));
    });
    connect(ui->pushButton_release, &QPushButton::clicked, this, [=]
    {
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://sourceforge.net/p/dynamic-desktop/")));
    });
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
