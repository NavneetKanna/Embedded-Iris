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
#include <sstream>
//#include "speak.h"

using namespace cv;
using namespace std;
using namespace cv::dnn;

class OCR {
  public:
     OCR();
     void perform_ocr();
     void get_image(); 
     void start();
     std::string get_ocr_result();
     

     std::string ocr_result = "";
     tesseract::TessBaseAPI *ocr;
     
};
