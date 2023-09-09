#include "FIRFilter.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "mysofa.h"

#define FIR_FILTER_LENGTH 256
#define SAMPLE_RATE 41000
#define PI 3.14159265358979

void FIRFilter_Init(FIRFilter *filter, float limpulse[], float rimpulse[]) {
    //Assign Impulse Response
    //open SOFA file
    //Assign variables
    //Clear all buffers
    for (int i = 0; i < 256; i++) {
        filter->circ_queue[i] = 0;
        filter->left_IR[i] = limpulse[i];
        filter->right_IR[i] = rimpulse[i];
    }


    filter->rotate_index = 1;
    filter->index = 0;
    filter->output[0] = 0.0f;
    filter->output[1] = 0.0f;
    return;
}

void FIRFilter_Apply(FIRFilter *filter, int16_t input) {
    // Enter newest value
    //if (input == -42) printf("%d FOUND IT\n", input);
    filter->circ_queue[filter->index] = input;
    
    
    
    // Increment index and return to 0 if greater than length
    filter->index++;
    if (filter->index == FIR_FILTER_LENGTH) {
        filter->index = 0;
    }
    
    //Set new output values
    filter->output[0] = 0.0f;
    filter->output[1] = 0.0f;

    //Set temporary index
    int tempIndex = filter->index;
    
    //printf("%d fuckishit \n", filter->circ_queue[0]);

    for (int i = 0; i < FIR_FILTER_LENGTH; i++) {
        //printf("%f fuck off %d \n", filter->left_IR[i], i);
        if (tempIndex > 0) {
            tempIndex--;
        } else{
            tempIndex = FIR_FILTER_LENGTH-1;
        }
        // Muliply IR value to Next oldest Input sample
        filter->output[0] += (float)filter->circ_queue[tempIndex] * filter->left_IR[i];
        
        filter->output[1] += filter->right_IR[i] * (float)filter->circ_queue[tempIndex];
        //printf("%f fuck %d cumsicles %f\n", filter->output[0], filter->circ_queue[tempIndex], filter->left_IR[i]);
    }
    return;
}

void FIRFilter_ChangeFilter(FIRFilter *filter, float limpulse[], float rimpulse[]){

    for (int i = 0; i<FIR_FILTER_LENGTH; i++) {
        filter->left_IR[i] = limpulse[i];
        filter->right_IR[i] = rimpulse[i];
    }
    
    return;
}
