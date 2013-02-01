#ifndef QT_GRAYDECODER_H
#   define QT_GRAYDECODER_H

#include <QThread>
//#include <QString>
#include <QMutex>
#include <QWaitCondition>
#include <threadSafeQueue.h>
#include "mt_graydecoder.h"

class qt_grayDecoder : public QThread
{
    Q_OBJECT
public:
    explicit qt_grayDecoder(ThreadSafeQueue<std::string>& q, QObject *parent = 0);
    mt_grayDecoder decoder;
    QMutex mutex;
    QWaitCondition workScheduled;

signals:
    void readyForDuty(int id);
    void fileLoaded(cv::Mat* file);
    
public slots:
    void prepareFileLoad(std::string filepath);
    void run();

private:
    ThreadSafeQueue<std::string>& queue;
    std::string filepath;
    void loadFile();
};

#endif // QT_GRAYDECODER_H
