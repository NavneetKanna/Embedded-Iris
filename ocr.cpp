#include <espeak-ng/speak_lib.h>
#include <iostream>
#include "ocr.h"

void getFrame(){
    // Will add the code after I get the device
}


espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_SYNCH_PLAYBACK;
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
}