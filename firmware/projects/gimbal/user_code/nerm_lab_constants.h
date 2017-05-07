#ifndef NERM_LAB_CONSTANTS_H_INCLUDED
#define NERM_LAB_CONSTANTS_H_INCLUDED

#include <cstdint>

// See *.cpp file for documentation.
extern const float NUM_POLE_PAIRS;
extern const float ENCODER_2_ANGLE;
extern const float MAX_DUTY;
extern const float VBUS;
extern const float MAX_DQ_VOLTAGE;
extern const float CALIBRATE_VOLTAGE;
extern const float CURRENT_SENSE_SCALE;
extern const float CURRENT_SENSE_OFFSET;
extern const float RAD_2_DEG;
extern const float MAX_VOLTAGE;

/* Global Params */
extern char mode      [254];
extern char Kp        [254];
extern char Ki        [254];
extern char Kd        [254];
extern char waveType  [254];
extern char magnitude [254];
extern char frequency [254];
extern char controller[254];
extern char duration  [254];
extern char sampleRate[254];

#endif
