#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QApplication>
#include <QtAVWidgets>

AboutDialog::AboutDialog(QWidget *parent) : QWidget(parent)
{
    ui = new Ui::AboutDialog();
    ui->setupUi(this);
    ui->lineEdit_version->setText(QApplication::applicationVersion());
    ui->lineEdit_commit_id->setText(QStringLiteral(DD_COMMIT_ID));
    ui->lineEdit_commit_time->setText(QStringLiteral(DD_COMMIT_TIME));
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
    connect(ui->pushButton_aboutQt, &QPushButton::clicked, qApp, &QApplication::aboutQt);
    connect(ui->pushButton_aboutQtAV, &QPushButton::clicked, this, [=]
    {
        QtAV::aboutQtAV();
    });
    connect(ui->pushButton_aboutFFmpeg, &QPushButton::clicked, this, [=]
    {
        QtAV::aboutFFmpeg();
    });
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::refreshTexts(const QString &language)
{
    Q_UNUSED(language)
    ui->retranslateUi(this);
}
