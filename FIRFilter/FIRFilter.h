#ifndef FIR_FILTER
#define FIR_FILTER

#include <stdint.h>
#include "mysofa.h"

#define FIR_FILTER_LENGTH 256

typedef struct {
    struct MYSOFA_EASY *hrtf;
    unsigned int rotate_index;
    float ldelay;
    float rdelay;

    float x;
    float y;
    float z;

    float left_IR[FIR_FILTER_LENGTH];
    float right_IR[FIR_FILTER_LENGTH];

    int16_t circ_queue[FIR_FILTER_LENGTH];
    int index;
    
    float output[2];
} FIRFilter;

void FIRFilter_Init(FIRFilter *filter, float limpulse[], float rimpulse[]);
void FIRFilter_Apply(FIRFilter *filter, int16_t input);
void FIRFilter_ChangeFilter(FIRFilter *filter, float limpulse[], float rimpulse[]);

#endif
