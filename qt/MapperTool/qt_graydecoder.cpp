#include "qt_graydecoder.h"

qt_grayDecoder::qt_grayDecoder(ThreadSafeQueue<std::string>& q, QObject *parent) :
    QThread(parent), queue(q)
{
    this->decoder = mt_grayDecoder();
    //this->queue = q;
}

void qt_grayDecoder::run() {

    qDebug("waiting %d", this->currentThreadId());
    while (!this->queue.isEmpty()) {
        qDebug("wakker %d", this->currentThreadId());
        /*switch ( decoder.state ) {
            case mt_grayDecoder::MTGD_STATE_LOADFILE/home/arnaud/Downloads/1 - raw photos/_DSC3899.tifS:
                loadFile();
                break;
            case 2:
                qDebug("nlanla");
                break;
            default:
                qDebug("niks te doen");
        }
        qDebug("Going to sleep");
        mutex.lock();
        workScheduled.wait(&mutex);
        mutex.unlock();
        */
        std::string file = this->queue.dequeue();
        cv::Mat* load = decoder.loadFile(file);
        qDebug(file.c_str());
        emit fileLoaded(load);
    }
}

void qt_grayDecoder::prepareFileLoad(std::string filepath) {

    qDebug("Prepare fileload %s", filepath.c_str());
    this->filepath = filepath;
    decoder.setState(decoder.MTGD_STATE_LOADFILES);
    workScheduled.wakeAll();
    //this->yieldCurrentThread();
}

void qt_grayDecoder::loadFile() {

    cv::Mat* load = decoder.loadFile(filepath);
    //qDebug("%i", load->cols);
    emit fileLoaded(load);
}
