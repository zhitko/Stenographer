#ifndef STENOGRAPHER_H
#define STENOGRAPHER_H

#include <QObject>
#include <QVariant>
#include <QFile>
#include <QAudioFormat>
#include <QAudioOutput>

class RecThread;
class GoogleSpeech;
class QAudioInput;
class DownloadAndPlay;

class Stenographer : public QObject
{
    Q_OBJECT
public:
    explicit Stenographer(QObject *parent = 0);
    RecThread * getRec(){ return recThread; }
    
signals:
    void updateData(QVariant);
    void updateLog(QVariant);
    void setImage(QVariant);
    void setVolumeValue(QVariant);
    void setVolumeRange(QVariant);
    void testStopped(QVariant);
    void needToStop();
    void setAudioDevices(QVariant);
    
public slots:
    void getOptions();
    void beginStopRec();
    void stopRec();
    void playLast(QVariant);
    void convertAndSent();
    void closeFile();
    void addRecText(QString);
    void logging(QString);
    void getVolumeValue(QVariant);
    void getVolumeRange(QVariant);
    void startTest();
    void _testStopped(int);
    void switchAutoMod();
    void setOverK(QVariant);
    void changeAudioDevice(QVariant);
    void saveText(QVariant,QVariant);
    void setSaveTempFiles(QVariant);

private:
    QString lastWavFile;
    RecThread * recThread;
    QAudioFormat format;
    QAudioOutput* outputAudio;
    GoogleSpeech * googleSpeech;
    DownloadAndPlay * downloadAndPlay;
    QFile * m_file;
    bool m_autoMod;
    bool m_isActive;
    bool m_saveTmpFiles;
//    QAudioInput * inputAudio;
    
};

#endif // STENOGRAPHER_H
