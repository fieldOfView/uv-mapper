#include "mapoperations.h"

MapOperations::MapOperations(cv::Mat initialMap)
{
    map = initialMap;
}

MapOperations::~MapOperations()
{
}

cv::Mat MapOperations::Inverse(QSize size, bool centered)
{
    cv::Mat newMap(size.width(), size.height(), map.type());

    map = newMap;
    return map;
}

cv::Mat MapOperations::GuassianBlur(double radius)
{
    // NB: this function should be made alphachannel-aware
    cv::Mat newMap = map.clone();

    cv::GaussianBlur(map, newMap, cv::Size(), radius);
    map = newMap;
    return map;
}

cv::Mat MapOperations::MedianBlur(int radius)
{
    // NB: this function should be made alphachannel-aware
    cv::Mat newMap = map.clone();

    cv::medianBlur(map, newMap, radius);
    map = newMap;
    return map;
}

cv::Mat MapOperations::Despeckle()
{
    return map;
}
