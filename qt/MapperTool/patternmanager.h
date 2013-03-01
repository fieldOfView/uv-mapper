#ifndef PATTERNMANAGER_H
#define PATTERNMANAGER_H

#include <QObject>
#include <QVector>
#include <QtOpenGL>
#include <opencv2/opencv.hpp>
#include "mt_graydecoder.h"
#include "threadSafeQueue.h"

class PatternManager : public QObject
{
    Q_OBJECT
public:
    PatternManager();
    void clearOriginalPatterns();
    //this could be a slot?
    bool loadFiles( QStringList fileNames);
    //bool save( QString fileName = NULL );

    cv::Mat* getMat(int index);
    GLuint getTexture(int index);

public slots:
    void fileLoadFinished();
    void thresholdImages();
    void thresholdImageFinished();

signals:
    void patternSetSizeSet(int index);
    void fileLoaded(int index);
    void originalPatternsLoaded();
    void imgThresholded(int index);
    void originalPatternsThresholded();

private:

    //QVector<qt_grayDecoder*> m_decoders;
    int patternSetSize;
    QVector<QString> m_fileNames;
    QVector<cv::Mat*> m_originalPatterns;
    QVector<cv::Mat*> m_thresholdedPatterns;
    cv::Mat min, max, diff;
    QQueue<QFutureWatcher<cv::Mat*>*> m_mtWatchers;

    ThreadSafeQueue<std::string> m_filesToLoad;
    //QVector<QFuture<cv::Mat*> > grayDecoders;
};

#endif // PATTERNMANAGER_H
