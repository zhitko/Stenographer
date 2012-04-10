#include "recthread.h"
#include "wavpcmfile.h"

#include <QAudioOutput>
#include <QAudioFormat>

#include <QTimer>

RecThread::RecThread(QObject *parent) :
    QObject(parent)
{
    isRun = false;
}

void RecThread::run(QAudioFormat format)
{
    emit logging("<RecThread> Starting...");
    isRun = true;

    inputAudio = new QAudioInput(format, this);
//    QAudioOutput* outputAudio = new QAudioOutput(format, this);
//    inputAudio->start(outputAudio->start());

    m_file = new WavPcmFile("Filename.wav", inputAudio->format(), this);
    if(m_file->open()) {
        inputAudio->start(m_file);
        emit logging("<RecThread> Start recoding");
    } else {
        emit logging("<RecThread> Error open file");
        emit logging("<RecThread> Error info: " + m_file->errorString());
    }

    checkRecording();
}

QFile * RecThread::stopRecording(){
    emit logging("<RecThread> Stoping...");
    isRun = false;
    return m_file;
}

void RecThread::checkRecording()
{
    if(isRun){
        emit logging("<RecThread> Recoding");
        QTimer::singleShot(3, this, SLOT(checkRecording()));
        return;
    }
    emit logging("<RecThread> Stop recoding");
    // Stops the recording
    inputAudio->stop();
    m_file->close();
    delete inputAudio;
}
