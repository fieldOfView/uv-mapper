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
    void imageThresholded(cv::Mat* file);
    void extremeMinPixelsFound(cv::Mat* extremeMinPixelsImgPtr);
    void extremeMaxPixelsFound(cv::Mat* extremeMaxPixelsImgPtr);
    void ThresholdedImagesGraycoded(cv::Mat* graycode);

public slots:
    void prepareFileLoad(std::string filepath);
    void prepareImageThreshold(cv::Mat* imgPtr);
    void prepareExtremePixelsFind(std::vector<cv::Mat*>* originalsPtr);
    void prepareThresholdedImagesToGraycode(std::vector<cv::Mat*>* thresholdedPtr);
    void run();

private:
    ThreadSafeQueue<std::string>& queue;
    //these are all set by the prepare methods
    std::string filepath;
    cv::Mat* imgPtr;
    std::vector<cv::Mat*>* originalsPtr;
    std::vector<cv::Mat*>* thresholdedPtr;

    void loadFile();
};

#endif // QT_GRAYDECODER_H
