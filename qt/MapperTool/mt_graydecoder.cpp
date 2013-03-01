#include "mt_graydecoder.h"
#include <QDebug>

mt_grayDecoder::mt_grayDecoder()
{
    state = MTGD_STATE_INIT;
}

void mt_grayDecoder::setState(MTGD_STATE state) {

    this->state = state;
}

cv::Mat* mt_grayDecoder::loadFile(std::string filepath)
{
    //remember you need to delete the ptr manually!!
    cv::Mat* loadPtr = new cv::Mat();
    *loadPtr = cv::imread(filepath, CV_LOAD_IMAGE_UNCHANGED);
    return loadPtr;
}

cv::Mat* mt_grayDecoder::findExtremeMinPixels(std::vector<cv::Mat*>* originals)
{
    std::cout << "Do findExtremePixels\n";
    //we start at image 3 just because Aldo says so :)
    //copy image 3 to the min and max Mat
    cv::Mat* min = new cv::Mat();
    *min = originals->at(2)->clone();
    for( int i = 3; i < originals->size(); i++ ) {
        cv::min(*min, *originals->at(i), *min);
    }
    return min;
}

cv::Mat* mt_grayDecoder::findExtremeMaxPixels(std::vector<cv::Mat*>* originals)
{
    std::cout << "Do findExtremePixels\n";
    //we start at image 3 just because Aldo says so :)
    //copy image 3 to the min and max Mat
    cv::Mat* max = new cv::Mat();
    *max = originals->at(2)->clone();
    for( int i = 3; i < originals->size(); i++ ) {
        cv::max(*max, *originals->at(i), *max);
    }
    return max;
}

cv::Mat* mt_grayDecoder::findExtremeMinMaxDiffPixels(cv::Mat& min, cv::Mat& max)
{
    cv::Mat* diff = new cv::Mat();
    //substract the final min from max to get the diff
    cv::subtract(max, min, *diff);
    return diff;
}

cv::Mat* mt_grayDecoder::thresholdImage(cv::Mat *img, cv::Mat min, cv::Mat diff) {

    std::cout << "Do thresholdImage\n";
    /*cv::Mat gray;
    cv::Mat* thresPtr = new cv::Mat();
    cv::cvtColor( *img, gray, CV_RGB2GRAY );
    cv::threshold( gray, *thresPtr, 0, 255, CV_THRESH_BINARY);
    return thresPtr;
    */
    //create thresholded image
    cv::Mat optimised;
    cv::Mat* thresPtr = new cv::Mat();

    //optimise image based on min and diff
    try {
        cv::subtract(*img, min, optimised);
        cv::divide(optimised, diff, optimised);
    }
    catch(std::exception e) {
        printf("Exception1: [%s]\n", e.what());
    }
    try {
        cv::cvtColor( optimised, *thresPtr, CV_RGB2GRAY );
        cv::threshold( *thresPtr, *thresPtr, 0, 255, CV_THRESH_BINARY);
    }
    catch(std::exception e) {
        printf("Exception: [%s]\n", e.what());
    }
    optimised.release();
    //diff.release();
    //min.release();
    return thresPtr;
}

cv::Mat* mt_grayDecoder::thresholdedImagesToGrayCode(std::vector<cv::Mat *> *thresholded) {

    std::cout << "Do thresholdedImagesToGrayCode\n";
    cv::Mat *dummy;
    return dummy;
}

void mt_grayDecoder::failed() {

    std::cout << "MT_grayDecoder: Damn, failed\n";
}

void mt_grayDecoder::success() {

    std::cout << "MT_grayDecoder: Yeah, success\n";
}
