// Includes
#include "stm32f4xx.h"
#include "pwm_measurement_timer.h"

volatile uint16_t pwm_measurement_period;
volatile uint16_t pwm_measurement_pulse;
volatile uint32_t pwm_measurement_clkfreq;


// Assuming a 72Mhz timer input clock the prescalar = 1, gives a clock freq
// of 36MHz.  Assuming a sixteen bit counter this gives a minimum input freq
// of 549 Hz
#define PWM_MEASUREMENT_CLOCK_FREQUENCY 18000000
#define PWM_MEASUREMENT_TIMER_PRESCALAR 2

//*****************************************************************************
PwmMeasurementTimer::PwmMeasurementTimer(uint8_t interrupt_priority)
{
    if (interrupt_priority > 15) interrupt_priority = 15;

    pwm_measurement_clkfreq = PWM_MEASUREMENT_CLOCK_FREQUENCY;
    
    // TIM4 clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // GPIOB clock enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    // GPIO Configuration: TIM3 CH2 (PC7)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Connect TIM3 pin using alternate functions
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);

    // Enable the TIM3 global Interrupt 
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = interrupt_priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         // Not used?
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // PWM Input Mode configuration
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);

    // Time base configuration
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_MEASUREMENT_TIMER_PRESCALAR;    // Probably the only thing used in this mode
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // Select the TIM3 Input Trigger: TI2FP2 */
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI2FP2);

    // Select the slave Mode: Reset Mode 
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);

    // TIM enable counter 
    TIM_Cmd(TIM3, ENABLE);

    // Enable the CC2 Interrupt Request
    TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
}

//*****************************************************************************

extern "C" void TIM3_IRQHandler(void)
{
    // Clear TIM4 Capture compare interrupt pending bit
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

    // Get the Input Capture value 
    pwm_measurement_period = TIM_GetCapture2(TIM3);

    pwm_measurement_pulse = TIM_GetCapture1(TIM3);
}
