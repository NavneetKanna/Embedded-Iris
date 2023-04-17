#include <opencv2/opencv.hpp>
#include <fstream>
#include <string.h>
#include <vector>
//#include "speak.h"


using namespace cv;
using namespace std;
using namespace cv::dnn;

class OD {
  public:
    OD();
    void get_image();
    void predict();
    void start();
    std::string get_predicted_label();
    
  
    Net net_;
    string label;
    std::vector<string> class_name;
    
};





