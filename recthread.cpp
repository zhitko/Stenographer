#include "recthread.h"
#include "wavpcmfile.h"
#include "audioinfo.h"

#include <QAudioOutput>
#include <QAudioFormat>
#include <QTextStream>

#include <QDate>
#include <QTime>
#include <QDir>

#include <QTimer>

RecThread::RecThread(QAudioFormat format) :
    QObject(0)
  , isTesting(false)
  , m_file(NULL)
{
    isRun = false;
    this->m_format = format;

    this->m_device = QAudioDeviceInfo::defaultInputDevice();
    this->devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    this->inputAudio = new QAudioInput(m_device, m_format, this);
    this->m_audioInfo  = new AudioInfo(m_format, this);
    connect(m_audioInfo, SIGNAL(update()), SLOT(refreshVolumeInfo()));
    connect(m_audioInfo, SIGNAL(needToStop()), SLOT(stopRecording()));
}

QList<QString> RecThread::getDevices()
{
    QList<QString> * dev = new QList<QString>();
    foreach (const QAudioDeviceInfo &deviceInfo, this->devices)
        dev->append(deviceInfo.deviceName());
    return *dev;
}

void RecThread::changeAudioDevice(int iDev)
{
    this->m_device = this->devices.at(iDev);
    delete this->inputAudio;
    this->inputAudio = new QAudioInput(m_device, m_format, this);
}

RecThread::~RecThread()
{
    delete this->inputAudio;
    delete this->m_audioInfo;
    delete this->m_file;
}

void RecThread::run()
{
    emit logging("<RecThread> Starting...");
    emit setVolumeRange(QVariant(m_audioInfo->getMaxAmplitude()));
    isRun = true;

//    inputAudio = new QAudioInput(m_format, this);
    inputAudio->reset();

    QDir(QDir::currentPath()).mkdir("wav");

    QString fname = "wav"+QString(QDir::separator())+"record_"
            + QDate::currentDate().toString("dd_MM_yy")
            + "_"
            + QTime::currentTime().toString("hh_mm_ss_zzz")
            + ".wav";

    m_audioInfo->setTmpFileName(fname);

    m_file = new WavPcmFile(fname, m_format, this);
    if(m_file->open()) {
        emit logging("<RecThread> Start recoding");
        inputAudio->start(m_audioInfo->start(m_file));

    } else {
        emit logging("<RecThread> Error open file");
        emit logging("<RecThread> Error info: " + m_file->errorString());
    }

    checkRecording();
}

void RecThread::stopRecording(){
    isRun = false;
    emit logging("<RecThread> Stoping...");
}

void RecThread::checkRecording()
{
    if(isRun){
        QTimer::singleShot(1, this, SLOT(checkRecording()));
        return;
    }
    emit logging("<RecThread> Stop recoding");
    // Stops the recording
    inputAudio->stop();
    m_audioInfo->stop();
    m_file->close();
    emit needToStop();
}

void RecThread::refreshVolumeInfo()
{
    qreal value = m_audioInfo->level();
    //emit logging("Amplitude: "+QString::number(value));
    emit setVolumeValue(QVariant(value));
}

void RecThread::startTest()
{
    QTextStream out(stdout);
    out << "Start test\n";
    isTesting = true;
    inputAudio->start(m_audioInfo->start());
    QTimer::singleShot(1000,this, SLOT(stopTest()));
}

void RecThread::stopTest()
{
    QTextStream out(stdout);
    out << "Stop test\n";
    isTesting = false;
    inputAudio->stop();
    m_audioInfo->stop();
    int result = m_audioInfo->calculateNoiseLevel();
    emit testStopped(result);
}

void RecThread::isNeedToSavaTmpFiles(bool is)
{
    m_audioInfo->isNeedSaveTmpFiles(is);
}

void RecThread::switchAutoMod(bool val)
{
    this->m_audioInfo->switchAutoMod(val);
}

QString RecThread::getWavFile()
{
    if ( m_file != NULL)
        return m_file->fileName();
    return "";
}

void RecThread::setOverK(double val)
{
    QTextStream out(stdout);
    out << val << "\n";
    this->m_audioInfo->setOverK(val);
}
