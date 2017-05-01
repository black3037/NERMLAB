/**
  ******************************************************************************
  * @file    main.c
  * @author  Derek Black (C) COPYRIGHT
  * @version V1.0
  * @date    3/11/17
  * @brief   NERMLAB Main File
  ******************************************************************************
*/
#include "main.h"
    
extern "C" void sys_tick_callback();

int main(void)
{
	/* Initalizations */
	InitUSART1();
	global_modes_t global_mode;
	global_mode = STOP;
	
	/*Setup SysTick */
        sys_timer_setup(10000,4);
	ticks = 0;
	lastTicks = 0;
	
	/* Initalize Tx and Rx Buffers */
	InitBuffer(&tx);
	InitBuffer(&rx);
	
	/* Start up Message */
        SendCharArrayUSART1(startText,strlen(startText));
	
	/* Check rx & tx buffers */
	while(1)
	{

		if (haveStr(&rx))
		{
			jsmn_parser parser;
			jsmntok_t tokens[50];
			jsmn_init(&parser);
			getStr(&rx, input_string);
			int numberTokens = jsmn_parse(&parser,input_string,strlen(input_string),tokens,50);
			extract_value(input_string,"Mode",mode,numberTokens,tokens);
			
			if (strcmp("RunWaveAutoSave",mode)==0)
			{
				SendCharArrayUSART1(runWaveAutoSaveStartUp,strlen(runWaveAutoSaveStartUp));
				global_mode = RUNWAVEAUTOSAVE;
			}				
			else if (strcmp("RunWaveContinuous",mode)==0)
			{
				SendCharArrayUSART1(runWaveContinousStartUp,strlen(runWaveContinousStartUp));
				global_mode = RUNWAVECONTINUOUS;
			}
			else
			{
				continue;
			}
		}
		
		switch(global_mode)
		{
			case RUNWAVEAUTOSAVE:
				global_mode = STOP;
				
				/* Get Values from JSON to Run Experiment */
				jsmn_parser parser;
				jsmntok_t tokens[50];
				jsmn_init(&parser);
				int numberTokens = jsmn_parse(&parser,input_string,strlen(input_string),tokens,50);
				extract_value(input_string,  "Kp",         Kp,         numberTokens, tokens);
				extract_value(input_string,  "Ki",         Ki,         numberTokens, tokens);
				extract_value(input_string,  "Kd",         Kd,         numberTokens, tokens);
				extract_value(input_string,  "WaveType",   waveType,   numberTokens, tokens);
				extract_value(input_string,  "Magnitude",  magnitude,  numberTokens, tokens);
				extract_value(input_string,  "Frequency",  frequency,  numberTokens, tokens);
				extract_value(input_string,  "Controller", controller, numberTokens, tokens);
				extract_value(input_string,  "Duration",   duration,   numberTokens, tokens);
				extract_value(input_string,  "SampleRate", sampleRate, numberTokens, tokens);
			
				break;
			
			case RUNWAVECONTINUOUS:
				break;
			
			case START:
				break;
			
			case STOP:
				break;
		}
	
	}

}

extern "C" void USART1_IRQHandler(void) 
{
	USARTCount++;	
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		static uint16_t cnt = 0;
		char t = USART1->DR;

		if ( !((t == '\n')||(t == '\r')) && (cnt < MAX_STRLEN) )
		{
			putChar(&rx,t);
			cnt++;
		}
		else
		{
			// Allows the \n character to be added to the buffer
			putChar(&rx,t);
			cnt = 0;
		}
	}
	if(USART_GetITStatus(USART1,USART_IT_TXE))
	{
		if(haveStr(&tx))
		{
			char c = getChar(&tx);
			USART1->DR = c;
		}
		if(tx.tail == tx.head)
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	}
}

extern "C" void sys_tick_callback()
{
  ticks++;
}


void sys_timer_setup(uint32_t sys_tick_frequency, uint8_t priority)
{
    RCC_ClocksTypeDef RCC_Clocks;

    // Use all bits of interrupt priority to be preemption (16 priority levels)
    SCB->AIRCR = 0x05FA0300;
    
    // SysTick configuration
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / sys_tick_frequency);
    
    // Should figure out how to do this with stdPeriph calls
    SCB->SHP[11]=priority<<4;
}



