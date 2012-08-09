#include <stdlib.h>
#include <math.h>

#include <QAudioDeviceInfo>
#include <QAudioInput>

#include <QFile>
#include <QDir>

#include <QBuffer>

#include <QtCore/qendian.h>
#include <QDataStream>

#include "audioinfo.h"
#include "wavpcmfile.h"

#include "QTextStream"

AudioInfo::AudioInfo(const QAudioFormat &format, QObject *parent)
    :   QIODevice(parent)
    ,   m_format(format)
    ,   m_maxAmplitude(0)
    ,   m_level(0.0)
    ,   m_isNeedSaveFile(true)
    ,   m_isNeedSaveTmpFiles(false)
    ,   underlyingDevice(NULL)
    ,   criticalValue(5000)
    ,   isCriticalOver(false)
    ,   kOverValue(1.5)
    ,   m_isAuto(true)

{
    setArgs();
    switch (m_format.sampleSize()) {
    case 8:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 32767;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

AudioInfo::~AudioInfo()
{
    delete firstStepValues;
    delete secondStepValues;
    delete bufferedValues;
    delete firstStepValuesBuf;

}

QIODevice * AudioInfo::start()
{
    open(QIODevice::WriteOnly);
    m_isNeedSaveFile = false;
    this->clearBuffers();
    return this;
}

QIODevice * AudioInfo::start(QIODevice * dev)
{
    open(QIODevice::WriteOnly);
    m_isNeedSaveFile = true;
    this->underlyingDevice = dev;
    this->clearBuffers();
    return this;
}

void AudioInfo::stop()
{
    close();
    if (m_isNeedSaveTmpFiles)
        saveToFile();
}

qint64 AudioInfo::readData(char *data, qint64 maxlen)
{
    return 0;
}

qint64 AudioInfo::writeData(const char *data, qint64 len)
{
    QTextStream out(stdout);
    if(m_isNeedSaveFile && (isCriticalOver || !m_isAuto)){
        if(this->tempBuff->size()>0)
            while(!this->tempBuff->isEmpty()){
                QByteArray tmp = this->tempBuff->dequeue();
                this->underlyingDevice->write(tmp.data(),tmp.size());
            }
        this->underlyingDevice->write(data, len);
    }else if(m_isNeedSaveFile){
        this->tempBuff->enqueue(QByteArray(data, len));
        if(this->tempMaxBuff < this->tempBuff->length()){
            this->tempBuff->dequeue();
        }
    }

    if (m_maxAmplitude) {
        Q_ASSERT(m_format.sampleSize() % 8 == 0);
        const int channelBytes = m_format.sampleSize() / 8;
        const int sampleBytes = m_format.channels() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const int numSamples = len / sampleBytes;

        quint16 maxValue = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

        for (int i = 0; i < numSamples; ++i) {
            for(int j = 0; j < m_format.channels(); ++j) {
                quint16 value = 0;

                if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                }

                maxValue = qMax(value, maxValue);
                calculate(qAbs((int) value));
                ptr += channelBytes;
            }
        }
        maxValue = qMin(maxValue, m_maxAmplitude);
        m_level = qreal(maxValue) / m_maxAmplitude;
        m_level = maxValue;
    }

    emit update();
    return len;
}

void AudioInfo::recalc()
{
    //    double firstPeriodSec = 0.008;
    //    double firstPeriodStepSec = 0.004;
    //    double secondPeriodSec = 0.4;
    //    double secondPeriodStepSec = 0.016;
        double freq = 16.0;        // kHz
        double dataSize = 1280.0;  // Data per read/write action
        this->tempMaxBuff = (double)kDelay*freq/dataSize;
        double firstPeriodSec = (double)kIT1/1000;
        double firstPeriodStepSec = (double)kITstep1/1000;
        double secondPeriodSec = (double)kIT2/1000;
        double secondPeriodStepSec = (double)kITstep2/1000;
        this->firstPeriod = firstPeriodSec * m_format.frequency();          // for 16kHz is 128
        this->firstPeriodStep = firstPeriodStepSec * m_format.frequency();  // for 16kHz is 64
        this->secondPeriod = secondPeriodSec / firstPeriodStepSec;          // for 250Hz is 100
        this->secondPeriodStep = secondPeriodStepSec / firstPeriodStepSec;  // for 250Hz is 2
        QTextStream out(stdout);
        out << "firstPeriod " << this->firstPeriod << "\n";
        out << "firstPeriodStep " << this->firstPeriodStep << "\n";
        out << "secondPeriod " << this->secondPeriod << "\n";
        out << "secondPeriodStep " << this->secondPeriodStep << "\n";
}

void AudioInfo::setArgs()
{
    kIT1 = 8;
    kITstep1 = 4;
    kIT2 = 400;
    kITstep2 = 160;
    kDelay = 1000;
    recalc();
    this->firstStepValues = new QVector<int>();
    this->firstStepValuesBuf = new QVector<int>();
    this->secondStepValues = new QVector<int>();
    this->bufferedValues = new QVector<int>();
    this->tempBuff = new QQueue<QByteArray>();
}

void AudioInfo::calculate(int value)
{
//    QTextStream out(stdout);
//    out << value << "\n";
    this->counter1 ++;
    this->bufferedValues->append(value);
    if (this->counter1 >= this->firstPeriodStep)
    {
        this->counter1 = 0;
        if (this->bufferedValues->count() >= this->firstPeriod)
        {
            // calculate first value
            long value1 = 0;
            for( int i=0 ; i<this->firstPeriod; i++ )
                value1 += this->bufferedValues->at(i);
            // remove first elements
            this->bufferedValues->remove(0,this->firstPeriodStep);
            // add calculated value to vector <1>
            value1 = value1 * 3 / this->firstPeriod;
            //out << value1 << "\n";
            if(value1 > m_maxAmplitude){
                this->firstStepValues->append( m_maxAmplitude );
                this->firstStepValuesBuf->append( m_maxAmplitude );
            }else{
                this->firstStepValues->append( value1 );
                this->firstStepValuesBuf->append( value1 );
            }
            this->counter2 ++;
            if(this->counter2 >= this->secondPeriodStep)
            {
                this->counter2 = 0;
                if(this->firstStepValues->count() >= this->secondPeriod)
                {
                    long value2 = 0;
                    // calculate second value
                    for(int i=0; i<this->secondPeriod; i++)
                        value2 += this->firstStepValues->at(i);
                    this->firstStepValues->remove(0,this->secondPeriodStep);
                    value2 = value2/this->secondPeriod;

                    if (m_isAuto){
                        if (value2 > criticalValue){
                            isCriticalOver = true;
                        }else
                            if(isCriticalOver)
                            {
                                emit needToStop();
                                isCriticalOver = false;
                            }
                    }

                    //out << value2 << "\n";

                    if(value2 > m_maxAmplitude)
                        this->secondStepValues->append( m_maxAmplitude );
                    else
                        this->secondStepValues->append( value2 );
                }
            }
        }
    }
}

void AudioInfo::saveToFile()
{
    QString fname1 = this->tmpFileName.replace(".wav","_step1.wav");
    WavPcmFile pcmFile(fname1, m_format, this);
    pcmFile.open();
    QDataStream out(&pcmFile);
    out.setByteOrder(QDataStream::LittleEndian);
    for(int i=0; i<this->firstStepValuesBuf->count(); i++)
        out << quint16(this->firstStepValuesBuf->at(i));
    pcmFile.close();

    QString fname2 = this->tmpFileName.replace("_step1.wav","_step2.wav");
    WavPcmFile pcmFile2(fname2, m_format, this);
    pcmFile2.open();
    QDataStream out2(&pcmFile2);
    out2.setByteOrder(QDataStream::LittleEndian);
    for(int i=0; i<this->secondStepValues->count(); i++)
        out2 << quint16(this->secondStepValues->at(i));
    pcmFile2.close();
}

void AudioInfo::clearBuffers()
{
    this->firstStepValues->clear();
    this->firstStepValuesBuf->clear();
    this->secondStepValues->clear();
    this->bufferedValues->clear();
    this->counter1 = 0;
    this->counter2 = 0;
    this->isCriticalOver = false;
}

int AudioInfo::calculateNoiseLevel()
{
    int noiseLevel = 0;
    for(int i=0; i<this->secondStepValues->count(); i++)
        noiseLevel += this->secondStepValues->at(i);
    criticalValue = (double)noiseLevel * kOverValue / this->secondStepValues->count();
    noiseLevel = noiseLevel/this->secondStepValues->count();
    QTextStream out(stdout);
    out << "noiseLevel " << noiseLevel << "\n";
    out << "criticalValue " << criticalValue << "\n";
    return noiseLevel;
}
