#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dm = new DownloadManager;
    numberOfConnection=4;
    ui->progressBar->setMinimum(0);
    ui->progressBar2->setMinimum(0);
    ui->progressBar3->setMinimum(0);
    ui->progressBar4->setMinimum(0);

    connect(dm,SIGNAL(sizeOfAll(qint64)),this,SLOT(sizeOfFile(qint64)));
    connect(dm,SIGNAL(speedOfDownload(double)),this,SLOT(showDownloadSpeed(double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_address_button_clicked()
{
    QString fileAddress = QFileDialog::getExistingDirectory(this,tr("Save Address Book"));
    ui->address_lineEdit->setText(fileAddress);
    path=fileAddress;
    path.append("/");
}

void MainWindow::on_start_button_clicked()
{
    ui->address_lineEdit->setDisabled(true);
    ui->url_lineEdit->setDisabled(true);
    dm->Download(ui->url_lineEdit->text(),path);
    ui->status->setText("Starting...");
}

void MainWindow::sizeOfFile(qint64 size)
{
    ui->status->setText("Downloading...");
    QString temp=QString::number(size/1024);
    temp.append("  KB");
    ui->size->setText(temp);
    ui->progressBar->setMaximum(size/4);
    ui->progressBar2->setMaximum(size/4);
    ui->progressBar3->setMaximum(size/4);
    ui->progressBar4->setMaximum(size/4);

    for(int i=0;i<numberOfConnection;i++)
    {
        connect(&dm->downloadFrag[i],SIGNAL(valueChanged(int,qint64)),this,SLOT(howMuchRecieved(int,qint64)));
    }

    connect(dm,SIGNAL(Finished()),this,SLOT(FinishedRecieve()));
    connect(dm,SIGNAL(errorHappened()),this,SLOT(ErrorRecieved()));
}

void MainWindow::howMuchRecieved(int index, qint64 input)
{
    switch (index) {
    case 0:
        ui->progressBar->setValue(input);
        break;
    case 1:
        ui->progressBar2->setValue(input);
        break;
    case 2:
        ui->progressBar3->setValue(input);
        break;
    case 3:
        ui->progressBar4->setValue(input);
        break;
    }
}

void MainWindow::FinishedRecieve()
{
    ui->status->setText("Finished Successfully");
    QPalette palette = ui->status->palette();
    palette.setColor(ui->status->foregroundRole(), Qt::green);
    ui->status->setPalette(palette);
}

void MainWindow::ErrorRecieved()
{
     ui->status->setText("Error Happened!Finished unexpendently!");
     QPalette palette = ui->status->palette();
     palette.setColor(ui->status->foregroundRole(), Qt::red);
     ui->status->setPalette(palette);

}

void MainWindow::showDownloadSpeed(double speed)
{
    QString temp=QString::number(speed/1024)+" KB/s";
    ui->speed->setText(temp);
}
