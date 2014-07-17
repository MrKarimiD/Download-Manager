#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkRequest"
#include "QtNetwork/QNetworkAccessManager"
#include "QUrl"
#include "QFile"
#include "QByteArray"
#include "downloadfragmnet.h"
#include "QThread"
#include "QTimer"
#include <QSemaphore>
#include "QString"
#include "QStringList"

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);
    void Download(QString fileURL,QString fileAddress);
    void setNumberOfConnections(int number);
    DownloadFragmnet *downloadFrag;


private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QByteArray *dataPart;
    QFile *file;
    int numberOfConnections;
    QThread *thread;
    bool firstTime;
    bool allPartsDownloadCorrectly;
    bool *eachPartDownloadSuccessfully;
    QString fileUrl;
    QTimer *checkTimer;
    QTimer *speedTimer;
    QSemaphore *semaphore;
    QSemaphore *semaphoreForSpeed;
    QByteArray data;
    qint64 sum;

signals:
    void sizeOfAll(qint64 newValue);
    void Finished();
    void errorHappened();
    void speedOfDownload(double speed);

private slots:
    void errorHappened(int index);
    void onDownloadProgress(qint64,qint64);
    void finishedEachSegment(int index,QByteArray data);
    void dataRecieved(qint64 diff);
    void checkComplete();
    void speedCal();
};

#endif // DOWNLOADMANAGER_H
