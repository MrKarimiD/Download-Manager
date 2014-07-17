#include "downloadfragmnet.h"

DownloadFragmnet::DownloadFragmnet(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager;
    firstTime=true;
}

void DownloadFragmnet::Download(QString fileURL,int start,int end)
{
    setBoubdaries(start,end);
    QNetworkRequest request;
    request.setUrl(QUrl(fileURL));
    QByteArray rangeHeaderValue = "bytes=" + QByteArray::number(startByte) + "-"+ QByteArray::number(endByte);
    request.setRawHeader("Range",rangeHeaderValue);
    reply = manager->get(request);
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}

void DownloadFragmnet::Download(QString fileURL, int start)
{
    startByte=start;
    QNetworkRequest request;
    request.setUrl(QUrl(fileURL));
    QByteArray rangeHeaderValue = "bytes=" + QByteArray::number(startByte) + "-";
    request.setRawHeader("Range",rangeHeaderValue);
    reply = manager->get(request);

    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}

void DownloadFragmnet::setIndex(int index)
{
    this->index=index;
}

void DownloadFragmnet::setBoubdaries(int start, int end)
{
    this->startByte=start;
    this->endByte=end;
}

void DownloadFragmnet::onDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
{
    emit valueChanged(index,bytesRead);
    if(firstTime)
    {
        diff=bytesRead;
        firstTime=false;
    }
    else
    {
        diff=bytesRead-diff;
        firstTime=true;
        emit difference(diff);
    }
}

void DownloadFragmnet::onReadyRead()
{
    dataFragment.append(reply->readAll());
}

void DownloadFragmnet::onFinished(QNetworkReply *)
{
    switch(reply->error())
    {
        case QNetworkReply::NoError:
        {
            emit Finished(index,dataFragment);
        }break;
        default:{
            emit errorHappened(index);
        };
    }
}
