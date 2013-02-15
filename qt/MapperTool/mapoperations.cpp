#include "mapoperations.h"
#include <QDebug>

MapOperations::MapOperations(cv::Mat initialMap)
{
    map = initialMap;
}

MapOperations::~MapOperations()
{
}

cv::Mat MapOperations::inverse(QSize size, bool centered)
{
    cv::Mat newMap(size.width(), size.height(), map.type());


    const int channels = map.channels();

    uint x,y;
    uint mapCols = map.cols;
    uint mapRows = map.rows;

    for(x=0; x<mapCols; x++) {
        for (y=0; y<mapRows; y++) {
            qDebug() << x << " | " << y;
            uint u,v,a;
            if(channels == 3) {
                cv::Vec3s mapPixel = map.at<cv::Vec3s>(x,y);
                u = mapPixel[0]*size.width()/65536;
                v = mapPixel[1]*size.height()/65536;
                a = -1;
            } else {
                cv::Vec4s mapPixel = map.at<cv::Vec4s>(x,y);
                u = mapPixel[0]*size.width()/65536;
                v = mapPixel[1]*size.height()/65536;
                a = mapPixel[3];
            }
            /*
            cv::Vec4s newMapPixel = newMap.at<cv::Vec4s>(u,v);
            if((uint)newMapPixel[3] < a) {
                newMap.at<cv::Vec4s>(u,v) = cv::Vec4s(x*65536/mapCols, y*65536/mapRows, 0, a);
            }
            */
        }
    }

    map = newMap;
    return map;
}

cv::Mat MapOperations::guassianBlur(double radius)
{
    // NB: this function should be made alphachannel-aware
    cv::Mat newMap = map.clone();

    cv::GaussianBlur(map, newMap, cv::Size(), radius);
    map = newMap;
    return map;
}

cv::Mat MapOperations::medianBlur(int radius)
{
    // NB: this function should be made alphachannel-aware
    cv::Mat newMap = map.clone();

    cv::medianBlur(map, newMap, radius);
    map = newMap;
    return map;
}

cv::Mat MapOperations::despeckle()
{
    // NB: this function should be made alphachannel-aware
    cv::Mat newMap = map.clone();

    // TODO: despeckle
    map = newMap;
    return map;
}
