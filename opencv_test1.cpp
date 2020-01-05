#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>

typedef std::vector<cv::Point> Contour;

void strokeRotatedRectangle(cv::Mat &image, cv::RotatedRect rotatedRect)
{
    cv::Scalar color = cv::Scalar(0, 255, 0);
    cv::Point2f vertices[4];
    rotatedRect.points(vertices);

    for (int i = 0; i < 4; i++)
    {
        cv::line(image, vertices[i], vertices[(i + 1) % 4],
                 color, 2);
    }
}

void strokeRotatedRectangleWithBoundingBox(cv::Mat &image, cv::RotatedRect rotatedRect)
{
    cv::Scalar color = cv::Scalar(0, 255, 0);
    cv::Point2f vertices[4];
    rotatedRect.points(vertices);

    for (int i = 0; i < 4; i++)
    {
        cv::line(image, vertices[i], vertices[(i + 1) % 4],
                 color, 2);
    }

    cv::Rect brect = rotatedRect.boundingRect();
    cv::rectangle(image, brect, cv::Scalar(255, 0, 0), 2);
}

void foo1()
{
    // std::string filename = "res/test.jpg";
    std::string filename = "kartka.jpg";
    cv::Mat image = cv::imread(filename);

    cv::Mat gray;
    cv::cvtColor(image, gray, CV_BGR2GRAY);

    cv::Mat blurred;
    cv::medianBlur(gray, blurred, 9);

    cv::Mat thresh_gray;
    cv::threshold(blurred, thresh_gray, 200, 255, CV_THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh_gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    std::vector<cv::Mat> minAreaContours;
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::RotatedRect minAreaRect = cv::minAreaRect(cv::Mat(contours[i]));
        std::cout << "center: " << minAreaRect.center.x << ", " << minAreaRect.center.y << std::endl;
        strokeRotatedRectangle(image, minAreaRect);
    }

    std::cout << minAreaContours.size() << std::endl;
    cv::drawContours(image, minAreaContours, 0, cv::Scalar(0, 255, 0), 10);

    cv::Mat final = image;
    cv::imwrite("test.jpg", final);
}

void foo2()
{
    // std::string filename = "res/3.jpg";
    std::string filename = "res/9.jpg";
    cv::Mat init_image = cv::imread(filename);

    cv::Mat image;
    double height = 800;
    cv::Size image_size(int(height / init_image.rows * init_image.cols), height);
    cv::resize(init_image, image, image_size);

    cv::Mat blurred_image;
    cv::medianBlur(image, blurred_image, 9);

    cv::Mat gray_image;
    cv::cvtColor(blurred_image, gray_image, CV_BGR2GRAY);

    cv::Mat canny_image;
    cv::Canny(gray_image, canny_image, 30, 50, 3);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(canny_image, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    std::cout << "Contour num = " << contours.size() << std::endl;

    double maxPerimeter = 0;
    std::vector<cv::Point> maxContour;

    for (size_t i = 0; i < contours.size(); i++)
    {
        std::vector<cv::Point> contour = contours[i];
        double perimeter = cv::arcLength(contour, true);
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, 0.03 * perimeter, true);

        if (perimeter > maxPerimeter)
        {
            maxPerimeter = perimeter;
            maxContour = approx;
        }

        // std::cout << perimeter << std::endl;
        // cv::polylines(image, approx, true, cv::Scalar(0, 255, 0), 1, 8);
    }

    std::cout << "Max perimeter = " << maxPerimeter << std::endl;
    cv::polylines(image, maxContour, true, cv::Scalar(0, 255, 0), 1, 8);

    // ---  WRITE  --------
    cv::Mat final = gray_image;
    // cv::namedWindow("Display window", CV_WINDOW_AUTOSIZE);
    // cv::imshow("Display window", final);
    // cv::waitKey(0);
    cv::imwrite("test.jpg", final);
}

void foo3()
{
    std::string filename = "res/4.jpg";
    cv::Mat init_image = cv::imread(filename);

    // File validation here

    cv::Mat image;
    double height = 800;
    cv::Size image_size(int(height / init_image.rows * init_image.cols), height);
    cv::resize(init_image, image, image_size);

    cv::Mat blurred_image;
    cv::medianBlur(image, blurred_image, 9);

    cv::Mat gray_image;
    cv::cvtColor(blurred_image, gray_image, CV_BGR2GRAY);

    cv::Mat canny_image;
    cv::Canny(gray_image, canny_image, 30, 50, 3);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(canny_image, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    std::cout << "Contour num = " << contours.size() << std::endl;

    double maxPerimeter = 0;
    std::vector<cv::Point> maxContour;

    for (size_t i = 0; i < contours.size(); i++)
    {
        std::vector<cv::Point> contour = contours[i];
        double perimeter = cv::arcLength(contour, true);
        std::vector<cv::Point> contourApprox;
        cv::approxPolyDP(contour, contourApprox, 0.02 * perimeter, true);

        if (perimeter > maxPerimeter && cv::isContourConvex(contourApprox) && contourApprox.size() == 4)
        {
            maxPerimeter = perimeter;
            maxContour = contourApprox;
        }
    }

    cv::polylines(image, maxContour, true, cv::Scalar(0, 255, 0), 1, 8);

    cv::Point2f srcTransformationPoints[4];
    for (size_t i = 0; i < 4; i++)
    {
        srcTransformationPoints[i] = cv::Point2f(maxContour.data()[i]);
    }

    std::cout << "src: " << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << srcTransformationPoints[i].x << ", " << srcTransformationPoints[i].y << std::endl;
    }

    cv::Point2f destTransformationPoints[4];
    if (srcTransformationPoints[0].y < srcTransformationPoints[1].y &&
        srcTransformationPoints[1].x < srcTransformationPoints[2].x &&
        srcTransformationPoints[2].y > srcTransformationPoints[3].y &&
        srcTransformationPoints[3].x > srcTransformationPoints[0].x)
    {
        destTransformationPoints[0] = cv::Point2f(0, 0);
        destTransformationPoints[1] = cv::Point2f(0, image_size.height);
        destTransformationPoints[2] = cv::Point2f(image_size.width, image_size.height);
        destTransformationPoints[3] = cv::Point2f(image_size.width, 0);
    }
    else
    {
        destTransformationPoints[0] = cv::Point2f(image_size.width, 0);
        destTransformationPoints[1] = cv::Point2f(0, 0);
        destTransformationPoints[2] = cv::Point2f(0, image_size.height);
        destTransformationPoints[3] = cv::Point2f(image_size.width, image_size.height);
    }

    // const cv::Point2f destTransformationPoints[4] = {cv::Point2f(0, 0),
    //                                                  cv::Point2f(0, image_size.height),
    //                                                  cv::Point2f(image_size.width, image_size.height),
    //                                                  cv::Point2f(image_size.width,0)};

    // cv::Point2f destTransformationPoints[4] = {cv::Point2f(image_size.width, 0), cv::Point2f(0, 0), cv::Point2f(0, image_size.height),
    //                                            cv::Point2f(image_size.width, image_size.height)};

    std::cout << "dest: " << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << destTransformationPoints[i].x << ", " << destTransformationPoints[i].y << std::endl;
    }

    cv::Mat transformationMatrix = cv::getPerspectiveTransform(srcTransformationPoints, destTransformationPoints);
    std::cout << "Transformation " << std::endl
              << transformationMatrix << std::endl;
    cv::Mat transformed_matrix;
    cv::warpPerspective(image, transformed_matrix, transformationMatrix, image_size);

    // ---  WRITE  --------
    cv::Mat final = transformed_matrix;
    // cv::namedWindow("Display window", CV_WINDOW_AUTOSIZE);
    // cv::imshow("Display window", final);
    // cv::waitKey(0);
    cv::imwrite("test.jpg", final);
}

int main(int argc, char const *argv[])
{
    foo3();
    return 0;
}
