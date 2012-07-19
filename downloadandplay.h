#ifndef DOWNLOADANDPLAY_H
#define DOWNLOADANDPLAY_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
#include <QUrl>

class DownloadAndPlay : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;
public:
    explicit DownloadAndPlay(QObject *parent = 0);
    void doDownload(const QUrl &url);
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);
    void play(const QString &file);

public slots:
    void downloadFinished(QNetworkReply *reply);
    
signals:
    
};

#endif // DOWNLOADANDPLAY_H
