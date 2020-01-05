#include <iostream>
#include <opencv2/opencv.hpp>

void DrawRotatedRectangle(cv::Mat &image, cv::RotatedRect rotatedRect)
{
    cv::Scalar color = cv::Scalar(0, 255, 0);
    cv::Point2f vertices[4];
    rotatedRect.points(vertices);

    for (int i = 0; i < 4; i++)
    {
        cv::line(image, vertices[i], vertices[(i + 1) % 4],
                 cv::Scalar(0, 255, 0), 2);
    }

    cv::Rect brect = rotatedRect.boundingRect();
    cv::rectangle(image, brect, cv::Scalar(255, 0, 0), 2);
}

int main(int argc, char const *argv[])
{
    std::string filename = "res/test.jpg";
    cv::Mat image = cv::imread(filename);

    cv::Mat gray;
    cv::cvtColor(image, gray, CV_BGR2GRAY);

    cv::Mat blurred;
    cv::medianBlur(gray, blurred, 9);

    cv::Mat thresh_gray;
    cv::threshold(blurred, thresh_gray, 200, 255, CV_THRESH_BINARY);

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(thresh_gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    std::vector<cv::Mat> minAreaContours;
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::RotatedRect minAreaRect = cv::minAreaRect(cv::Mat(contours[i]));
        std::cout << "center: " << minAreaRect.center.x << ", " << minAreaRect.center.y << std::endl;
        DrawRotatedRectangle(image, minAreaRect);
    }

    std::cout << minAreaContours.size() << std::endl;
    cv::drawContours(image, minAreaContours, 0, cv::Scalar(0, 255, 0), 10);

    cv::Mat final = image;
    cv::imwrite("test.jpg", final);

    return 0;
}
