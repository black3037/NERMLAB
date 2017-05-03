#ifndef _ERROR_H
#define _ERROR_H

#include <cstdint>
#include "USART.h"
#include <string.h>

// See *.cpp file for documentation.
void throwException(uint8_t error);

#endif
