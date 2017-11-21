#ifndef BPM_H
#define BPM_H

#include <QMainWindow>
#include <QtMultimedia>
#include <QAudio>

namespace Ui {
class BPM;
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;
}

class BPM : public QMainWindow
{
    Q_OBJECT

public:
    explicit BPM(QWidget *parent = 0);
    ~BPM();

private slots:
    void processBuffer(const QAudioBuffer&);
    void on_pushButton_clicked();

private:
    Ui::BPM *ui;
    QAudioEncoderSettings settings;
    QAudioRecorder * recorder;
    bool toggle;
    int analyseBPM();
};

#endif // BPM_H
