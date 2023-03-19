
// g++ -I/home/pi/darknet/include -I/usr/include/opencv4  object_detection.cpp -o darknt `pkg-config --libs opencv4` -L/home/pi/darknet -ldarknet 




// Include Libraries.
#include <opencv2/opencv.hpp>
#include <fstream>

// Namespaces.
using namespace cv;
using namespace std;
using namespace cv::dnn;

// Constants.
const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.5;
const float NMS_THRESHOLD = 0.45;
const float CONFIDENCE_THRESHOLD = 0.45;

vector<Mat> pre_process(Mat &input_image, Net &net)
{
    // Convert to blob.
    Mat blob;
    blobFromImage(input_image, blob, 1./255., Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(), true, false);

    net.setInput(blob);

    // Forward propagate.
    vector<Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    return outputs;
}


Mat post_process(Mat input_image, vector<Mat> &outputs, const vector<string> &class_name) 
{
    // Initialize vectors to hold respective outputs while unwrapping detections.
    vector<int> class_ids;
    vector<float> confidences;

    // Resizing factor.
    float x_factor = input_image.cols / INPUT_WIDTH;
    float y_factor = input_image.rows / INPUT_HEIGHT;

    float *data = (float *)outputs[0].data;

    const int dimensions = 85;
    const int rows = 25200;
    // Iterate through 25200 detections.
    for (int i = 0; i < rows; ++i) 
    {
        float confidence = data[4];
        // Discard bad detections and continue.
        if (confidence >= CONFIDENCE_THRESHOLD) 
        {
            float * classes_scores = data + 5;
            // Create a 1x85 Mat and store class scores of 80 classes.
            Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
            // Perform minMaxLoc and acquire index of best class score.
            Point class_id;
            double max_class_score;
            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            // Continue if the class score is above the threshold.
            if (max_class_score > SCORE_THRESHOLD) 
            {
                // Store class ID and confidence in the pre-defined respective vectors.
                confidences.push_back(confidence);
                class_ids.push_back(class_id.x);
            }

        }
        // Jump to the next column.
        data += 85;
    }

    // Perform Non Maximum Suppression and draw predictions.
    vector<int> indices;
    std::cout<<"nms"<<std::endl;
    NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
    std::cout<<"finished"<<std::endl;
    std::cout<<"indices size " <<indices.size()<<std::endl;
    
    for (int i = 0; i < indices.size(); i++) 
    {
        int idx = indices[i];

        // Get the label for the class name and its confidence.
        string label = format("%.2f", confidences[idx]);
        label = class_name[class_ids[idx]] + ":" + label;
        std::cout<<"Detected object: "<<label<<std::endl;
    }
    std::cout<<"finished"<<std::endl;
    return input_image;
}


int main()
{
    // Load class list.
    vector<string> class_list;
    std::cout<<"ifstream"<<std::endl;
    ifstream ifs("coco.names");
    std::cout<<"finsihed"<<std::endl;
    string line;
    
    std::cout<<"while"<<std::endl;
    while (getline(ifs, line))
    {
        class_list.push_back(line);
    }
    std::cout<<"finished"<<std::endl;

    // Load image.
    Mat frame;
    frame = imread("test1.jpg");
    //frame = imread("banana2.jpg");

    // Load model.
    Net net;
    std::cout<<"read net"<<std::endl;
    net = readNet("yolov5s.onnx"); 
    std::cout<<"finsihed"<<std::endl;

    vector<Mat> detections;
    std::cout<<"pre process"<<std::endl;
    detections = pre_process(frame, net);
    std::cout<<"finished"<<std::endl;

    std::cout<<"post process"<<std::endl;
    Mat img = post_process(frame.clone(), detections, class_list);
    std::cout<<"finished"<<std::endl;

    imshow("Output", img);
    waitKey(0);

    return 0;
}
