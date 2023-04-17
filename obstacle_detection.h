#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include <thread>


class ObstacleDetection {
 public:
   ObstacleDetection();
   void detect();
   //void start();
   void stop();
   void start();
   bool is_running = true; 
   bool is_sensor_on = false; 
   bool once = true;
   std::thread thread_;
   std::thread stop_thread_;
};
