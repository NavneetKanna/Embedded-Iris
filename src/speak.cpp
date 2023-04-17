#include <espeak-ng/speak_lib.h>
#include <string>
#include "../include/speak.h"
#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <cstdio>

bool Speak::is_special_char(char c)
{
    // List of special characters that you want to remove
    std::string special_chars = "[]{}()<>*%$#@!^&-_=+\\|;:\"',.?/";
    return special_chars.find(c) != std::string::npos;
}

std::string Speak::remove_special_chars(const std::string& str)
{
    std::string result;
    for (const auto& c : str) {
        if (!is_special_char(c)) {
            result += c;
        }
    }
    return result;
}

std::string Speak::remove_newline(std::string te){
   std::string str = te;
   std::size_t pos = str.find("\n");
   while (pos != std::string::npos) {
      str.erase(pos, 1);
      pos = str.find("\n", pos);  
       
   } 
   
   return str;   
}

void Speak::text_to_speech(string text){   
    
   std::cout<<"In speak text to speech   "<< text.c_str()<<std::endl;
   std::cout<<"text size " << text.size() <<std::endl;

   if (text.empty()) {
      char command[100];
      std::string ss ="echo could not detect | festival --tts";

      snprintf(command, sizeof(command), "%s", ss.c_str());

      std::cout<<"cmd " <<command<<std::endl;
      // Launch the command and capture its output
      FILE* pipe = popen(command, "r");
      // Highly "Unlikely" this will happend
      if (!pipe) {
         std::cout("popen failed");
      }
       
      // Wait for the command to finish
      std::cout<<"closing pipe"<<std::endl;
      pclose(pipe);
      std::cout<<"Finished"<<std::endl;
      
   } else {
      std::string hh = remove_newline(text);

      std::string output = remove_special_chars(hh.c_str());
      
      char command[100];
      std::string ss ="echo " + output + " | festival --tts";

      snprintf(command, sizeof(command), "%s", ss.c_str());

      // Launch the command and capture its output
      FILE* pipe = popen(command, "r");
      // Highly "Unlikely" this will happend
      if (!pipe) {
         std::cout<<"popen failed"<<std::endl; 
      }
       
      // Wait for the command to finish
      std::cout<<"closing pipe"<<std::endl;
      pclose(pipe);
      std::cout<<"Finished"<<std::endl;
   }
}