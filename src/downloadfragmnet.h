#ifndef DOWNLOADFRAGMNET_H
#define DOWNLOADFRAGMNET_H

#include <QObject>
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkRequest"
#include "QtNetwork/QNetworkAccessManager"
#include "QUrl"
#include "QFile"
#include "QByteArray"

class DownloadFragmnet : public QObject
{
    Q_OBJECT
public:
    explicit DownloadFragmnet(QObject *parent = 0);
    void Download(QString fileURL,int start,int end);
    void Download(QString fileURL,int start);
    void setIndex(int index);
    void setBoubdaries(int start,int end);

private:
    int index,startByte,endByte;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QByteArray dataFragment;
    bool firstTime;
    qint64 diff;

signals:
    void valueChanged(int index,qint64 newValue);
    void Finished(int index,QByteArray data);
    void errorHappened(int index);
    void difference(qint64 diff);

public slots:
    void onDownloadProgress(qint64 bytesRead, qint64 bytesTotal);
    void onReadyRead();
    void onFinished(QNetworkReply*);

};

#endif // DOWNLOADFRAGMNET_H
