#ifndef AUDIOINFO_H
#define AUDIOINFO_H

#include <QIODevice>
#include <QAudioFormat>
#include <QVector>
#include <QQueue>

class AudioInfo : public QIODevice
{
    Q_OBJECT
public:
    AudioInfo(const QAudioFormat &format, QObject *parent);
    ~AudioInfo();

    QIODevice * start(QIODevice *);
    QIODevice * start();
    void stop();

    qreal level() const { return m_level; }

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

    quint16 getMaxAmplitude(){return m_maxAmplitude;}

    void isNeedSaveTmpFiles(bool val){ m_isNeedSaveTmpFiles=val; }
    void switchAutoMod(bool val){ m_isAuto = val; }
    void setOverK(double val){ kOverValue = val;}
    void recalc();
    void setIT1(int val){ kIT1 = val; recalc();}
    void setITstep1(int val){ kITstep1 = val; recalc();}
    void setIT2(int val){ kIT2 = val; recalc();}
    void setITstep2(int val){ kITstep2 = val; recalc();}
    void setDelay(int val){ kDelay = val; recalc();}

    int calculateNoiseLevel();

    void setTmpFileName(QString fname){ this->tmpFileName = fname;}

private:  
    void setArgs();
    bool m_isNeedSaveTmpFiles;
    void saveToFile();
    void calculate(int);
    bool isCriticalOver;
    int criticalValue;
    double kOverValue;
    int counter1, counter2;
    int kIT1, kITstep1, kIT2, kITstep2, kDelay;
    int firstPeriod, secondPeriod;
    int firstPeriodStep, secondPeriodStep;
    QVector<int> * firstStepValues;
    QVector<int> * secondStepValues;
    QVector<int> * bufferedValues;
    QVector<int> * firstStepValuesBuf;
    void clearBuffers();
    int tempMaxBuff;
    QQueue<QByteArray> * tempBuff;

    bool m_isAuto;

    bool m_isNeedSaveFile;
    QIODevice * underlyingDevice;

    const QAudioFormat m_format;
    quint16 m_maxAmplitude;
    qreal m_level; // 0.0 <= m_level <= 1.0

    QString tmpFileName;

signals:
    void update();
    void needToStop();
};

#endif // AUDIOINFO_H
