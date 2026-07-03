#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QCoreApplication>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("DVD Extractor");

    // Define tamanho fixo
    this->setMinimumSize(440, 219);
    this->setMaximumSize(440, 219);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_vob_in_clicked()
{
    vobsin = QFileDialog::getOpenFileNames(this, "Escolher .VOBS", "", "*.VOB");
}

void MainWindow::on_pushButton_vob_out_clicked()
{
    if (!vobsin.isEmpty())
        vobout = QFileDialog::getSaveFileName(this, "Salvar", "", "*.VOB");
}

void MainWindow::on_pushButton_run_clicked()
{
    if (vobsin.isEmpty() || vobout.isEmpty())
        return;

    // calcular tamanho total (apenas para referência)
    qint64 totalSize = 0;
    for (int i = 0; i < vobsin.size(); ++i) {
        QFileInfo fi(vobsin.at(i));
        totalSize += fi.size();
    }


    process = new QProcess(this);

    // quando terminar, força 100%
    QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [=](int, QProcess::ExitStatus) {
                if (progressTimer) {
                    progressTimer->stop();
                    progressTimer->deleteLater();
                }
                ui->progressBar->setValue(100);
            });

    // iniciar timer para simular progresso
    progressTimer = new QTimer(this);
    QObject::connect(progressTimer, &QTimer::timeout, this, [=]() {
        int current = ui->progressBar->value();
        if (current < 95) { // deixa espaço para o 100% final
            ui->progressBar->setValue(current + 1);
        }
    });
    progressTimer->start(200); // atualiza a cada 200ms

    // montar comando cat
    QStringList args = vobsin;
    process->setStandardOutputFile(vobout);
    QString catPath = QCoreApplication::applicationDirPath() + "/cat";
    process->start(catPath, args);

    ui->progressBar->setValue(0);
}
