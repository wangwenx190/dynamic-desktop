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
    QString compiler = QStringLiteral("Unknown");
#ifdef Q_CC_CLANG
    compiler = QStringLiteral("Clang C++ Compiler V%0.%1.%2").arg(__clang_major__).arg(__clang_minor__).arg(__clang_patchlevel__);
#elif defined(Q_CC_MSVC)
    QString msvcFullVer = QString::number(_MSC_FULL_VER);
    msvcFullVer.insert(2, QLatin1Char('.'));
    msvcFullVer.insert(5, QLatin1Char('.'));
    msvcFullVer.append(QLatin1Char('.'));
    QString msvcBuildVer = QString::number(_MSC_BUILD);
    if (_MSC_BUILD < 10)
        msvcBuildVer.prepend(QLatin1Char('0'));
    msvcFullVer.append(msvcBuildVer);
    compiler = QStringLiteral("Microsoft Visual C++ Compiler V") + msvcFullVer;
#elif defined(Q_CC_INTEL)
    compiler = QStringLiteral("Intel C++ Compiler V%0").arg(_MSC_VER); // How to detect ICC version?
#elif defined(Q_CC_GNU)
    compiler = QStringLiteral("GNU C++ Compiler V%0.%1.%2").arg(__GNUC__).arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__); // Are these macros correct?
#endif
    ui->lineEdit_compiler->setText(compiler);
    ui->lineEdit_compiler->setCursorPosition(0);
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

#ifndef DD_NO_TRANSLATIONS
void AboutDialog::refreshTexts(const QString &language)
{
    Q_UNUSED(language)
    ui->retranslateUi(this);
}
#endif
