#include "USART.h"

void InitUSART1(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)
	
  /* USART1 configured as follow:
	- BaudRate = 115200 baud
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
   */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

   /* Enable UART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

  {
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  }

  {
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  }

  /* USART configuration */
  USART_Init(USART1, &USART_InitStructure);
	
	
	/* Here the USART6 receive interrupt is enabled
	 * and the interrupt controller is configured
	 * to jump to the USART6_IRQHandler() function
	 * if the USART6 receive interrupt occurs
	 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);           // enable the USART6 receive interrupt

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		     // we want to configure the USART6 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART6 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		   // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			     // the USART6 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							             // the properties are passed to the NVIC_Init function which takes care of the low level stuff
	
    
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}

void SendCharArrayUSART1(char arr[], int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
			
		USART_SendData(USART1, arr[i]);
	}
}

void SendArrayUSART1(uint16_t array[], int len)
{
	for(int i = 0; i < len; i++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
		char value = (char)(array[i] % 10) + '0'; //convert integer to character
		USART_SendData(USART1, value);
	}
}

void SendMatrixUSART1(char mat[3][4], int row, int col)
{
	int i, j;
	char newLine[] = "\n";
	for (i = 0; i < row; i++)
	{	
		for (j = 0; j < col + 1; j++)
		{
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
			char value = (char)(mat[i][j] % 10) + '0'; //convert integer to character
			USART_SendData(USART1, value);
		}
		USART_SendData(USART1, newLine[0]);
	}
}

void GetCharArrayUSART1(char arr[], int len)
{
		USART_ReceiveData(USART1);
}

