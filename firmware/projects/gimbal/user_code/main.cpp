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
    
int main(void)
{
	/* Initalizations */
	InitUSART1();
        global_modes_t global_mode;
        global_mode = STOP;
      
	/*Setup SysTick */
        sys_timer_setup(10000,4);

	/* Initalize Tx and Rx Buffers */
	InitBuffer(&tx);
	InitBuffer(&rx);
        
        /* Wait for GUI to respond */
        while(1)
        {
            if (haveStr(&rx)) 
            {
                getStr(&rx, input_string);
                getMode();
                if (strcmp("GUI Start",mode)==0)
                {     
                    break;
                }
            }
            /* Need to fix this. Sends out reqest to GUI to make sure its not already connected
            else
            {
                char* isConnected = "{\"NERMLAB\":\"ISCONNECTED\"}\n";
                SendCharArrayUSART1(isConnected,strlen(isConnected));
                lastTicks = ticks;
                while(ticks-lastTicks > 10000);
                if (haveStr(&rx)) 
                {
                    getStr(&rx, input_string);
                    getMode();
                    if (strcmp("True",mode)==0)
                    {     
                        break;
                    }
                }
            }
            */
        }


        /* Send out a request to GUI for starting values */
        sendDataRequest();
	
        /* Loop waiting for user input */
	while(1)
	{
            if (haveStr(&rx))
            {
                    getStr(&rx, input_string);
                    getMode();
                    
                    /* Parse incoming string and set global mode */
                    if (strcmp("RunWaveAutoSave",mode)==0)
                    {
                            global_mode = RUNWAVEAUTOSAVE;
                    }				
                    else if (strcmp("RunWaveContinuous",mode)==0)
                    {
                            global_mode = RUNWAVECONTINUOUS;
                    }
                    else if (strcmp("Start",mode)==0)
                    {
                            global_mode = START;
                    }
                    else if (strcmp("Stop",mode)==0)
                    {
                            global_mode = STOP;
                    }
                    else if (strcmp("Reset Encoders",mode)==0)
                    {
                            global_mode = RESETENCODERS;
                    }
                    else if (strcmp("DATA REQUEST",mode)==0)
                    {
                            global_mode = INIT;
                    }
                    else if (strcmp("Start and Collect",mode)==0)
                    {
                            global_mode = STARTANDCOLLECT;
                    }
                    else if (strcmp("Collect Data",mode)==0)
                    {
                            global_mode = COLLECTDATA;
                    }
                    else
                    {
                            throwException(1);
                            continue;
                    }
            }
            
            switch(global_mode)
            {
                    case INIT:
                            getAllValues();
                            break;
                    case RUNWAVEAUTOSAVE:
                            getAllValues();
                            // Run experiment
                            global_mode = STOP;
                            break;
                    case RUNWAVECONTINUOUS:
                            getAllValues();
                            // Run experiment
                            break;
                    case START:
                            getAllValues();
                            // Run experiment
                            break;
                    case STOP:
                             // Stop process
                            break;
                    case RESETENCODERS:
                            encoderAngle = 0.0f;
                            encoder.set(0);
                            global_mode = STOP;
                            break;
                    case STARTANDCOLLECT:
                            // Start experiment
                            break;
                    case COLLECTDATA:
                            // Start experiment
                            break;
            }
            
	    if (ticks - lastTicks > 1000)
            {
                  ftoa(encoderAngle,encoderAngle2String);
                  pack_json("{s:s,s:s}",JSONOutputString,"NERMLAB","Encoder","Angle",encoderAngle2String);
                  if (encoderAngle != lastEncoderAngle)
                  {
                      putStr(&tx,JSONOutputString);
                      USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
                  }
                  else
                  {
                      continue;
                  }
                  lastTicks = ticks;
                  lastEncoderAngle = encoderAngle;
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
    encoderAngle = encoder.read()*ENCODER_2_ANGLE*RAD2DEG;
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

void sendDataRequest(void)
{
    char * initRequest = "{\"NERMLAB\":\"DATA REQUEST\"}";
    SendCharArrayUSART1(initRequest,strlen(initRequest));
}

void requestDataParams(void)
{
    sendDataRequest();
    getAllValues();   
}

void getAllValues(void)
{
    jsmn_parser parser;
    jsmntok_t tokens[50];
    jsmn_init(&parser);
    int numTokens = jsmn_parse(&parser,input_string,strlen(input_string),tokens,50);
    extract_value(input_string,  "Mode",       mode,       numTokens, tokens);
    extract_value(input_string,  "Kp",         Kp,         numTokens, tokens);
    extract_value(input_string,  "Ki",         Ki,         numTokens, tokens);
    extract_value(input_string,  "Kd",         Kd,         numTokens, tokens);
    extract_value(input_string,  "WaveType",   waveType,   numTokens, tokens);
    extract_value(input_string,  "Magnitude",  magnitude,  numTokens, tokens);
    extract_value(input_string,  "Frequency",  frequency,  numTokens, tokens);
    extract_value(input_string,  "Controller", controller, numTokens, tokens);
    extract_value(input_string,  "Duration",   duration,   numTokens, tokens);
    extract_value(input_string,  "SampleRate", sampleRate, numTokens, tokens);
}

void getMode(void)
{
    jsmn_parser parser;
    jsmntok_t tokens[50];
    jsmn_init(&parser);
    int numTokens = jsmn_parse(&parser,input_string,strlen(input_string),tokens,50);
    extract_value(input_string,  "Mode",       mode,       numTokens, tokens);
}



