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
#include <stdio.h>
float duty[3];
float Vq = 1.0f;
float speed = 10.0f;
float motor_angle;
float encoder_angle;
float command = PI/2;
bool enable_out = true;
float calibrate_angle;
float mech_offset = -0.284275f;
int8_t mech_direction = 1;

double time = 0.0f;
    
int main(void)
{
	/* Initalizations */
	InitUSART1();
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
                    else if (strcmp("Calibrate",mode)==0)
                    {
                            global_mode = CALIBRATE;
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
                    case CALIBRATE:
                            MotorCommutationCalibration commutation_cal(NUM_POLE_PAIRS);
                            encoder.set(motor_angle/ENCODER_2_ANGLE);
                            while (1)
                            {
                                if(commutation_cal.runStateMachine(time,motor_angle,&calibrate_angle))
                                {
                                    mech_offset    = commutation_cal.getOffset();
                                    mech_direction = commutation_cal.getDirection();
                                    pack_json("{s:s}",JSONOutputString,"NERMLAB","Calibration Complete");
                                    putStr(&tx,JSONOutputString);
                                    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
                                    global_mode = STOP;
                                    break;
                                }
                                if (enable_out)
                                {
                                    enableA.set();
                                    enableB.set();
                                    enableC.set();
                                }
                                else
                                {
                                    enableA.clear();
                                    enableB.clear();
                                    enableC.clear();
                                }
                            }
                            global_mode = STOP;
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
    float sth, cth;
    /* Tick Counter */
    ticks++;
    time += 0.0001f;
    
    /* Encoder Angle */
    encoderAngle = encoder.read()*ENCODER_2_ANGLE*RAD2DEG;
    
    /* Motor Angle */
    motor_angle = get_angle_from_pulse();
    motor_angle = mech_direction*motor_angle + mech_offset;
    motor_angle = wrap_angle_0_to_2PI(motor_angle);
    
    float elec_angle = encoderAngle*NUM_POLE_PAIRS;
    elec_angle = wrap_angle_0_to_2PI(elec_angle);
    
    if (global_mode == CALIBRATE)
    {
        sincos_table(calibrate_angle, &sth, &cth);
        Vq = CALIBRATE_VOLTAGE;
    }
    else
    {
        sincos_table(elec_angle, &sth, &cth);
    }
    
    Vq = limit(Vq, -MAX_VOLTAGE, MAX_VOLTAGE);
    
    
    //TODO: decide whether we need Vd and do remove complex limit calc if not
    dq_voltage_to_pwm(Vq, 0.0f, VBUS, cth, sth, duty);
    
    motor_pwm.setDuty( duty[0], 1 );
    motor_pwm.setDuty( duty[1], 2 );
    motor_pwm.setDuty( duty[2], 3 );
    
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

void dq_voltage_to_pwm(float Vq,
                       float Vd,
                       float Vbus,
                       float cos_th,
                       float sin_th,
                       float duty[3])
{
    // normalize voltages to half of Vbus
    float uq = Vq/Vbus/2.0f;
    float ud = Vd/Vbus/2.0f;
    
    // limit magnitude, max_mag = sqrt(3/2)(maxduty-0.5)
    float max_mag = 1.224745f*(MAX_DUTY - 0.5f);
    float mag = sqrtf(uq*uq + ud*ud);
    if ( mag > max_mag)
    {
        float scale = max_mag/mag;
        uq *= scale;
        ud *= scale;
    }

    // alpha-beta duty thru iPark Transform
    float ua = cos_th*ud - sin_th*uq;
    float ub = sin_th*ud + cos_th*uq;
    
    // abc DutyCycles thu iClark Transform
    duty[0] = 0.8164966f*ua + 0.5f;
    duty[1] = -0.4082483f*ua + 0.7071068f*ub + 0.5f;
    duty[2] = -0.4082483f*ua - 0.7071068f*ub + 0.5f;
}

float get_angle_from_pulse(void)
{
    uint32_t pulse_counts, period_counts;

    // the pulse and period from pwm measurement are volatile
    __disable_irq();
    pulse_counts = pwm_measurement_pulse;
    period_counts = pwm_measurement_period;
    __enable_irq();
    
    // return garbabe if invalid
    if ( period_counts == 0 )
        return (100.0f);
    
    // scale to ideal period of AS5048/AS5047
    pulse_counts = (float)pulse_counts*4119/period_counts;
    
    // error indication band is 12 to 16
    if (pulse_counts <= 14)
        return (100.0f);
    
    if (pulse_counts < 16) pulse_counts = 16;   // allow it to be 15 for zero
    return ( (pulse_counts-16)*2.0f*PI/4095 );
}



