#ifndef MT_GRAYDECODER_H
#define MT_GRAYDECODER_H

#include <stdlib.h>
#include <opencv2/opencv.hpp>

class mt_grayDecoder
{
public:
    enum MTGD_STATE {
        MTGD_STATE_INIT,
        MTGD_STATE_LOADFILES,
        MTGD_STATE_FINDEXTREME,
        MTGD_STATE_THRESHOLDING,
        MTGD_STATE_GRAYDECODING,
        MTGD_STATE_INVERSING,
        MTGD_STATE_FINISHED,
        MTGD_STATE_ERROR
    };
    MTGD_STATE state;

    mt_grayDecoder();

    void setState(MTGD_STATE state);
    static cv::Mat* loadFile(std::string filepath);
    //cv::Mat* findExtremePixels(std::vector<cv::Mat*>* originalsPtr);
    cv::Mat* findExtremeMinPixels(std::vector<cv::Mat*>* originalsPtr);
    cv::Mat* findExtremeMaxPixels(std::vector<cv::Mat*>* originalsPtr);
    cv::Mat* findExtremeMinMaxDiffPixels(cv::Mat& min, cv::Mat& max);
    cv::Mat* thresholdImage(cv::Mat* imgPtr);
    cv::Mat* thresholdedImagesToGrayCode(std::vector<cv::Mat*>* thresholdedPtr);

protected:
    void failed();
    void success();

private:

};

#endif // MT_GRAYDECODER_H
