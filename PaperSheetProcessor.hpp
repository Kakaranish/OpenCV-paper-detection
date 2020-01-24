#ifndef PAPERSHEETPROCESSOR_HPP
#define PAPERSHEETPROCESSOR_HPP

#include <algorithm>
#include <opencv2/opencv.hpp>

class PaperSheetProcessor
{
private:
    cv::Point2f *maintainTransformationPoints(cv::Point2f *srcTransformPoints, cv::Size imageSize);
public:
    cv::Mat *processImage(std::string filename);
};

#endif