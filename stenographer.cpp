#include "stenographer.h"
#include "wavpcmfile.h"
#include "recthread.h"
#include "googlespeech.h"
#include "downloadandplay.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QUrl>
#include <QTranslator>

Stenographer::Stenographer(QObject *parent) :
    QObject(parent)
{
    this->lastWavFile = "";
    this->m_file = NULL;
    m_autoMod = true;
    m_isActive = false;
    m_saveTmpFiles = false;

    this->downloadAndPlay = new DownloadAndPlay();

    this->googleSpeech = new GoogleSpeech();
    QObject::connect(googleSpeech,SIGNAL(getText(QString)),this,SLOT(addRecText(QString)));
    QObject::connect(googleSpeech,SIGNAL(logging(QString)),this,SLOT(logging(QString)));

    //QAudioFormat format;
    format.setFrequency(16000);
    format.setChannels(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    recThread = new RecThread(format);
    recThread->isNeedToSavaTmpFiles(m_saveTmpFiles);
    connect(recThread, SIGNAL(logging(QString)), this, SLOT(logging(QString)));
    connect(recThread, SIGNAL(setVolumeRange(QVariant)), this, SLOT(getVolumeRange(QVariant)));
    connect(recThread, SIGNAL(setVolumeValue(QVariant)), this, SLOT(getVolumeValue(QVariant)));
    connect(recThread, SIGNAL(needToStop()), this, SLOT(stopRec()));

    connect(recThread, SIGNAL(testStopped(int)), this, SLOT(_testStopped(int)));

    recThread->switchAutoMod(m_autoMod);

    outputAudio = new QAudioOutput(format, this);
    QObject::connect(outputAudio,SIGNAL(stateChanged(QAudio::State)),this,SLOT(closeFile()));
}

void Stenographer::getOptions()
{
    emit setAudioDevices(QVariant(recThread->getDevices()));
}

void Stenographer::changeAudioDevice(QVariant val){
    int iDev = val.toInt();
    this->recThread->changeAudioDevice(iDev);
}

void Stenographer::getVolumeRange(QVariant data)
{ emit setVolumeRange(data); }

void Stenographer::getVolumeValue(QVariant data)
{ emit setVolumeValue(data); }

void Stenographer::stopRec()
{
    this->lastWavFile = this->recThread->getWavFile();
    if(!m_autoMod)
        emit needToStop();
    else{
        if(!this->m_isActive) return;
        this->convertAndSent();
        this->beginStopRec();
        this->m_isActive = true;
    }
}

void Stenographer::beginStopRec()
{
    if(recThread->isRunning())
    {
        emit recThread->stopRecording();
        this->lastWavFile = this->recThread->getWavFile();
        emit logging("<Stenographer> Recorded file name: " + this->lastWavFile);
        while(recThread->isRunning()){;}
    }
    if(!this->m_isActive || this->m_autoMod)
    {
        recThread->run();
    }
    this->m_isActive = !this->m_isActive;
}

void Stenographer::playLast(QVariant text)
{
    QString args("q="+text.toString().trimmed().replace(" ","+"));
    QUrl url("http://translate.google.com/translate_tts?"+args);
    emit logging("<DownloadAndPlay> " + args);
    this->downloadAndPlay->doDownload(url);
}

void Stenographer::closeFile()
{
    if(this->outputAudio->state() == QAudio::StoppedState){
        outputAudio->stop();
        m_file->close();
    }
    //delete m_file;
}

void Stenographer::convertAndSent()
{
    emit logging("<Stenographer> Begin convert");
    googleSpeech->sent(lastWavFile);
}

void Stenographer::setSaveTempFiles(QVariant _is)
{
    m_saveTmpFiles = _is.toBool();
    recThread->isNeedToSavaTmpFiles(m_saveTmpFiles);
}

void Stenographer::addRecText(QString data)
{
    if(m_saveTmpFiles){
        QFile file(this->lastWavFile.replace(".wav",".txt"));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
            file.write(data.toUtf8());
            file.close();
        }
    }
    emit updateData(QVariant(QObject::tr(data.toStdString().c_str())));
}

void Stenographer::saveText(QVariant data, QVariant name)
{
    QString text = data.toString();
    QString f_name = name.toString();
    QFile file(f_name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        file.write(text.toUtf8());
        file.close();
    }
}

void Stenographer::logging(QString str)
{
    emit updateLog(QVariant(QObject::tr(str.toStdString().c_str())));
}

void Stenographer::startTest()
{
    this->recThread->startTest();
}

void Stenographer::_testStopped(int result)
{
    emit testStopped(QVariant(result));
}

void Stenographer::switchAutoMod()
{
    m_autoMod = !m_autoMod;
    emit logging(QString("<Stenographer> Auto mod: ") + (m_autoMod?"on":"off"));
    this->recThread->switchAutoMod(m_autoMod);
}

void Stenographer::setOverK(QVariant val)
{
    emit logging(QString("<Stenographer> Factor is: ") + QString::number(val.toDouble()));
    this->recThread->setOverK(val.toDouble());
}

void Stenographer::setIT1(QVariant val)
{
    emit logging(QString("<Stenographer> First integration time is: ") + QString::number(val.toInt()));
    this->recThread->setIT1(val.toInt());
}

void Stenographer::setITstep1(QVariant val)
{
    emit logging(QString("<Stenographer> First integration step is: ") + QString::number(val.toInt()));
    this->recThread->setITstep1(val.toInt());
}

void Stenographer::setIT2(QVariant val)
{
    emit logging(QString("<Stenographer> Second integration time is: ") + QString::number(val.toInt()));
    this->recThread->setIT2(val.toInt());
}

void Stenographer::setITstep2(QVariant val)
{
    emit logging(QString("<Stenographer> Second integration step is: ") + QString::number(val.toInt()));
    this->recThread->setITstep2(val.toInt());
}

void Stenographer::setDelay(QVariant val)
{
    emit logging(QString("<Stenographer> Delay is: ") + QString::number(val.toInt()));
    this->recThread->setDelay(val.toInt());
}
