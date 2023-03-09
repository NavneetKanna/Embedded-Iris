#include <iostream>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) 
{
    tesseract::ResultIterator* iterator = tess.GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
    if (iterator != NULL) {
        do {
            float x1, y1, x2, y2;
            iterator->BoundingBox(level, &x1, &y1, &x2, &y2);
            cv::Rect boundingBox(cv::Point(x1, y1), cv::Point(x2, y2));
            cv::rectangle(inputImage, boundingBox, cv::Scalar(0, 0, 255), 2);
        } while (iterator->Next(level));

    Mat img = imread(argv[1]); 
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    Mat thresh;
    threshold(gray, thresh, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng");
    tess.SetPageSegMode(tesseract::PSM_AUTO);
    tess.SetImage(thresh.data, thresh.cols, thresh.rows, 1, thresh.cols);
    char* out = tess.GetUTF8Text();
    cout << out << endl;
    return 0;


    }  



    ]

