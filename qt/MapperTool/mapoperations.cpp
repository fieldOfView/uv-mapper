#include "mapoperations.h"
#include <QDebug>

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
    QPoint uvOffset;
    if(centered)
        uvOffset = (QPoint(powerOf2Size, powerOf2Size) - QPoint(size.width(), size.height())) / 2;
    else
        uvOffset = QPoint(0,0);

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
    // NB: this function should be made alphachannel-aware
    cv::Mat newMap = m_map.clone();

    cv::GaussianBlur(m_map, newMap, cv::Size(), radius);
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

    // TODO: fill holes
    m_map = newMap;
    return m_map;
}
