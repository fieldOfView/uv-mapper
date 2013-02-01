#include "mt_graydecoder.h"

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

    //cv::Mat bla  = cv::imread(filepath, CV_LOAD_IMAGE_UNCHANGED);
    //return &bla
}

cv::Mat* mt_grayDecoder::findExtremePixels(std::vector<cv::Mat *> *originals) {

    std::cout << "Do findExtremePixels\n";
}

cv::Mat* mt_grayDecoder::thresholdImage(cv::Mat *img) {

    std::cout << "Do thresholdImage\n";
}

cv::Mat* mt_grayDecoder::thresholdedImagesToGrayCode(std::vector<cv::Mat *> *thresholded) {

    std::cout << "Do thresholdedImagesToGrayCode\n";
}

void mt_grayDecoder::failed() {

    std::cout << "MT_grayDecoder: Damn, failed\n";
}

void mt_grayDecoder::success() {

    std::cout << "MT_grayDecoder: Yeah, success\n";
}
