#ifndef NOTEBOOK_USART_H
#define NOTEBOOK_USART_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

void InitUSART1(void);
void SendCharArrayUSART1(char arr[],int len);
void GetCharArrayUSART1();
void SendArrayUSART1(uint16_t array[], int len);
void SendMatrixUSART1(char mat[3][4], int row, int col);

	
#endif
