#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include "obstacle_detection.h"
#include <thread>
#include "speak.h"
#include <chrono>

#define TRIG_PIN 6
#define ECHO_PIN 10
using namespace std;

ObstacleDetection::ObstacleDetection(){
   
   wiringPiSetup();
   pinMode(TRIG_PIN, OUTPUT);
   pinMode(ECHO_PIN, INPUT);
   std::cout<<"Finsihed Init of obstacle detection"<<std::endl;
}


void ObstacleDetection::start(){
    std::cout<<"in obs begin"<<std::endl;

    is_sensor_on = !is_sensor_on;
    std::cout<<"is sensor on "<< is_sensor_on <<std::endl;
    if(is_sensor_on) {
	std::cout<<"--------------- Start ---------" <<std::endl;
	is_running = true;
	std::cout<<"calling obs thred"<<std::endl;
	thread_ = std::thread(&ObstacleDetection::detect, this);
	thread_.detach();
	std::cout<<"detached"<<std::endl;
    }
       
    else {
	std::cout<<"--------------- Stop ---------" <<std::endl;
	std::cout<<"calling thread to join"<<std::endl;
	stop_thread_ = std::thread(&ObstacleDetection::stop, this);
	stop_thread_.detach();
	std::cout<<"Finished obs thread join 2"<<std::endl;
    }
}

void ObstacleDetection::stop(){
  std::cout<<"In ObstacleDetection stop"<<std::endl;
  is_running = false;
  //thread_.join();
  if(thread_.joinable()){
     thread_.join();
  }
  std::cout<<"Finished obs thread join 1"<<std::endl;
}

void ObstacleDetection::detect() {
    std::cout<<"In ObstacleDetection detect"<<std::endl;
    Speak speak;
    while (is_running) {
	// Send a 10us pulse to trigger the ultrasonic sensor
	digitalWrite(TRIG_PIN, LOW);
	usleep(2);
	digitalWrite(TRIG_PIN, HIGH);
	usleep(10);
	digitalWrite(TRIG_PIN, LOW);

	// Wait for the ECHO_PIN to go HIGH, and record the time
	while (digitalRead(ECHO_PIN) == LOW) {}
	uint32_t start_time = micros();

	// Wait for the ECHO_PIN to go LOW, and record the time
	while (digitalRead(ECHO_PIN) == HIGH) {}
	uint32_t end_time = micros();

	// Calculate the distance based on the time it took for the sound wave to travel
	uint32_t duration = end_time - start_time;
	int distance = duration * 0.0343 / 2;
	Speak speak;
	// Print the distance to the console
	if(100 > distance && distance > 50) {
	    std::cout<<"Not Close"<<std::endl;
	    string text = "An obstacle is detected " + std::to_string(distance) + " centimeter ahead you";
	    speak.text_to_speech(text);
	    //std::cout << "An obstacle is detected " << distance << " centimeter before you" << std::endl;
	    once = true;
	}
  
	else if (once && distance < 50) {
	    std::cout<<"close"<<std::endl;
	    string text = "An obstacle is detected very close to you";
	    speak.text_to_speech(text);
	    std::cout <<"An obstacle is detected very close to you"<<std::endl;
	    once = false;
	}

	// Wait for 1 second before taking another measurement
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	sleep(1);
    }
    std::cout<<"After while in obs"<<std::endl;
}

