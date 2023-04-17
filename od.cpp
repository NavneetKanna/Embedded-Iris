#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <thread>
#include "speak.h"

#include "od.h"

using namespace cv;
using namespace std;
using namespace cv::dnn;

// Constants.
const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.5;
const float NMS_THRESHOLD = 0.45;
const float CONFIDENCE_THRESHOLD = 0.45;

OD::OD() {
    // Load class list.
    ifstream ifs("coco.names");
    string line;
    while (getline(ifs, line)) {
	class_name.push_back(line);
    }

    // Load model.
    net_ = readNet("/home/pi/Lab/yolov5s.onnx"); 
}

void OD::get_image(){
   char command[100];
   snprintf(command, sizeof(command), "libcamera-jpeg -o image.jpg --nopreview  -t1000 --rotation 180 2>/dev/null");

   // Launch the command and capture its output
   FILE* pipe = popen(command, "r");
   if (!pipe) {
      Speak speak;
      speak.text_to_speech("popen failed");
   }
    
   // Wait for the command to finish
   pclose(pipe);
}

void OD::start(){
    std::thread t(&OD::predict, this);
    t.detach();
}

void OD::predict() {
    get_image();
    // Read the image
    Mat input_image = imread("image.jpg");
    Mat re;
    resize(input_image, re, Size(1280, 720));
    // Convert to blob.
    Mat blob;
    blobFromImage(re, blob, 1./255., Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(), true, false);

    net_.setInput(blob);

    // Forward propagation.
    vector<Mat> outputs;
    net_.forward(outputs, net_.getUnconnectedOutLayersNames());

    // Initialize vectors to hold respective outputs while unwrapping detections.
    vector<int> class_ids;
    vector<float> confidences;
    vector<Rect> boxes; 

    // Resizing factor.
    float x_factor = re.cols / INPUT_WIDTH;
    float y_factor = re.rows / INPUT_HEIGHT;

    float *data = (float *)outputs[0].data;

    const int dimensions = 85;
    const int rows = 25200;
    // Iterate through 25200 detections.
    for (int i = 0; i < rows; ++i) {
	float confidence = data[4];
	// Discard bad detections and continue.
	if (confidence >= CONFIDENCE_THRESHOLD) {
	    float * classes_scores = data + 5;
	    // Create a 1x85 Mat and store class scores of 80 classes.
	    Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
	    // Perform minMaxLoc and acquire index of best class score.
	    Point class_id;
	    double max_class_score;
	    minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
	    // Continue if the class score is above the threshold.
	    if (max_class_score > SCORE_THRESHOLD) {
	        // Store class ID and confidence in the pre-defined respective vectors.
	        confidences.push_back(confidence);
	        class_ids.push_back(class_id.x);

	        // Center.
	        float cx = data[0];
	        float cy = data[1];
	        // Box dimension.
	        float w = data[2];
	        float h = data[3];
	        // Bounding box coordinates.
	        int left = int((cx - 0.5 * w) * x_factor);
	        int top = int((cy - 0.5 * h) * y_factor);
	        int width = int(w * x_factor);
	        int height = int(h * y_factor);
	        // Store good detections in the boxes vector.
	        boxes.push_back(Rect(left, top, width, height));
	    }

	}
	// Jump to the next column.
	data += 85;
    }

    // Perform Non Maximum Suppression
    vector<int> indices;
    NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
    
    // TODO: What if there is more than 1 prediction ?
    for (int i = 0; i < indices.size(); i++) {
	int idx = indices[i];

	// Get the label for the class name and its confidence.
	label = format("%.2f", confidences[idx]);
	label = class_name[class_ids[idx]];
    }
    Speak speak;
    speak.text_to_speech(label);
}

// Getter
std::string OD::get_predicted_label(){
    return label;
}

