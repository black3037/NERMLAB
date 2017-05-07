
// Includes
#include "physical_constants.h"
#include "nerm_lab_constants.h"

// Number of motor pole pairs
const float NUM_POLE_PAIRS = 7.0f;

// Angle that motor shaft turns (in radians) per encoder tick
const float ENCODER_2_ANGLE = 2.0f * PI / 2000.0f;

// Max duty cycle allowed on motor phases, could affect current measurment
const float MAX_DUTY = 0.95f;

// Bus voltage
const float VBUS = 12.0f;

// Max d-q axis voltage
const float MAX_VOLTAGE =  1.224745f*(MAX_DUTY - 0.5f)*2.0f*VBUS;

// q-axis voltage used during calibration of commutation
const float CALIBRATE_VOLTAGE = 5.0f;

// Global params
char mode      [254];
char Kp        [254];
char Ki        [254];
char Kd        [254];
char waveType  [254];
char magnitude [254];
char frequency [254];
char controller[254];
char duration  [254];
char sampleRate[254];