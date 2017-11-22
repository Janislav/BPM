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
    qInfo() << "Called constuctor";


    qInfo() << "Write a text file";

    qInfo() << "Done write a text file";

    qRegisterMetaType<QAudio::State>("QAudio::State");

    toggle = true;

    QString pwd("");
    char * PWD;
    PWD = getenv ("PWD");
    pwd.append(PWD);

    qInfo() << "Home is: " << QDir::currentPath();

    settings.setCodec("audio/raw");
    settings.setSampleRate(44100);
    settings.setQuality(QMultimedia::HighQuality);

    recorder = new QAudioRecorder(this);

    recorder->setEncodingSettings(settings);

    recorder->setOutputLocation(
                QUrl::fromLocalFile(
                    QStandardPaths::writableLocation(
                        QStandardPaths::HomeLocation).append("/Documents/test.wav")));

    QString selectedInput = recorder->defaultAudioInput();
    qInfo() << "Info: " << selectedInput;

    ui->setupUi(this);
}

BPM::~BPM()
{
    delete ui;
}

void BPM::on_pushButton_clicked()
{
    qInfo() << "Clicked button";
    if(toggle){
        toggle = false;
        qInfo() << "Start recording";
        recorder->record();
    } else {
        toggle = true;
        qInfo() << "Stop recording";
        recorder->stop();
        int bpm = analyseBPM();
        QString sbpm = QString::number(bpm);
        //QString* bpmstr = new QString(std::to_string(bpm));

        qInfo() << "BPM FINAL: " << sbpm;

        ui->lcdNumber->display(sbpm);
    }
}

int BPM::analyseBPM(){
    qInfo() << "BPM::analyseBPM() -> call function";

    QString path = QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/Documents/test.wav")).toString();
    std::string str = path.toStdString();
    str = str.erase(0,6);
    const char_t * p = str.c_str();

    qInfo() << "Path is: " << p;

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

    qInfo() << "BPM: " << bpmSum/counter;

    return bpmSum/counter;
}

void BPM::processBuffer(const QAudioBuffer& buffer) {

}
