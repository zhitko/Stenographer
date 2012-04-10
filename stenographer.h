#ifndef STENOGRAPHER_H
#define STENOGRAPHER_H

#include <QObject>
#include <QVariant>
#include <QFile>
#include <QAudioFormat>
#include <QAudioOutput>

class RecThread;
class GoogleSpeech;

class Stenographer : public QObject
{
    Q_OBJECT
public:
    explicit Stenographer(QObject *parent = 0);
    
signals:
    void updateData(QVariant);
    void updateLog(QVariant);
    
public slots:
    void beginStopRec();
    void playLast();
    void convertAndSent();
    void closeFile();
    void addRecText(QString);
    void logging(QString);

private:
    QFile * lastWavFile;
    RecThread * recThread;
    QAudioFormat format;
    QAudioOutput* outputAudio;
    GoogleSpeech * googleSpeech;
    
};

#endif // STENOGRAPHER_H
