#include "stenographer.h"
#include "wavpcmfile.h"
#include "recthread.h"
#include "googlespeech.h"

Stenographer::Stenographer(QObject *parent) :
    QObject(parent)
{
    this->lastWavFile = NULL;
    this->recThread = NULL;

    googleSpeech = new GoogleSpeech();
    QObject::connect(googleSpeech,SIGNAL(getText(QString)),this,SLOT(addRecText(QString)));
    QObject::connect(googleSpeech,SIGNAL(logging(QString)),this,SLOT(logging(QString)));

    //QAudioFormat format;
    format.setFrequency(16000);
    format.setChannels(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
}

void Stenographer::beginStopRec()
{
    if(recThread == NULL)
    {
        recThread = new RecThread();
        connect(recThread, SIGNAL(logging(QString)), this, SLOT(logging(QString)));
    }
    if(recThread->isRunning())
    {
        this->lastWavFile = emit recThread->stopRecording();
        emit logging("<Stenographer> Recorded file name: " + this->lastWavFile->fileName());
    }else
    {
        recThread->run(format);
    }
}

void Stenographer::playLast()
{
    if(this->lastWavFile != NULL)
    {
        if(lastWavFile->isOpen())
        {
            emit closeFile();
            return;
        }
        emit logging("<Stenographer> Played file name: " + this->lastWavFile->fileName());
        //QSound(lastWavFile).play();
        lastWavFile->open(QFile::ReadWrite);
        outputAudio = new QAudioOutput(format, this);
        outputAudio->start(lastWavFile);
        QObject::connect(outputAudio,SIGNAL(stateChanged(QAudio::State)),this,SLOT(closeFile()));
    }
}

void Stenographer::closeFile()
{
    lastWavFile->close();
    delete outputAudio;
}

void Stenographer::convertAndSent()
{
    emit logging("<Stenographer> Begin convert");
    googleSpeech->sent(lastWavFile);
}

void Stenographer::addRecText(QString data)
{
    //*log << QObject::tr("тест") << endl;
    //*log << QObject::tr(QString::fromUtf8(data.toAscii()).toStdString().c_str()) << endl;
    //*log << data << endl;
    //*log << data.toUtf8() << endl;
    //*log << data.toAscii() << endl;
    //*log << "<Stenographer> Get string: " << data << endl;
    emit updateData(QVariant(QObject::tr(data.toStdString().c_str())));
}

void Stenographer::logging(QString str)
{
    emit updateLog(QVariant(QObject::tr(str.toStdString().c_str())));
}
