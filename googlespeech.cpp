#include "googlespeech.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProcess>
#include <QRegExp>
#include <QDebug>

GoogleSpeech::GoogleSpeech(QObject *parent) :
    QObject(parent)
{
}

void GoogleSpeech::sent(QString file_name)
{
    this->sent(file_name, "ru-ru", "AIzaSyBOti4mM-6x9WDnZIjIeyEU21OpBXqWBgw");
}

void GoogleSpeech::sent(QString file_name, QString lang, QString key)
{
    emit logging("<GoogleSpeech> Sent new file: ");

    nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

//    QUrl url("http://www.google.com/speech-api/v1/recognize?client=chromium&lang=ru-Ru&maxresults=10");
    QString url_s = QString("https://www.google.com/speech-api/v2/recognize?output=json&lang=%1&key=%2").arg(lang).arg(key);
    QUrl url(url_s);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "audio/l16; rate=16000");
    qDebug() << "file " << file_name;
    m_file = new QFile(file_name);
    m_file->open(QFile::ReadOnly);
    nam->post(request, m_file);
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

        QString answer = "";
        QRegExp ans("transcript\":\"(.*)\"");
        ans.setMinimal(true);
        int pos = 0;
        while ((pos = ans.indexIn(string, pos)) != -1) {
            answer = answer + "\n" + ans.cap(1);
            pos += ans.matchedLength();
        }

        emit getText(answer);
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
