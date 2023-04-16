#include <iostream>
#include <thread>
#include <wiringPi.h>
//#include <pigpio.h>

#include "ocr.h"
#include "od.h"
#include "obstacle_detection.h"

#include <opencv2/opencv.hpp>
#include <cstdint>

#include <sstream>
//#include "speak.h"

// TODO: Delete images

using namespace std;
bool flag = true;
bool oflag = true;

bool is_running = false; 
bool is_sensor_on = false; 
   
   
OD od;
OCR ocr;
ObstacleDetection obs;
    
    
void poweroff(){
std::cout<<"Finishing main"<<std::endl;
//flag = false;
}


void start_ocr(){

    oflag = false;
 std::cout<<"start_ocr"<<std::endl;
 ocr.start();


}

void start_obs(){
  std::cout<<"start_obs"<<std::endl;
  obs.begin();
}

void start_od(){
 
        oflag = false;
  std::cout<<"start_od"<<std::endl;
  od.start();
}

int main() {
    std::cout<<" In main "<<std::endl;
    
    wiringPiSetup();
  //  Speak speak;
   // speak.text_to_speech("EmbeddedIris is now ready to use");
    
    // Object Detection -> GPIO 18 
    pinMode(9, INPUT); 
    std::cout<<" Setting pin 9 for interrupt "<<std::endl;
   wiringPiISR(9, INT_EDGE_RISING, start_od); 
   // wiringPiISR(1, INT_EDGE_RISING, [&od]() { std::thread od_t(&OD::predict, &od); od_t.detach(); }); 
    
    
    // OCR -> GPIO
    pinMode(8, INPUT); 
    std::cout<<" Setting pin 8 for interrupt "<<std::endl;
    wiringPiISR(8, INT_EDGE_RISING, start_ocr); 
   //wiringPiISR(1, INT_EDGE_RISING, [&ocr]() { std::thread ocr_t(&OCR::perform_ocr, &ocr); ocr_t.detach(); }); 
    
    
    // Obstacle Detection -> GPIO
    pinMode(7, INPUT); 
    std::cout<<" Setting pin 7 for interrupt "<<std::endl;
   wiringPiISR(7, INT_EDGE_RISING, start_obs);
   /*wiringPiISR(1, INT_EDGE_RISING, [&obsdetect, &is_sensor_on]() { 
       
    });*/
    
    // Poweroff
   pinMode(12, INPUT); 
   std::cout<<" Setting pin 12 for interrupt "<<std::endl;
   wiringPiISR(12, INT_EDGE_RISING, poweroff);
   //wiringPiISR(12, INT_EDGE_RISING, start_od);
    
    
    std::cout<<"while"<<std::endl;
    while(flag) {}
    std::cout << "flag  " << flag<<std::endl;
    std::cout<<"after while"<<std::endl;
	
   

   // return 0;
}
