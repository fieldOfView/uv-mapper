#include "patternmanager.h"
#include <QVectorIterator>
#include <QStringListIterator>
#include <QFuture>

PatternManager::PatternManager()
{
    /*QVectorIterator<QString> it(m_fileNames);
    while(it.hasNext()) {

        m_filesToLoad.enqueue(it.next().toStdString());
    }*/
}

void PatternManager::clearOriginalPatterns() {

    QVectorIterator<cv::Mat*> it(m_originalPatterns);
    while ( it.hasNext() ) {

        delete it.next();
    }
    m_originalPatterns.clear();
}

bool PatternManager::loadFiles(QStringList fileNames) {

    if (!m_mtWatchers.isEmpty()) {
        qDebug("Error: the watchers queue is not empty!");
        return false;
    }

    emit patternSetSizeSet(fileNames.size());
    QStringListIterator it(fileNames);
    while ( it.hasNext() ) {
        QString fileName = it.next();
        QFutureWatcher<cv::Mat*>* mtWatcher = new QFutureWatcher<cv::Mat*>();
        //this doesn't work because of the while loop at the end
        QObject::connect(mtWatcher, SIGNAL(finished()), this, SLOT(fileLoadFinished()));

        //class method only works with internal member methods (not static so you need an instance)
        //see thresholdImage
        //mt_grayDecoder decoder;// = new mt_grayDecoder();
        //QFuture<cv::Mat*> fileLoader = QtConcurrent::run(decoder, &mt_grayDecoder::loadFile, fileName.toStdString());

        //static method only works with static methods in the class(simple)
        QFuture<cv::Mat*> fileLoader = QtConcurrent::run(mt_grayDecoder::loadFile, fileName.toStdString());

        mtWatcher->setFuture(fileLoader);
        //m_mtWatchers.enqueue(mtWatcher);
        qDebug() << "file "<< fileName << " loading";
    }

    //This while loop is blocking any signalling QT wants to do so no Signals are parsed
    /*while(!m_mtWatchers.isEmpty()) {
        QFutureWatcher<cv::Mat*>* watch = m_mtWatchers.dequeue();
        qDebug() << watch->isRunning();
        cv::Mat* t = watch->result();
        this->m_originalPatterns.push_back(watch->result());
        qDebug("file loaded");
        delete watch;
    }*/
    return true;
}

cv::Mat* PatternManager::getMat(int index) {

    return m_originalPatterns.at(index);
}

void PatternManager::fileLoadFinished() {//cv::Mat *file) {
    //this->m_originalPatterns.push_back(file);
    qDebug("file loaded signal");
    //QProgressBar* watcher2 = qobject_cast<QProgressBar*>(QObject::sender());
    QFutureWatcher<cv::Mat*>* watcher = static_cast<QFutureWatcher<cv::Mat*>*>(QObject::sender());
    this->m_originalPatterns.push_back(watcher->result());
    //Do I need to delete the watcher now???
    delete watcher;
    emit fileLoaded(this->m_originalPatterns.size());
}

bool PatternManager::thresholdImages()
{
    qDebug() << "img thresholding";
    if (!m_mtWatchers.isEmpty()) {
        qDebug("Error: the watchers queue is not empty!");
        return false;
    }

    QVectorIterator<cv::Mat*> it(m_originalPatterns);
    while ( it.hasNext() ) {
        cv::Mat* origPtr = it.next();
        QFutureWatcher<cv::Mat*>* mtWatcher = new QFutureWatcher<cv::Mat*>();
        //QObject::connect(mtWatcher, SIGNAL(finished)), this, SLOT(fileLoaded(cv::Mat*))
        mt_grayDecoder decoder;
        QFuture<cv::Mat*> thresholder = QtConcurrent::run(decoder, &mt_grayDecoder::thresholdImage, origPtr);
        mtWatcher->setFuture(thresholder);
        m_mtWatchers.enqueue(mtWatcher);
        qDebug() << "img thresholding";
    }

    while(!m_mtWatchers.isEmpty()) {
        QFutureWatcher<cv::Mat*>* watch = m_mtWatchers.dequeue();
        this->m_thresholdedPatterns.push_back(watch->result());
        qDebug("Image thresholded");
        delete watch;
    }
    return true;
}

GLuint PatternManager::getTexture(int index)
{
    /*
     * convert cv::Mat to openGL texture
     */
    GLuint texture;
    GLenum type, format;

    cv::Mat* map = m_originalPatterns.at(index);

    switch (map->depth()) {
    case CV_8U:
        format = GL_UNSIGNED_BYTE;
        break;
    case CV_16U:
        format = GL_UNSIGNED_SHORT;
        break;
    }

    switch (map->channels()) {
    case 3:
        type = GL_BGR_EXT;
        break;
    case 4:
        type = GL_BGRA_EXT;
        break;
    }

    GLint glFormat = (type == GL_BGR_EXT)?
                ((format == GL_UNSIGNED_BYTE)?GL_RGB8:GL_RGB16):
                ((format == GL_UNSIGNED_BYTE)?GL_RGBA8:GL_RGBA16);

    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexImage2D( GL_TEXTURE_2D, 0, glFormat, map->cols, map->rows, 0, type, format, map->data);
    return texture;
}
