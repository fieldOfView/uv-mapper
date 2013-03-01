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
    //clear current contents
    clearOriginalPatterns();

    //save the size of the list of filenames
    patternSetSize = fileNames.size();
    emit patternSetSizeSet(fileNames.size());

    //iterate the filenames
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
    return true;
}

cv::Mat* PatternManager::getMat(int index) {

    if(index < m_thresholdedPatterns.size()) {
        qDebug() <<"return thres" << m_thresholdedPatterns.at(index)->at<uint>(100);
        return m_thresholdedPatterns.at(index);
    }
    return m_originalPatterns.at(index);
}

void PatternManager::fileLoadFinished()
{
    qDebug("file loaded signal");
    //QProgressBar* watcher2 = qobject_cast<QProgressBar*>(QObject::sender());
    QFutureWatcher<cv::Mat*>* watcher = static_cast<QFutureWatcher<cv::Mat*>*>(QObject::sender());
    this->m_originalPatterns.push_back(watcher->result());
    //Do I need to delete the watcher now???
    delete watcher;
    emit fileLoaded(this->m_originalPatterns.size());
    if(this->m_originalPatterns.size() == patternSetSize) {
        emit originalPatternsLoaded();
    }
}

void PatternManager::thresholdImages()
{
    qDebug() << "img thresholding";
    if (!m_mtWatchers.isEmpty()) {
        qDebug("Error: the watchers queue is not empty!");
        return;
    }
    //Fix me this synchronous logic and not threadsafe...
    mt_grayDecoder decoder2;
    std::vector<cv::Mat*> orgStdVec = m_originalPatterns.toStdVector();
    min = *decoder2.findExtremeMinPixels(&orgStdVec);
    max = *decoder2.findExtremeMaxPixels(&orgStdVec);
    diff = *decoder2.findExtremeMinMaxDiffPixels(min, max);

    QVectorIterator<cv::Mat*> it(m_originalPatterns);
    while ( it.hasNext() ) {
        cv::Mat* origPtr = it.next();
        QFutureWatcher<cv::Mat*>* mtWatcher = new QFutureWatcher<cv::Mat*>();
        //QObject::connect(mtWatcher, SIGNAL(finished)), this, SLOT(fileLoaded(cv::Mat*))
        QObject::connect(mtWatcher, SIGNAL(finished()), this, SLOT(thresholdImageFinished()));

        mt_grayDecoder decoder;
        QFuture<cv::Mat*> thresholder = QtConcurrent::run(decoder, &mt_grayDecoder::thresholdImage, origPtr, min, diff);
        mtWatcher->setFuture(thresholder);
        //m_mtWatchers.enqueue(mtWatcher);
        qDebug() << "img thresholding";
    }
}

void PatternManager::thresholdImageFinished() {
    QFutureWatcher<cv::Mat*>* watcher = static_cast<QFutureWatcher<cv::Mat*>*>(QObject::sender());
    this->m_thresholdedPatterns.push_back(watcher->result());
    qDebug() << "Image thresholded " << watcher->result()->depth();
    delete watcher;
    emit imgThresholded(this->m_thresholdedPatterns.size());
    if(this->m_thresholdedPatterns.size() == patternSetSize) {
        emit originalPatternsThresholded();
        qDebug("All images thresholded");
    }
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
