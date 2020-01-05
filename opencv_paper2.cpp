#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        std::cout << "No image path provided." << std::endl;
        return -1;
    }

    cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(image.data == false)
    {
        std::cout << "There is no image behind provided path." << std::endl;
        return -1;
    }

    // cv::minAreaRect
    // cv::minAreaRect()
    cv::Mat thresholdedImage;
    cv::threshold(image, thresholdedImage, 197, 255, CV_ADAPTIVE_THRESH_MEAN_C);
    // cv::Mat thresholdedImage(image);
    // cv::contourArea()
    cv::Mat denoisedImage;
    // cv::fastNlMeansDenoising(thresholdedImage, denoisedImage)
    cv::medianBlur(thresholdedImage, denoisedImage, 5);
    cv::imwrite("generated_kartka.jpg", denoisedImage);

    return 0;
}
