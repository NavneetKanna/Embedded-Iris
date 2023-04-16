#include <iostream>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

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
   
    
    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        double aspectRatio = rect.width / (double)rect.height;
        // Check for only valid text regions
        if (aspectRatio > 1.0 && aspectRatio < 10.0 && rect.area() > 50 && rect.height > 10) {
            //rectangle(img, rect, Scalar(0, 0, 255), 2);
            // Pre-Processing for OCR 
            Mat roi = img(rect);
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
            ocr->SetImage((uchar*)roi_gray.data, roi_gray.cols, roi_gray.rows, 1, roi_gray.step);
            //ocr->SetImage(resized_img.data, resized_img.cols, resized_img.rows, 3, resized_img.step);
            //ocr->SetImage(roi.data, roi.cols, roi.rows, 3, roi.step);
            ocr->SetSourceResolution(300);
            string text = ocr->GetUTF8Text();
            //std::cout<<"text " <<text<<std::endl;
            ocr_result += text + " ";
        }
    }
    

    }  
}



    

