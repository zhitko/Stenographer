#include "googlespeech.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProcess>

GoogleSpeech::GoogleSpeech(QObject *parent) :
    QObject(parent)
{
}

void GoogleSpeech::sent(QFile * file)
{
    emit logging("<GoogleSpeech> Sent new file: ");

    QString cmd = "";
    QString fileNameNew = file->fileName() + ".flac";
    #ifdef Q_OS_LINUX
        cmd = "flac -f -s ./Filename.wav -o ./Filename.wav.flac";
    #else //Q_OS_MSDOS
        cmd = "flac.exe -f -s " + file->fileName() + " -o " + fileNameNew;
    #endif
    emit logging("<GoogleSpeech> Exec: "+cmd);

//    #if defined(Q_WS_WIN)  //Если платформа Windows
//    #elif defined(Q_WS_X11) // Если платформа, с использованием Х11(в том числе Linux)

    QProcess *console=new QProcess();
    console->start(cmd);
    console->waitForReadyRead();
    emit logging(console->readAllStandardOutput());

    nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

    QUrl url("http://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=ru-RU");
    //QUrl url("http://www.google.com/speech-api/v1/recognize?client=chromium&lang=ru-Ru&maxresults=10");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "audio/x-flac; rate=16000");
    m_file = new QFile(fileNameNew);
    m_file->open(QFile::ReadOnly);
    nam->post(request, m_file);

    //QNetworkReply* reply = nam->post(QNetworkRequest(url),file);
}

void GoogleSpeech::finishedSlot(QNetworkReply * reply)
{
    m_file->close();
    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to handle this

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here

        // Example 1: Creating QImage from the reply
//        QImageReader imageReader(reply);
//        QImage pic = imageReader.read();

        // Example 2: Reading bytes form the reply
        QByteArray bytes = reply->readAll();  // bytes
        QString string(bytes); // string
        emit logging("<GoogleSpeech> Reply: " + string);
        emit getText(string);
    }
    // Some http error received
    else
    {
        // handle errors here
        emit logging("<GoogleSpeech> Error: " + reply->errorString());
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    //delete reply;
}
