#include <espeak-ng/speak_lib.h>
#include <string>
#include "speak.h"
#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <cstdio>

char *path = NULL;
void* user_data;
unsigned int *identifier;
espeak_POSITION_TYPE position_type;
espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_SYNCH_PLAYBACK;



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

//TODO: Check these parameters
void Speak::text_to_speech(string text){
   
   
   
   std::cout<<"In speak text to speech   "<< text.c_str()<<std::endl;
   char voicename[] = {"English"}; 
   int buflength = 1024, options = 0;
   position_type = POS_CHARACTER;
   unsigned int position = 0, end_position = 0, flags = espeakCHARS_AUTO;
   espeak_Initialize(output, buflength, path, espeakINITIALIZE_PHONEME_EVENTS);
   espeak_SetVoiceByName(voicename);
   espeak_Synth(text.c_str(), buflength, position, position_type, end_position, flags, identifier, user_data);
   //espeak_Synth(text.c_str(), std::strlen(text.c_str)), position, position_type, end_position, flags, identifier, NULL, NULL);
   std::cout<<"before"<<std::endl;
  // espeak_Cancel();
   std::cout<<"before 1"<<std::endl;
   //espeak_Synchronize();
   std::cout<<"before 2"<<std::endl;
   //espeak_Cancel();
   std::cout<<"before 3"<<std::endl;
   espeak_Terminate();
   std::cout<<"Last"<<std::endl;
   
   
  /* std::cout<<"In speak text to speech   "<< text.c_str()<<std::endl;
   if (text.empty()) {
      std::cout<<"in if speak" <<std::endl;
      char command[100];
      //std::string str = "could not detect";
      //const char cstr[] = "could not detect";
      std::string ss ="echo could not detect | festival --tts";
   
     // std::string cmd = ss;
      //std::cout<<""
      snprintf(command, sizeof(command), "%s", ss.c_str());

      std::cout<<"cmd " <<command<<std::endl;
      // Launch the command and capture its output
      FILE* pipe = popen(command, "r");
      if (!pipe) {
         perror("popen failed");
        //TODO: Output to speak the error  
      }
       
      // Wait for the command to finish
      std::cout<<"closing pipe"<<std::endl;
      pclose(pipe);
      std::cout<<"Finished"<<std::endl;
      
   } else {
   std::cout<<"in else speak" <<std::endl;
      std::string hh = remove_newline(text);
      
      std::cout <<"after rremove new line " << hh <<std::endl;


      // std::string input = "This is a test string with special characters #$&^%*!";
       std::string output = remove_special_chars(hh.c_str());
       
       
       std::cout<<"after pre " <<output <<std::endl;
      
      char command[100];
      std::string ss ="echo " + output + " | festival --tts";
      
     // std::string cmd = ss;
      //std::cout<<""
      snprintf(command, sizeof(command), "%s", ss.c_str());

      std::cout<<"cmd " <<command<<std::endl;
      // Launch the command and capture its output
      FILE* pipe = popen(command, "r");
      if (!pipe) {
         perror("popen failed");
        //TODO: Output to speak the error  
      }
       
      // Wait for the command to finish
      std::cout<<"closing pipe"<<std::endl;
      pclose(pipe);
      std::cout<<"Finished"<<std::endl;
   
   
}
   
   
   
 /*  std::cout<<"In speak text to speech   "<< text.c_str()<<std::endl;
   std::cout<<"Text size "<< text.size() <<std::endl;
   int i, j, k, flags=espeakCHARS_AUTO, done=0;
   unsigned int size;
   //char *buffer = NULL;
   espeak_Initialize(output, 1024, NULL, espeakINITIALIZE_PHONEME_EVENTS);
  // espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, NULL, 0);
   
   //espeak_SetParameter(espeakRATE, 150, 0);
   //espeak_SetParameter(espeakVOLUME, 80, 0);
   //espeak_SetParameter(espeakCHARS_AUTO, 1, 0);
   
   size = text.size() + 1;
   unique_ptr<char[]> buffer(new char[size]);
   memset(buffer.get(), 0, size); 
   memcpy(buffer.get(), text.c_str(), size-1);  
   
   espeak_Synth(buffer.get(), size, 0, POS_CHARACTER, 0, espeakENDPAUSE, NULL, NULL);
   std::cout<<"before syn and ter" <<std::endl;
   espeak_Synchronize();
   espeak_Terminate();
   
   std::cout<<"Finished speak"<<std::endl;*/
   
   
   
   
}
