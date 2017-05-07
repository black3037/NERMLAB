#ifndef _MESSAGE_PARSER_H
#define _MESSAGE_PARSER_H

#include <jsmn.h>
#include "USART.h"

/* Buffer strings */
extern char input_string[2049];

// See *.cpp file for documentation.
void requestDataParams(void);
void sendDataRequest(void);
void getAllValues(void);
void getMode(void);

#endif
