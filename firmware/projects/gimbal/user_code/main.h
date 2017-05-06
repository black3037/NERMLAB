/*----------------------------------------------------------------------------
 * Name:    main.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/

#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "USART.h"
#include "circularBuffer.h"
#include <string.h>
#include "jsmn.h"
#include "error.h"
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

/**************************************************/
/******************** Defines *********************/
#define MAXINT (1<<32)
#define MAX_STRLEN 2048

/**************************************************/
/******************** Global **********************/
typedef enum {
RUNWAVEAUTOSAVE   = 0,
RUNWAVECONTINUOUS = 1,
START             = 2,
STOP              = 3,
MANUAL            = 4,
COLLECTDATA       = 5,
STARTANDCOLLECT   = 6,
RESETENCODERS     = 7,
INIT              = 8
} global_modes_t;

/**************************************************/
/******************** Prototypes ******************/
void sys_timer_setup(uint32_t sys_tick_frequency, uint8_t priority);
extern "C" void sys_tick_callback();
void requestDataParams(void);
void sendDataRequest(void);
void getAllValues(void);
void getMode(void);

/* Global variables */
static uint32_t ticks         = 0;
static uint32_t lastTicks     = 0;
static uint32_t USARTCount    = 0;
float encoderAngle            = 0.0f;
float lastEncoderAngle        = 0.0f;
char JSONOutputString[MAX_STRLEN+1];
char encoderAngle2String[MAX_STRLEN+1];

/* Start up Messages */
char* startText               = "{\"NERMLAB\":\"Connected!\"}\n";
char* runWaveAutoSaveStartUp  = "{\"NERMLAB\":\"Run Wave Autosave Initalized\"}\n";
char* runWaveContinousStartUp = "{\"NERMLAB\":\"Run Wave Continous Initalized\"}\n";

/* Buffer strings */
char input_string[MAX_STRLEN+1];

/* JSON Parsing Strings */
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

/* Global Structs */
commBuffer_t rx;
commBuffer_t tx;

/* Global Objects */
PwmOutAdvancedTimer motor_pwm;
PwmMeasurementTimer as5048_input(1);
DigitalOut enableA(PC10);
DigitalOut enableB(PC11);
DigitalOut enableC(PC12);
Encoder encoder(EncoderA);

/* Test Messages */
char A[3][4]=
{
	{1, 5, 6, 7},
	{4, 4, 8, 0},
	{2, 3, 4, 5}
};
char* debugMessage = "Debug Message\n";


/* SOME OLD CODE 

getStr(&rx, input_string);
USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

if (strcmp("test\n",input_string)==0)
{
	
	SendMatrixUSART1(A,3,4);
}

*/


#endif
