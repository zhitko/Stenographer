#include "downloadandplay.h"
#include <QTextStream>
#include <phonon>

DownloadAndPlay::DownloadAndPlay(QObject *parent) :
    QObject(parent)
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

void DownloadAndPlay::doDownload(const QUrl &url)
{
    QTextStream out(stdout);
    out << url.toString();
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    currentDownloads.append(reply);
}

QString DownloadAndPlay::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename + ".mp3";
}

bool DownloadAndPlay::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void DownloadAndPlay::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        QString filename = saveFileName(url);
        if (saveToDisk(filename, reply)){
            printf("Download of %s succeeded (saved to %s)\n",
                   url.toEncoded().constData(), qPrintable(filename));
            play(filename);
        }
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();
}

void DownloadAndPlay::play(const QString &file){
    QTextStream out(stdout);
    out << "<DownloadAndPlay> play " << file;
    Phonon::MediaObject *music =
             Phonon::createPlayer(Phonon::MusicCategory,
                                  Phonon::MediaSource(file));
    music->play();
}
