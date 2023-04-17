#include <iostream>
#include <thread>
#include <wiringPi.h>
//#include <pigpio.h>

#include "../include/ocr.h"
#include "../include/od.h"
#include "../obstacle_detection.h"
#include "../include/speak.h"


#include <opencv2/opencv.hpp>
#include <cstdint>
#include <sstream>
#include <cstdio>

// TODO: Delete images

using namespace std;

// This flag is used to exit the main function after the user clicks on the poweroff button
bool flag = true;


// These flags are used to make sure that the callback function is called only once for one click
bool ocr_flag = true;
bool od_flag = true;
bool obs_flag = true;
bool poweroff_flag = true;
   
// Class objects   
OD od;
OCR ocr;
ObstacleDetection obs;
    
    
void poweroff(){
    if (poweroff_flag) {
        poweroff_flag = false;
        Speak speak;
        std::cout<<"Finishing main"<<std::endl;
        const char* filename = "example.png";
        if (std::remove(filename) != 0) {
            speak.text_to_speech("Error deleting image")
        } else {
            speak.text_to_speech("Image successfully deleted");
        }
        flag = false;
        speak.text_to_speech("EmbeddedIris is shutting down");
    }
}


void start_ocr(){
    std::cout<<"start_ocr"<<std::endl;
    if (ocr_flag) {
        ocr_flag = false;
        std::cout<<"start_ocr"<<std::endl;
        ocr.start();
        ocr_flag=true;
    }
}

void start_obs(){
    std::cout<<"start_obs"<<std::endl;
    if (obs_flag) {
        obs_flag = false;
        std::cout<<"start_obs"<<std::endl;
        obs.start();
        obs_flag = true;
    }
}

void start_od(){
    std::cout<<"start_od"<<std::endl;
    if(od_flag) {
        od_flag = false;
        std::cout<<"start_od"<<std::endl;
        od.start();
        od_flag = true;
    }
}

int main() {
    std::cout<<" In main "<<std::endl;
    
    wiringPiSetup();
    Speak speak;
    speak.text_to_speech("EmbeddedIris is now ready to use");
    
    // Object Detection -> GPIO 18 
    pinMode(13, INPUT); 
    std::cout<<" Setting pin 9 for interrupt "<<std::endl;
    wiringPiISR(13, INT_EDGE_RISING, start_obs); 
    
    // OCR -> GPIO
    pinMode(14, INPUT); 
    std::cout<<" Setting pin 8 for interrupt "<<std::endl;
    wiringPiISR(14, INT_EDGE_RISING, start_ocr); 
    
    // Obstacle Detection -> GPIO
    pinMode(3, INPUT); 
    std::cout<<" Setting pin 7 for interrupt "<<std::endl;
    wiringPiISR(3, INT_EDGE_RISING, start_obs);
    
    // Poweroff
    pinMode(12, INPUT); 
    std::cout<<" Setting pin 12 for interrupt "<<std::endl;
    wiringPiISR(12, INT_EDGE_RISING, poweroff);
    
    std::cout<<"while"<<std::endl;
    while(flag) {}
    std::cout<<"after while"<<std::endl;
   
    return 0;
}
