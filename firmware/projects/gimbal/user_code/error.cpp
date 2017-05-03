#include "error.h"

void throwException(uint8_t error)
{
    if (error == 0)
    {
        SendCharArrayUSART1("ERROR CODE 0: NO ERRORS", strlen("ERROR 0: NO ERRORS"));
    }
    else if (error == 1)
    {
        SendCharArrayUSART1("ERROR CODE 1: NONVALID JSON STRING (MOST LIKELY DATA STREAM INTERRUPTED)", strlen("ERROR CODE 1: NONVALID JSON STRING (MOST LIKELY DATA STREAM INTERRUPTED)"));
    }
    else if (error == 2)
    {
        SendCharArrayUSART1("ERROR CODE 2: PROCESS INTERRUPTED", strlen("ERROR CODE 2: PROCESS INTERRUPTED"));
    }
    else
    {
        SendCharArrayUSART1("ERROR CODE NULL: CANNOT DETERMINE ERROR TYPE", strlen("ERROR CODE NULL: CANNOT DETERMINE ERROR TYPE"));
    }

}