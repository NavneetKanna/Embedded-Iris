#include <espeak-ng/speak_lib.h>
#include <iostream>
#include "ocr.h"
#include <cstdio>
#include <chrono>
#include <thread>
#include <string>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <sstream>

using namespace cv;
using namespace cv::dnn;

Net net;

std::string ocr(cv::Mat&);

void decode(const Mat& scores, const Mat& geometry, float scoreThresh,
            std::vector<RotatedRect>& detections, std::vector<float>& confidences)

{
    detections.clear();
    CV_Assert(scores.dims == 4); CV_Assert(geometry.dims == 4); CV_Assert(scores.size[0] == 1);
    CV_Assert(geometry.size[0] == 1); CV_Assert(scores.size[1] == 1); CV_Assert(geometry.size[1] == 5);
    CV_Assert(scores.size[2] == geometry.size[2]); CV_Assert(scores.size[3] == geometry.size[3]);

    const int height = scores.size[2];
    const int width = scores.size[3];
    for (int y = 0; y < height; ++y)
    {
        const float* scoresData = scores.ptr<float>(0, 0, y);
        const float* x0_data = geometry.ptr<float>(0, 0, y);
        const float* x1_data = geometry.ptr<float>(0, 1, y);
        const float* x2_data = geometry.ptr<float>(0, 2, y);
        const float* x3_data = geometry.ptr<float>(0, 3, y);
        const float* anglesData = geometry.ptr<float>(0, 4, y);
        for (int x = 0; x < width; ++x)
        {
            float score = scoresData[x];
            if (score < scoreThresh)
                continue;

            // Decode a prediction.
            // Multiple by 4 because feature maps are 4 time less than input image.
            float offsetX = x * 4.0f, offsetY = y * 4.0f;
            float angle = anglesData[x];
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);
            float h = x0_data[x] + x2_data[x];
            float w = x1_data[x] + x3_data[x];

            Point2f offset(offsetX + cosA * x1_data[x] + sinA * x2_data[x],
                           offsetY - sinA * x1_data[x] + cosA * x2_data[x]);
            Point2f p1 = Point2f(-sinA * h, -cosA * h) + offset;
            Point2f p3 = Point2f(-cosA * w, sinA * w) + offset;
            RotatedRect r(0.5f * (p1 + p3), Size2f(w, h), -angle * 180.0f / (float)CV_PI);
            detections.push_back(r);
            confidences.push_back(score);
        }
    }
}



void getTextRegions() {
	std::cout<<"------- In getTextRegions() ---------"<<std::endl;   
	std::string ocr_result = "";
  
    //	if (device == "cpu") {
        net.setPreferableBackend(DNN_TARGET_CPU);
    	//}

	// Open a video file or an image file or a camera stream.
	//VideoCapture cap;
	std::cout<<"Opening resize.jpg"<<std::endl;
	Mat frame = imread("resize.jpg");
	//if (parser.has("input")){
	
	//cap.open("resize.jpg");
	std::cout<<"Finished"<<std::endl;
	
	if (frame.empty()) {
		    std::cout << "Failed to load image" << std::endl;
		    return;
	}
		
	   //cap.open(parser.get<String>("input"));
	//}
	//else {
	//std::cout<<"In else"<<std::endl;
	   //cap.open(0);
	//}

	//static const std::string kWinName = "EAST: An Efficient and Accurate Scene Text Detector";
	//namedWindow(kWinName, WINDOW_NORMAL);
	
	std::cout<<"Defining some variables"<<std::endl;
	std::vector<Mat> output;
	std::vector<String> outputLayers(2);
	outputLayers[0] = "feature_fusion/Conv_7/Sigmoid";
	outputLayers[1] = "feature_fusion/concat_3";
	std::cout<<"Finished"<<std::endl;

	Mat  blob;
	
	std::cout<<"cap>>frame"<<std::endl;
	//cap >> frame;
	std::cout<<"Finished"<<std::endl;
	
	if (frame.empty()) {
	    std::cout<<"Frame is empty"<<std::endl;
	    waitKey();
	    
        }
	
	std::cout<<"blobFromImage"<<std::endl;
        blobFromImage(frame, blob, 1.0, Size(320, 320), Scalar(123.68, 116.78, 103.94), true, false);
        std::cout<<"Finished"<<std::endl;
        std::cout<<"Net set input"<<std::endl;
        net.setInput(blob);
        std::cout<<"Finished"<<std::endl;
        std::cout<<"Feed Forward"<<std::endl;
        net.forward(output, outputLayers);
        std::cout<<"Finished"<<std::endl;

        Mat scores = output[0];
        Mat geometry = output[1];

        // Decode predicted bounding boxes.
        std::vector<RotatedRect> boxes;
        std::vector<float> confidences;
        std::cout<<"Calling decode()"<<std::endl;
        decode(scores, geometry, 0.5, boxes, confidences);
        std::cout<<"Finished"<<std::endl;

        // Apply non-maximum suppression procedure.
        std::vector<int> indices;
        std::cout<<"Applying non-maximum suppression"<<std::endl;
        NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
        std::cout<<"Finished"<<std::endl;

        // Render detections.
        Point2f ratio((float)frame.cols / 320, (float)frame.rows / 320);
      	std::cout<<"Starting for loop indices.size()"<<std::endl;
      	int k = 0;
        for (size_t i = 0; i < indices.size(); ++i) {
		RotatedRect& box = boxes[indices[i]];
            
            	Point2f vertices[4];
            	std::cout<<"box.points"<<std::endl;
            	box.points(vertices);
            	std::cout<<"Finished"<<std::endl;

	    //	if (flag ==1) {
		//    std::cout<<"inside flag "<<std::endl;
		  //  flag =2;
		    //Point2f vertices2[4] = {{1334.93, 1885.55},{1275.97, 1711.98}, {2287.28, 1603.28}, {2346.24, 1776.85}};
	    	//}
	    	
		Point2f vertices2[4];
		
		std::cout<<"For Loop"<<std::endl;
	    	for (int j = 0; j < 4; ++j) {
	            vertices[j].x *= ratio.x;
	            vertices[j].y *= ratio.y;
	            vertices2[j] = vertices[j];
	            
	            //vertices2[j][0] = vertices[j].x
	            //vertices2[j][1] = vertices[j].y
	    	}
	    	std::cout<<"Finished"<<std::endl;

            	std::vector<Point> points2;
            	
            	std::cout<<"For loop"<<std::endl;
		for (int i=0; i<4;i++){
		    points2.push_back(vertices2[i]);            
		}
		std::cout<<"Finished"<<std::endl;
	     	
	     	std::cout<<"Mat::zeros"<<std::endl;
		Mat cropped = Mat::zeros(frame.size(), frame.type());
		std::cout<<"Finished"<<std::endl;
		
		std::cout<<"Mat:ones"<<std::endl;
		Mat mask = Mat::ones(frame.size(), CV_8UC1)*255;
		std::cout<<"Finished"<<std::endl;

		std::vector<std::vector<Point>> contours;
		std::vector<Point> contour;
		
		std::cout<<"For loop"<<std::endl;
		for(int i=0;i<4;++i){
	 	   contour.push_back(vertices2[i]);
		}
		
		std::cout<<"Finished"<<std::endl;
		
		contours.push_back(contour);
		   // drawContours(mask, contours, 0, Scalar(0), FILLED);
		std::cout<<"drawContours"<<std::endl;
		drawContours(mask, contours, 0, Scalar(255), FILLED);
		std::cout<<"Finished"<<std::endl;


		    //fillPoly(mask, contours, Scalar(255));
		//Mat cropped2;
		std::cout<<"Copy mask to cropped"<<std::endl;
		frame.copyTo(cropped, mask);
		std::cout<<"Finished"<<std::endl;
		    
		Rect boundingRect1 = cv::boundingRect(contour);
		Mat croppedRegion = cropped(boundingRect1);
		    
		    //std::vector<Point2f> srcPoints = Point2f()
		std::cout<<"Saving to cropped2.jpg"<<std::endl;
		std::stringstream ss;
		ss << "cropped" << k <<".jpg";
		std::string cropped_name = ss.str();
		imwrite(cropped_name, croppedRegion);
		k += 1;
		std::cout<<"Finished"<<std::endl;
		
		std::cout<<"Calling ocr()"<<std::endl;
		std::string result = ocr(croppedRegion);
		std::cout<<"Finished"<<std::endl;
		ocr_result += result + " ";
		    
		  //  imwrite("cropped2.jpg", cropped); 
		    
		    
		    //imshow("cropped image ", frame);
		    //waitKey(7000);
            
             //   for (int j = 0; j < 4; ++j)
               //    line(frame, vertices[j], vertices[(j + 1) % 4], Scalar(0, 255, 0), 2, LINE_AA);
        }
        
        std::cout <<"Final Text -> " <<ocr_result<<std::endl;
        std::cout<<"Finished outer for loop indices.size()"<<std::endl;
      //  return 0;
}

std::string ocr(cv::Mat& image) {
	std::cout<<"--------- In ocr() ---------"<<std::endl;
	std::string outText;
	// Create a Tesseract OCR engine and set its parameters
	tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
	ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
	ocr->SetPageSegMode(tesseract::PSM_AUTO);

	// Convert the OpenCV image to a Pix image
	//  Pix* pix = pixCreate(image.cols, image.rows, 8);
	//pixSetData(pix, reinterpret_cast<l_uint32*>(image.data));
	// pixSetWpl(pix, image.step / 4);

	// Perform OCR on the Pix image
	//   ocr.SetImage(pix);
	//img.cols
	
	std::cout<<"ocr setImage"<<std::endl;
	ocr->SetImage(image.data, image.cols, image.rows, 3, image.step);
	std::cout<<"Finished"<<std::endl;
	char *text = ocr->GetUTF8Text();

	outText = std::string(ocr->GetUTF8Text());
	std::cout<< "Ocr: " << outText << std::endl;
	//delete[] text;
	ocr->End();  
	return outText;
	//std::cout<< "Ocr: " << outText << std::endl;
	//std::cout << "OCR result: " << text << std::endl;

	// Clean up
	
}


void getFrame(){
	std::cout<<"-------- In getFrame() ---------" <<std::endl;
	while (true) {
		// Construct the command to capture a JPEG image
		char command[400];

		// 2>/dev/null
		// ; convert image.jpg -units PixelsPerInch -density 70 output.jpg
		std::cout<<"calling libcamera to capture image"<<std::endl;
		snprintf(command, sizeof(command), "libcamera-jpeg -o image.jpg --nopreview -t1000 2>/dev/null");
		std::cout<<"Finished"<<std::endl;

		// Launch the command and capture its output
		FILE* pipe = popen(command, "r");
		if (!pipe) {
		    perror("popen failed");
		    return;
		}

		// Wait for the command to finish
		pclose(pipe);

		// Load the captured image using OpenCV
		std::cout<<"Reading Image"<<std::endl;
		cv::Mat image = cv::imread("image.jpg", cv::IMREAD_COLOR);
		std::cout<<"Finished"<<std::endl;
		cv::Mat outimage;

		if (image.empty()) {
		    std::cout << "Failed to load image" << std::endl;
		    return;
		}
		
		std::cout<<"Resizing Image"<<std::endl;
		cv::resize(image, outimage, cv::Size(1024, 768), 0, 0);
		std::cout<<"Finished"<<std::endl;
		
		std::cout<<"Saving resize.jpg"<<std::endl;
		cv::imwrite("resize.jpg", outimage);
		std::cout<<"Finished"<<std::endl;


		//cv::cvtColor(outimage, outimage2, cv::COLOR_BGR2BGR);
		//  cv::imwrite("input_tiff.tiff", outimage);

		//  cv::Mat tiffimage = cv::imread("input_tiff.tiff"); 

		//      cv::imshow("Image", image);
		//	cv::waitKey(0); 

		std::cout<<"+++++++++++++++++++++++++++++++++ Calling getTextRegions() +++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
		getTextRegions();
		std::cout<<"++++++++++++++++++++++++++++++++ Finished OCR +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;

		// Perform OCR on the image
		// ocr(outimage);
		
		std::cout<<"Removing Images"<<std::endl;
		std::remove("resize.jpg");
		std::remove("image.jpeg");
		std::cout<<"Finished"<<std::endl;
		// std::remove("input_tiff.tiff");

		// Wait for one second before capturing the next image
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


/*espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_SYNCH_PLAYBACK;
char *path = NULL;
void* user_data;
unsigned int *identifier;
espeak_POSITION_TYPE position_type;


void tta(){
	char voicename[] = {"English"}; 
	char text[] = {"Hello world!"};
	int buflength = 500, options = 0;
	position_type = POS_CHARACTER;
	unsigned int position = 0, end_position = 0, flags = espeakCHARS_AUTO;

	espeak_Initialize(output, buflength, path, options);
	espeak_SetVoiceByName(voicename);
	printf("Saying  '%s'...\n", text);
	espeak_Synth(text, buflength, position, position_type, end_position, flags, identifier, user_data);
	printf("Done\n");
}*/


// g++ -I/usr/include/opencv4 ocr.cpp -o ocr -lespeak-ng `pkg-config --libs opencv4` -llept -ltesseract

int main(){
	std::cout<< "------------------------------ In Main ---------------------------------" <<std::endl;
	std::cout<<"Reading the net"<<std::endl;
	net = readNet("frozen_east_text_detection.pb");
	std::cout<<"Finished"<<std::endl;
	std::cout<<"Calling getFrame()"<<std::endl;
	getFrame();
	std::cout<<"Exit"<<std::endl;
	return 0;
}



    

