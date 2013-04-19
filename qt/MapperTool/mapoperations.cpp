#include "mapoperations.h"
#include <QDebug>
#include <QPoint>

#include "opencv2/photo/photo.hpp"

MapOperations::MapOperations(cv::Mat initialMap)
{
    m_map = initialMap;
}

MapOperations::~MapOperations()
{
}

cv::Mat MapOperations::inverse(QSize size, uint bits, bool centered)
{
    cv::Mat newMap = cv::Mat::zeros(size.height(), size.width(), CV_16UC4);

    const int channels = m_map.channels();
    const uint mapCols = m_map.cols;
    const uint mapRows = m_map.rows;

    const uint powerOf2Size = (uint)pow( 2., (double)bits );

    double uvMultiplier = (double)powerOf2Size / 65536.0;
    QPoint uvOffset(0,0);
    if(centered)
        uvOffset = (QPoint(powerOf2Size, powerOf2Size) - QPoint(size.width(), size.height())) / 2;

    uint x,y;
    ushort u,v,a;
    cv::Vec3w mapPixel;

    for (y=0; y<mapRows; y++) {
        for(x=0; x<mapCols; x++) {
            if(channels == 3) {
                cv::Vec3w pixel = m_map.at<cv::Vec3w>(y,x);
                // CV stores pixels in BGR order
                mapPixel = cv::Vec3w(pixel[2], pixel[1], -1);
            } else {
                cv::Vec4s pixel = m_map.at<cv::Vec4s>(y,x);
                // CV stores pixels in BGR order
                mapPixel = cv::Vec3w(pixel[2], pixel[1], pixel[3]);
            }
            u = (unsigned long)mapPixel[0] * uvMultiplier - uvOffset.x();
            v = (unsigned long)mapPixel[1] * uvMultiplier - uvOffset.y();
            a = mapPixel[2];

            if(u<size.width() && v<size.height()) {
                cv::Vec4w &newMapPixel = newMap.at<cv::Vec4w>(v,u);
                if(newMapPixel[3] < a) {
                    int newX = (x << 16) / mapCols;
                    int newY = (y << 16) / mapRows;

                    // CV stores pixels in BGR order
                    newMapPixel[0] = 0;
                    newMapPixel[1] = newY;
                    newMapPixel[2] = newX;
                    newMapPixel[3] = a;
                }
            }
        }
    }

    m_map = newMap;
    return m_map;
}

cv::Mat MapOperations::guassianBlur(double radius)
{
    cv::Mat newMap = m_map.clone();

    // This gaussian filter is alpha-aware
    // red & green are first multiplied by alpha,
    // then all three channels are blurred and
    // finally red and green are divided by the (blurred) alpha

    cv::Mat red(newMap.rows, newMap.cols, CV_16UC1);
    cv::Mat green(newMap.rows, newMap.cols, CV_16UC1);
    cv::Mat alpha(newMap.rows, newMap.cols, CV_16UC1);
    cv::Mat mats[] = { red, green, alpha };

    // CV stores pixels in BGR order
    // We're not interested in the blue channel
    int mixSplit[] = { 2,0, 1,1, 3,2 };
    int mixMerge[] = { 0,2, 1,1, 2,3 };

    mixChannels(&newMap, 1, mats, 3, mixSplit, 3);

    multiply(red, alpha, red, 1./65536);
    multiply(green, alpha, green, 1./65536);

    cv::GaussianBlur(red, red, cv::Size(), radius);
    cv::GaussianBlur(green, green, cv::Size(), radius);
    cv::GaussianBlur(alpha, alpha, cv::Size(), radius);

    divide(red, alpha, red, 65536);
    divide(green, alpha, green, 65536);

    mixChannels(mats, 3, &newMap, 1, mixMerge, 3);

    m_map = newMap;
    return m_map;
}

cv::Mat MapOperations::medianBlur(int radius)
{
    // NB: this function should be made alphachannel-aware
    cv::Mat newMap = m_map.clone();

    cv::medianBlur(m_map, newMap, radius);
    m_map = newMap;
    return m_map;
}

cv::Mat MapOperations::despeckle()
{
    // NB: this function should be made alphachannel-aware
    cv::Mat newMap = m_map.clone();

    // TODO: despeckle
    m_map = newMap;
    return m_map;
}

cv::Mat MapOperations::fillHoles()
{
    // NB: this function should be made alphachannel-aware
    cv::Mat newMap = m_map.clone();

    cv::Mat smallMap;
    cv::resize(newMap, smallMap, cv::Size(512, 512),0,0,cv::INTER_NEAREST);

    cv::Mat alpha16(smallMap.rows, smallMap.cols, CV_16UC1), alpha;
    cv::Mat rgb16(smallMap.rows, smallMap.cols, CV_16UC3), rgb;

    cv::Mat resultMats[] = { rgb16, alpha16 };
    int channelMix[] = { 0,0, 1,1, 2,2, 3,3 };
    cv::mixChannels( &smallMap, 1, resultMats, 2, channelMix, 4 );

    cv::Mat smallMask = (alpha16==0);

    rgb16.convertTo(rgb, CV_8UC3, 1/256., 0);
    rgb16.release();

    alpha16.convertTo(alpha, CV_8UC1, 1/256., 0);
    alpha16.release();

    cv::Mat filledRgb;
    cv::Mat filledAlpha;
    cv::Mat filledRgba(smallMap.rows, smallMap.cols, CV_8UC4);

    cv::inpaint(rgb, smallMask, filledRgb, 3, cv::INPAINT_NS);
    cv::inpaint(alpha, smallMask, filledAlpha, 3, cv::INPAINT_NS);

    rgb.release();
    alpha.release();
    smallMap.release();

    cv::Mat inputMats[] = { filledRgb, filledAlpha };
    cv::mixChannels( inputMats, 2, &filledRgba, 1, channelMix, 4 );

    filledRgb.release();
    filledAlpha.release();

    cv::Mat tmpMap;
    cv::resize(filledRgba, tmpMap, cv::Size(newMap.cols, newMap.rows),0,0,cv::INTER_NEAREST);

    cv::Mat mask16(newMap.rows, newMap.cols, CV_16UC4);
    int maskMix[] = { 3,0, 3,1, 3,2, 3,3 };
    cv::mixChannels( &newMap, 1, &mask16, 1, maskMix ,4 );

    tmpMap &= (mask16==0);
    cv::Mat tmpMap16;
    tmpMap.convertTo(tmpMap16, CV_16UC4, 256., 0);
    newMap += tmpMap16;

    tmpMap16.release();

    m_map = newMap;
    return m_map;
}
