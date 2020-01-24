#include "PaperSheetProcessor.hpp"

cv::Point2f *PaperSheetProcessor::maintainTransformationPoints(cv::Point2f *srcTransformPoints, cv::Size imageSize)
{
    const int pointsCount = 4;
    cv::Point2f *destTransformPoints = new cv::Point2f[pointsCount];
    for (size_t i = 0; i < pointsCount; i++)
    {
        destTransformPoints[i] = cv::Point2f(srcTransformPoints[i]);
    }

    if (destTransformPoints[0].y < destTransformPoints[1].y &&
        destTransformPoints[1].x < destTransformPoints[2].x &&
        destTransformPoints[2].y > destTransformPoints[3].y &&
        destTransformPoints[3].x > destTransformPoints[0].x)
    {
        destTransformPoints[0] = cv::Point2f(0, 0);
        destTransformPoints[1] = cv::Point2f(0, imageSize.height);
        destTransformPoints[2] = cv::Point2f(imageSize.width, imageSize.height);
        destTransformPoints[3] = cv::Point2f(imageSize.width, 0);
    }
    else
    {
        destTransformPoints[0] = cv::Point2f(imageSize.width, 0);
        destTransformPoints[1] = cv::Point2f(0, 0);
        destTransformPoints[2] = cv::Point2f(0, imageSize.height);
        destTransformPoints[3] = cv::Point2f(imageSize.width, imageSize.height);
    }

    return destTransformPoints;
}

cv::Mat *PaperSheetProcessor::processImage(std::string filename)
{
    cv::Mat init_image = cv::imread(filename);
    if (init_image.empty())
    {
        return nullptr;
    }

    cv::Mat image;
    double height = 800;
    cv::Size imageSize(int(height / init_image.rows * init_image.cols), height);
    cv::resize(init_image, image, imageSize);

    cv::Mat blurred_image;
    cv::medianBlur(image, blurred_image, 9);

    cv::Mat gray_image;
    cv::cvtColor(blurred_image, gray_image, CV_BGR2GRAY);

    cv::Mat canny_image;
    cv::Canny(gray_image, canny_image, 30, 50, 3);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(canny_image, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

    double maxFoundPerimeter = 0;
    std::vector<cv::Point> maxContour;
    for (size_t i = 0; i < contours.size(); i++)
    {
        std::vector<cv::Point> contour = contours[i];
        double perimeter = cv::arcLength(contour, true);
        std::vector<cv::Point> contourApprox;
        cv::approxPolyDP(contour, contourApprox, 0.02 * perimeter, true);

        if (perimeter > maxFoundPerimeter && cv::isContourConvex(contourApprox) && contourApprox.size() == 4)
        {
            maxFoundPerimeter = perimeter;
            maxContour = contourApprox;
        }
    }

    cv::polylines(image, maxContour, true, cv::Scalar(0, 255, 0), 1, 8);

    cv::Point2f srcTransformationPoints[4];
    for (size_t i = 0; i < 4; i++)
    {
        srcTransformationPoints[i] = cv::Point2f(maxContour.data()[i]);
    }
    cv::Point2f *destTransformationPoints = maintainTransformationPoints(srcTransformationPoints, imageSize);
    cv::Mat transformationMatrix = cv::getPerspectiveTransform(srcTransformationPoints, destTransformationPoints);
    cv::Mat *finalImage = new cv::Mat;
    cv::warpPerspective(image, *finalImage, transformationMatrix, imageSize);
    delete destTransformationPoints;

    return finalImage;
}