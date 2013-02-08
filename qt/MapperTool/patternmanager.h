#ifndef PATTERNMANAGER_H
#define PATTERNMANAGER_H

#include <QObject>
#include <QVector>
#include <QtOpenGL>
#include <opencv2/opencv.hpp>
#include "mt_graydecoder.h"
#include "threadSafeQueue.h"

class PatternManager : QObject
{
    Q_OBJECT
public:
    PatternManager();
    bool loadFiles( QStringList fileNames);
    bool thresholdImages();
    //bool save( QString fileName = NULL );

    GLuint getTexture(int index);

public slots:
    void fileLoaded();


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
