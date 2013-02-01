#ifndef PATTERNMANAGER_H
#define PATTERNMANAGER_H

#include <QObject>
#include <QVector>
#include <QtOpenGL>
#include <opencv2/opencv.hpp>
#include "qt_graydecoder.h"
#include "threadSafeQueue.h"

class PatternManager : QObject
{
    Q_OBJECT
public:
    PatternManager();
    bool loadFiles( QString fileName = NULL );
    //bool save( QString fileName = NULL );

    GLuint getTexture(int index);

public slots:
    void fileLoaded(cv::Mat* file);


private:

    QVector<qt_grayDecoder*> m_decoders;
    QVector<QString> m_fileNames;
    QVector<cv::Mat*> m_originalPatterns;
    QVector<cv::Mat*> m_thresholdedPatterns;
    ThreadSafeQueue<std::string> m_filesToLoad;
};

#endif // PATTERNMANAGER_H
