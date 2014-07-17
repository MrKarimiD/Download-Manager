#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "downloadmanager.h"
#include "QFileDialog"
#include "QMessageBox"
#include "downloadfragmnet.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_address_button_clicked();
    void on_start_button_clicked();
    void sizeOfFile(qint64 size);
    void howMuchRecieved(int index,qint64 input);
    void FinishedRecieve();
    void ErrorRecieved();
    void showDownloadSpeed(double speed);

private:
    Ui::MainWindow *ui;
    DownloadManager *dm;
    QString path;
    int numberOfConnection;
};

#endif // MAINWINDOW_H
