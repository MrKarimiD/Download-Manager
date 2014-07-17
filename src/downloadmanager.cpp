#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{
    this->manager = new QNetworkAccessManager(this);
    firstTime=true;
    numberOfConnections=4;
    sum=0;
    dataPart=new QByteArray[numberOfConnections];
    downloadFrag=new DownloadFragmnet[numberOfConnections];
    for(int i=0;i<numberOfConnections;i++)
    {
        downloadFrag[i].setIndex(i);
    }
    eachPartDownloadSuccessfully=new bool[numberOfConnections];
    for(int i=0;i<numberOfConnections;i++)
    {
        eachPartDownloadSuccessfully[i]=false;
    }
    thread=new QThread();
    checkTimer=new QTimer();
    speedTimer=new QTimer();
    semaphore=new QSemaphore(4);
    semaphoreForSpeed=new QSemaphore(1);
}

void DownloadManager::Download(QString fileURL, QString fileAddress)
{
        this->fileUrl=fileURL;
        QString filePath = fileURL;
        QString saveFilePath;
        QStringList filePathList = filePath.split('/');
        QString fileName = filePathList.at(filePathList.count() - 1);
        saveFilePath = QString(fileAddress + fileName );

        QNetworkRequest request;
        request.setUrl(QUrl(fileURL));
        reply = manager->get(request);
        checkTimer->start(100);
        speedTimer->start(1000);

        file = new QFile;
        file->setFileName(saveFilePath);
        file->open(QIODevice::WriteOnly);

        connect(checkTimer, SIGNAL(timeout()), this, SLOT(checkComplete()));
        connect(speedTimer,SIGNAL(timeout()),this,SLOT(speedCal()));
        connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
}

void DownloadManager::finishedEachSegment(int index, QByteArray data)
{
       semaphore->acquire(1);
       dataPart[index]=data;
       eachPartDownloadSuccessfully[index]=true;
       semaphore->release(1);
}

void DownloadManager::dataRecieved(qint64 diff)
{
    semaphoreForSpeed->acquire(1);
    sum=sum+diff;
    semaphoreForSpeed->release(1);
}

void DownloadManager::checkComplete()
{
    semaphore->acquire(numberOfConnections);

    if(eachPartDownloadSuccessfully[0] && eachPartDownloadSuccessfully[1] && eachPartDownloadSuccessfully[2] && eachPartDownloadSuccessfully[3] )
    {
        for(int i=0;i<numberOfConnections;i++)
        {
            data.append(dataPart[i]);
        }
        file->write(data);

        file->close();
        file->deleteLater();
        checkTimer->stop();
        emit Finished();
    }
    semaphore->release(numberOfConnections);

}

void DownloadManager::speedCal()
{
    semaphoreForSpeed->acquire(1);
    emit speedOfDownload(sum);
    sum=0;
    semaphoreForSpeed->release(1);
}

void DownloadManager::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qint64 size=bytesTotal;
    emit sizeOfAll(size);

    if(firstTime)
    {
        qint64 fragmentSize=size/numberOfConnections;

        for(int i=0;i<numberOfConnections-1;i++)
        {
            downloadFrag[i].setIndex(i);
            downloadFrag[i].Download(fileUrl,(i*fragmentSize),((i+1)*fragmentSize)-1);
            downloadFrag[i].moveToThread(thread);
        }
        downloadFrag[numberOfConnections-1].setIndex(numberOfConnections-1);
        downloadFrag[numberOfConnections-1].Download(fileUrl,((numberOfConnections-1)*fragmentSize));
        downloadFrag[numberOfConnections-1].moveToThread(thread);

        for(int i=0;i<numberOfConnections;i++)
        {
            connect(&downloadFrag[i],SIGNAL(Finished(int,QByteArray)),this,SLOT(finishedEachSegment(int,QByteArray)));
            connect(&downloadFrag[i],SIGNAL(errorHappened(int)),this,SLOT(errorHappened(int)));
            connect(&downloadFrag[i],SIGNAL(difference(qint64)),this,SLOT(dataRecieved(qint64)));
        }
        firstTime=false;
        disconnect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
        thread->start();
    }
}

void DownloadManager::setNumberOfConnections(int number)
{
    numberOfConnections=number;
}

void DownloadManager::errorHappened(int index)
{
    emit errorHappened();
}
