#include <stm32f4xx.h>
#include "circularBuffer.h"

void InitBuffer(commBuffer_t* comm)
{
	comm->tail = 0;
	comm->head = 0;
	comm->size = 0;
}

uint32_t bufferSize(commBuffer_t* comm)
{
	return (comm->head - comm->tail + (MAXCOMMBUFFER)) % (MAXCOMMBUFFER);
}

void checkBufferOverflowStatus(commBuffer_t* comm)
{
	if (comm->head > MAXCOMMBUFFER)
	{
		comm->head = 0;
	}
	if (comm->tail > MAXCOMMBUFFER)
	{
		comm->tail = 0;
	}
}


uint8_t haveStr(commBuffer_t* comm)
{
	uint8_t buffer_status = 0;
	
	for (int i = comm->tail; i < ((comm->tail + comm->size)); i++)
	{
		int inc = i%MAXCOMMBUFFER;
		
		if (comm->buffer[inc] == '\n')
		{
			buffer_status = 1;
		}
	}
	return buffer_status;
}

void putChar(commBuffer_t* comm, char ch)
{
		if (comm->size < MAXCOMMBUFFER - 1)
		{	
			comm->buffer[comm->head] = ch;
			comm->size++;
			comm->head = (comm->head + 1)%MAXCOMMBUFFER;
		}
		else if (comm->size == MAXCOMMBUFFER - 1)
		{
			comm->buffer[comm->head]='\n';
			comm->size++;
			comm->head = (comm->head + 1)%MAXCOMMBUFFER;
		}
}

char getChar(commBuffer_t* comm)
{
	char out=comm->buffer[comm->tail];
	comm->size--;
	comm->tail = (comm->tail + 1)%MAXCOMMBUFFER;
	return out;
}

void putStr(commBuffer_t* comm, char* str)
{
	uint8_t length = strlen(str);
	for (uint32_t i = 0; i < length; i++)
	{
		putChar(comm, str[i]);
	}
	putChar(comm,'\n');
}

void getStr(commBuffer_t* comm, char* str)
{
	char val = comm->tail;
	uint32_t i = 0;
	while(comm->size != 0)
	{
			char c = getChar(comm);
			if (c != '\n')
			{
				str[i]= c;
			
			}
			else
			{
				str[i] = c;
				str[i+1] = 0x0;
				break;
			}
			val = (val + 1)%MAXCOMMBUFFER;
			i++;
	}
}









