#ifndef RECTHREAD_H
#define RECTHREAD_H

#include <QFile>
#include <QAudioInput>
#include <QVariant>
#include <QAudioDeviceInfo>

class WavPcmFile;
class AudioInfo;
class QAudioFormat;

class RecThread : public QObject
{
    Q_OBJECT
public:
    explicit RecThread(QAudioFormat format);
    ~RecThread();
    void run();
    bool isRunning(){ return isRun;}
    QAudioInput * inputAudio;
    void startTest();
    QString getWavFile();

    void switchAutoMod(bool);
    void setOverK(double);
    void setIT1(int);
    void setITstep1(int);
    void setIT2(int);
    void setITstep2(int);
    void setDelay(int);
    QList<QString> getDevices();

    void isNeedToSavaTmpFiles(bool);

protected:
    bool isRun;
    WavPcmFile * m_file;
    AudioInfo * m_audioInfo;
    QAudioFormat m_format;
    bool isTesting;
    QAudioDeviceInfo m_device;
    QList<QAudioDeviceInfo> devices;
    
signals:
    void logging(QString);
    void setVolumeValue(QVariant);
    void setVolumeRange(QVariant);

    void testStopped(int);

    void needToStop();
    
public slots:
    void changeAudioDevice(int);
    void stopRecording();
    void checkRecording();
    void refreshVolumeInfo();

    void stopTest();
    
};

#endif // RECTHREAD_H
