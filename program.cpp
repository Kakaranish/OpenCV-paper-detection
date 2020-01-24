#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "PaperSheetProcessor.hpp"

bool userProvidedHelpFlag(int argc, char const *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg == "-h")
        {
            return true;
        }
    }
    return false;
}

void showHelp()
{
    std::cout << "DESCRIPTION " << std::endl;
    std::cout << "The purpose of this program is detecting, straightening and trimming "
              << " photos of A4 sheets" << std::endl;
    std::cout << std::endl;
    std::cout << "SYNOPSIS" << std::endl;
    std::cout << "./program [FILE] [OPTION]..." << std::endl;
    std::cout << std::endl;
    std::cout << "Available options" << std::endl;
    std::cout << "-h            show help" << std::endl;
    std::cout << "-o filename   output filename" << std::endl;
    std::cout << std::endl;
}

void showErrorMessage(std::string message)
{
    std::cout << std::endl;
    std::cout << "*   " << message << "   *" << std::endl;
    std::cout << std::endl;
    showHelp();
}

std::string getProvidedByUserOutputFilename(int argc, char const *argv[])
{
    std::string flag(argv[2]);
    std::string filename(argv[3]);
    if (flag == "-o")
    {
        return filename;
    }
    return std::string();
}

int main(int argc, char const *argv[])
{
    if (userProvidedHelpFlag(argc, argv))
    {
        showHelp();
        exit(0);
    }

    std::string outputFilename = "output.jpg";
    if (argc == 4)
    {
        outputFilename = getProvidedByUserOutputFilename(argc, argv);
        if (outputFilename.empty())
        {
            showErrorMessage("Invalid arguments");
            exit(-1);
        }
    }
    else if (argc != 2)
    {
        showErrorMessage("Args count is not valid. Required exactly two");
        exit(-1);
    }

    std::string filename = argv[1];
    PaperSheetProcessor processor;
    cv::Mat *result = processor.processImage(filename);
    if (result == nullptr)
    {
        showErrorMessage("Provided file is not image or cannot be loaded.");
        exit(-1);
    }

    cv::imwrite(outputFilename, *result);
    return 0;
}
