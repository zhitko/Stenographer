#ifndef RECTHREAD_H
#define RECTHREAD_H

#include <QFile>
#include <QAudioInput>

class WavPcmFile;

class RecThread : public QObject
{
    Q_OBJECT
public:
    explicit RecThread(QObject *parent = 0);
    void run(QAudioFormat);
    bool isRunning(){ return isRun;}

protected:
    bool isRun;
    QAudioInput * inputAudio;
    WavPcmFile * m_file;
    
signals:
    void logging(QString);
    
public slots:
    QFile * stopRecording();
    void checkRecording();
    
};

#endif // RECTHREAD_H
