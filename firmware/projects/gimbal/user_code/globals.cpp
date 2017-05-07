#include "globals.h"

/* Global variables */
float encoderAngle            = 0.0f;
float lastEncoderAngle        = 0.0f;

/* Start up Messages */
char* startText               = "{\"NERMLAB\":\"Connected!\"}\n";
char* runWaveAutoSaveStartUp  = "{\"NERMLAB\":\"Run Wave Autosave Initalized\"}\n";
char* runWaveContinousStartUp = "{\"NERMLAB\":\"Run Wave Continous Initalized\"}\n";

/* Test Messages */
char A[3][4]=
{
	{1, 5, 6, 7},
	{4, 4, 8, 0},
	{2, 3, 4, 5}
};
char* debugMessage = "Debug Message\n";