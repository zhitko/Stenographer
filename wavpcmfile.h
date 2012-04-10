#ifndef WAVPCMFILE_H
#define WAVPCMFILE_H

#include <QFile>
#include <QAudioFormat>

class WavPcmFile : public QFile {
public:
  WavPcmFile(const QString & name, const QAudioFormat & format, QObject *parent = 0);
  bool open();
  void close();

private:
  void writeHeader();
  bool hasSupportedFormat();
  QAudioFormat format;
};

#endif // WAVPCMFILE_H
