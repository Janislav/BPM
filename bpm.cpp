#include <QAudioProbe>
#include <QMultimedia>
#include <QAudioRecorder>
#include <QAudio>
#include <aubio/aubio.h>

#include "bpm.h"
#include "ui_bpm.h"

BPM::BPM(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BPM)
{
    qRegisterMetaType<QAudio::State>("QAudio::State");

    toggle = true;

    QUrl fileUrl = QUrl::fromLocalFile(
        QStandardPaths::writableLocation(
            QStandardPaths::HomeLocation).append("/Documents/record.wav"));


    timer = new QTimer();

    timer->setInterval(2000);

    connect(timer, &QTimer::timeout, this, [=](){
        qInfo() << "Checking BPM now";
        recorder->stop();
        int bpm = analyseBPM();
        qInfo() << "Current BPM: " << bpm;
        if((bpm > 145) || (bpm < 90)){
            qInfo() << "BPM is wrong, try it again! ";
            recorder->record();
        } else {
            QString sbpm = QString::number(bpm);
            qInfo() << "BPM is right! ";
            ui->lcdNumber->display(sbpm);
            toggle = true;
            ui->pushButton->setDisabled(false);
            timer->stop();
        }
    });

    settings.setCodec("audio/raw");
    settings.setSampleRate(44100);
    settings.setQuality(QMultimedia::HighQuality);

    recorder = new QAudioRecorder(this);

    recorder->setEncodingSettings(settings);

    recorder->setOutputLocation(fileUrl);
    ui->setupUi(this);
}

BPM::~BPM()
{
    delete ui;
}

void BPM::on_pushButton_clicked()
{
    if(toggle){
        toggle = false;
        ui->pushButton->setDisabled(true);
        timer->start(2000);
        recorder->record();
    } else {
        toggle = true;
        //recorder->stop();
        //int bpm = analyseBPM();
        //QString sbpm = QString::number(bpm);
        //ui->lcdNumber->display(sbpm);
    }
}

int BPM::analyseBPM(){
    qInfo() << "BPM::analyseBPM() -> call function";

    QString path = QUrl::fromLocalFile(
                QStandardPaths::writableLocation(
                    QStandardPaths::HomeLocation).append("/Documents/record.wav")).toString();

    std::string str = path.toStdString();
    str = str.erase(0,6);
    const char_t * p = str.c_str();

    aubio_source_t * source = new_aubio_source(p ,44100, 256);

    if(!source){
        qInfo() << "File dosen't exist!";
        return 0;
    }

    uint_t samplerate = 44100;
    uint_t win_size = 1024;
    uint_t hop_size = win_size / 4;

    uint_t n_frames = 0, read = 0;

    fvec_t * in = new_fvec (hop_size); // input audio buffer
    fvec_t * out = new_fvec (1); // output position

    aubio_tempo_t * o = new_aubio_tempo("default", win_size, hop_size, samplerate);
    int counter = 0;
    float bpmSum = 0;

    do {
        // put some fresh data in input vector
        aubio_source_do(source, in, &read);
        // execute tempo
        aubio_tempo_do(o,in,out);
        // do something with the beats
        if (out->data[0] != 0) {
            bpmSum = bpmSum + aubio_tempo_get_bpm(o);
            counter++;
        }
        n_frames += read;
    } while ( read == hop_size );

    // clean up memory
    del_aubio_tempo(o);
    del_fvec(in);
    del_fvec(out);
    del_aubio_source(source);

    return bpmSum/counter;
}

void BPM::processBuffer(const QAudioBuffer& buffer) {

}
