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
    bool loadFiles( QStringList fileNames);
    bool thresholdImages();
    //bool save( QString fileName = NULL );

    cv::Mat* getMat(int index);
    GLuint getTexture(int index);

public slots:
    void fileLoadFinished();

signals:
    void fileLoaded(int index);
    void patternSetSizeSet(int index);

private:

    //QVector<qt_grayDecoder*> m_decoders;
    QVector<QString> m_fileNames;
    QVector<cv::Mat*> m_originalPatterns;
    QVector<cv::Mat*> m_thresholdedPatterns;
    QQueue<QFutureWatcher<cv::Mat*>*> m_mtWatchers;

    ThreadSafeQueue<std::string> m_filesToLoad;
    //QVector<QFuture<cv::Mat*> > grayDecoders;
};

#endif // PATTERNMANAGER_H
