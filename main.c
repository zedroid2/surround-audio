#include "portaudio.h"
//#include "dependencies/portaudio.h"
#include <math.h>
#include "FIRFilter/FIRFilter.h"
//#include "dependencies/mysofa.h"
#include "mysofa.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <raylib.h>

#define SAMPLE_RATE 44100
#define PI 3.14159265358979


// USER DATA STRUCT
typedef struct {
    int16_t *input_data;
    FIRFilter *current_filter;
    int frameIndex;
    int maxFrameIndex;
}paData;


// GET WAV FILE
void wavOpen(const char *filename, paData *samples) {
    //variables
    FILE *file;
    char temp[4];
    uint32_t filesize;
    uint32_t format_length;
    uint16_t format_type;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t bytes_per_second;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_size;

    file = fopen(filename, "rb");
    if (file == NULL) printf("%s failed to open", filename);

    fread(temp, 1, 4, file);
    fread(&filesize, 4, 1, file);
    fread(temp, 1, 4, file);
    fread(temp, 1, 4, file);
    fread(&format_length, 4, 1, file);
    fread(&format_type, 2, 1, file);
    fread(&num_channels, 2, 1, file);
    fread(&sample_rate, 4, 1, file);
    fread(&bytes_per_second, 4, 1, file);
    fread(&block_align, 2, 1, file);
    fread(&bits_per_sample, 2, 1, file);
    fread(temp, 1, 4, file);
    fread(&data_size, 4, 1, file);

    samples->input_data = malloc(data_size*2);
    fread(samples->input_data, 1, data_size, file);
    samples->maxFrameIndex = ((data_size*8)/(bits_per_sample*num_channels)) -1;
    if (file!=NULL) {
        fclose(file);
    }
    return;
}

static int patestCallback(const void *inputBuffer,
                     void *outputBuffer, 
                     unsigned long framesPerBuffer, 
                     const PaStreamCallbackTimeInfo* timeInfo,
                     PaStreamCallbackFlags statusFlags,
                     void *userData) {
    // casting etc etc
    paData *data = (paData*)userData;
    int16_t *rptr = &data->input_data[data->frameIndex];
    int16_t *wptr = (int16_t*)outputBuffer;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    unsigned int i;
    int finished;

    //reset output data

    //assign new filter for data
    FIRFilter *filter_ptr = data->current_filter;
    //printf("FUCKING CUNT %f \n", filter_ptr->left_IR[0] );
    
    

    // reset output data
    if (framesLeft < framesPerBuffer) {
        // FINISH BUFFER
        for (i = 0; i<framesLeft; i++){
            FIRFilter_Apply(data->current_filter, *rptr);
            *wptr++ = (filter_ptr)->output[0];
            *wptr++ = (filter_ptr)->output[1];
            
        }
        finished = paContinue;
        data->frameIndex = 0;
    } else {
        for (i = 0; i<framesPerBuffer; i++){
            FIRFilter_Apply(data->current_filter, *rptr);
            //printf("%d why %d\n", i, (filter_ptr)->output[0]);
            *wptr++ = (filter_ptr)->output[0];
            *wptr++ = (filter_ptr)->output[1];
            rptr+= 1;
            //printf("input %d, output %f", *rptr, *wptr);
            //printf("FUCK %d\n", *rptr);
        }
        finished = paContinue;
        data->frameIndex += framesPerBuffer;
    }
    //FIRFilter_ChangeFilter(data->current_filter);
    return finished;
}

int main() {
    // CONFIGURATION OF FILTER
    // DATA INITIALISATION
    int filter_length;
    int err;
    float ldelay;
    float rdelay;
    float left_IR[256];
    float right_IR[256];
    struct MYSOFA_EASY *hrtf = NULL;
    hrtf = mysofa_open("sofa_files/MRT01.sofa", 44100, &filter_length, &err);
    mysofa_getfilter_float(hrtf, 0, 1, 0, left_IR, right_IR, &ldelay, &rdelay);

    // CREATE FILTER
    FIRFilter current_filter;
    FIRFilter_Init(&current_filter, left_IR, right_IR);

    // setup PortAudio data and parameters
    paData data;
    data.frameIndex = 0;
    wavOpen("guitartune.wav", &data); // Wave file open
    data.current_filter = &current_filter;
    PaStreamParameters outputParameters;

    //Initialise PortAudio
    PaError err2 = Pa_Initialize();
    if (err2 != paNoError) printf("PortAudio Initialise error: %s\n", Pa_GetErrorText(err2));

    // Opening stream
    PaStream *stream;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paInt16; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err2 = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              250,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              patestCallback,
              &data );
    if (err2 != paNoError) printf("PortAudio Stream Error: %s \n", Pa_GetErrorText(err2));
    
    //CHANGE the filter

    err2 = Pa_StartStream(stream);
    if (err2 != paNoError) printf("PortAudio startstream error: %s\n", Pa_GetErrorText(err2));

    //-----------------------------------------------------------------
    // SETUP RAYLIB

    const int screenWidth = 450;
    const int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Main");

    // Setup camera object

    Camera camera = {0};
    camera.position = (Vector3) {10.0f, 10.0f, 10.0f};
    camera.target = (Vector3) {0.0f, 0.0f, 0.0f};
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    DisableCursor();
    SetTargetFPS(144);

    // Setup Cube Positions
    Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
    Vector3 speakerPosition = {4.0f, 0.0f, 0.0f};

    // Setup ratios for max 1 distance of speaker
    float mag = 1.0f;

    // Loop while the program continues to run;
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        // Draw cube
        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        // Draw speaker
        DrawCube(speakerPosition, 0.5f, 0.5f, 0.5f, BLUE);
        DrawCubeWires(speakerPosition, 0.5f, 0.5f, 0.5f, RED);
        DrawGrid(10, 1.0f);
        EndMode3D();
        EndDrawing();
        
        Pa_Sleep(100);
        
        // Reset left and right IRs
        memset(left_IR, 0, sizeof(left_IR));
        memset(right_IR, 0, sizeof(right_IR));


        mysofa_getfilter_float(hrtf, camera.position.x, -camera.position.z, camera.position.y, left_IR, right_IR, &ldelay, &rdelay);
        FIRFilter_ChangeFilter(data.current_filter, left_IR, right_IR);
        
    }

    // Close Raylib Window

    CloseWindow();

    // END Raylib
    //-----------------------------------------------------------------

    /*float x = 0.0f;
    float y = 1.0f;
    float z = 0;
    for (int j = 1; j < 15; j++) {
        for (int i = 1; i < 21; i++) {
            Pa_Sleep(300);
            //err2 = Pa_StopStream(stream);
            float temp = x;
            x = x*cos(PI/30) - y*sin(PI/30);
            y = x*sin(PI/30) + y*cos(PI/30);
            printf("%f f%f\n", x, y);
            memset(left_IR, 0, sizeof(left_IR));
            memset(right_IR, 0, sizeof(right_IR));
            mysofa_getfilter_float(hrtf, x, y, z, left_IR, right_IR, &ldelay, &rdelay);
            FIRFilter_ChangeFilter(data.current_filter, left_IR, right_IR);
            
            //err2 = Pa_StartStream(stream);
        }
    }*/
    Pa_Sleep(10000);
    

    printf("%s \n", "heyyy");
    err2 = Pa_StopStream(stream);
    if (err2 != paNoError) printf("PortAudio stopstream error: %s\n", Pa_GetErrorText(err2));

    

    err2 = Pa_CloseStream(stream);
    if (err2 != paNoError) printf("PortAudio Terminate error: %s\n", Pa_GetErrorText(err2));

    // Terminate PortAudio
    err2 = Pa_Terminate();
    if (err2 != paNoError) printf("PortAudio Terminate error: %s\n", Pa_GetErrorText(err2));

    
    
    return 0;
}
