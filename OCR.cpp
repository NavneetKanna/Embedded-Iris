#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) 
{
    Mat img = imread(argv[1]); 
     if (img.empty()) {
        std::cout << "Could not read the image" << std::endl;
    }
  
    // Text Detection
    
    // Convert image to grayscale
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Apply Gaussian blur to reduce noise
    GaussianBlur(gray, gray, Size(3, 3), 0);

    // Apply adaptive thresholding to binarize image - easy to process
    Mat thresh;
    adaptiveThreshold(gray, thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);

    // Find contours (text regions) in the thresholded image
    vector<vector<Point>> contours;
    findContours(thresh, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
   
    
     int largest = 0;
    Rect large;
    // Loop through each contour(text region)
    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        //std::cout<<"detected rect"<<std::endl;
        if (rect.area() > largest) {
        std::cout<<"detected large rect"<<std::endl;
         large = rect;
         largest = rect.area();
        }
    }
    
    
    double aspectRatio = large.width / (double)large.height;
     
    Mat roi = img(large);
    imwrite("tt.jpg", roi);
    Mat resized_img;
    resize(roi, resized_img, Size(), 1.2, 1.2);  
    Mat roi_gray;
    cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(1, 1));
    dilate(roi_gray, roi_gray, kernel, Point(-1, -1), 1);
    erode(roi_gray, roi_gray, kernel, Point(-1, -1), 1);
    medianBlur(roi_gray, roi_gray, 3);
    threshold(roi_gray, roi_gray, 0, 255, THRESH_BINARY+THRESH_OTSU);
    
    // Perform OCR
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    // ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetPageSegMode(tesseract::PSM_AUTO);
    ocr->SetImage((uchar*)roi_gray.data, roi_gray.cols, roi_gray.rows, 1, roi_gray.step);
    //ocr->SetImage(resized_img.data, resized_img.cols, resized_img.rows, 3, resized_img.step);
    //ocr->SetImage(roi.data, roi.cols, roi.rows, 3, roi.step);
    ocr->SetSourceResolution(300);
    string text = ocr->GetUTF8Text();
    ocr_result = "";
    if(!text.empty()){
        ocr_result = text + " ";
    }


    }  
}



    

