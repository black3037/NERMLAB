#ifndef _GLOBALS_H
#define _GLOBALS_H

/* Global Variables */
#define MAX_STRLEN    2048

#include "pwm_out_advanced_timer.h"
#include "digital_out.h"
#include "trigtables.h"
#include "physical_constants.h"
#include "math.h"
#include "math_util.h"
#include "pwm_measurement_timer.h"
#include "nerm_lab_constants.h"
#include "motor_commutation_calibration.h"
#include "encoder.h"
#include "circularBuffer.h"

char JSONOutputString[MAX_STRLEN+1];
char encoderAngle2String[MAX_STRLEN+1];
extern float encoderAngle;      
extern float lastEncoderAngle;  

/* Buffer strings */
char input_string[MAX_STRLEN+1];


#endif

