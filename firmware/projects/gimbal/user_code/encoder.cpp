// Includes
#include "encoder.h"

//*****************************************************************************
Encoder::Encoder(encoder_id_t id)
{
    encoder_id_ = id;
    prev_counter_ = 0;
    overflows_ = 0;

    GPIO_InitTypeDef GPIO_InitStructure;

    switch (encoder_id_)
    {
        case EncoderA:

            // GPIO clock enable
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

            // TIM2 clock enable
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // Alternate function mode
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   // Speed doesn't matter (input)
            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;       // Open Drain - but shouldn't matter (input)
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
            GPIO_Init(GPIOA, &GPIO_InitStructure);
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
            GPIO_Init(GPIOB, &GPIO_InitStructure);

            TIM_SetAutoreload(TIM2, 0xffff);
            TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

            GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);   // Assign PA15 and PB3
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);   // to TIM2

            TIM_Cmd(TIM2, ENABLE);

            TIM2->CCMR1 |= 0xF0F0;  // Implements digital filter

            break;

        case EncoderB:
            // GPIOD clock enable
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

            // TIM4 clock enable
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // Alternate function mode
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   // Speed doesn't matter (input)
            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;       // Open Drain - but shouldn't matter (input)
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            GPIO_Init(GPIOD, &GPIO_InitStructure);

            TIM_SetAutoreload(TIM4, 0xffff);
            TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

            GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);   // Assign PD12 and PD13
            GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);   // to TIM4

            TIM_Cmd(TIM4, ENABLE);

            TIM4->CCMR1 |= 0xF0F0;   // Implements digital filter

            break;
    }
}

//*****************************************************************************
int32_t Encoder::read(void)
{
    uint16_t counter; // 16 bit counter of timer
    int32_t count32;  // 32 bit counter which accounts for timer overflows

    switch (encoder_id_)
    {
        case EncoderA:
            counter = TIM2->CNT;
            break;
        case EncoderB:
            counter = TIM4->CNT;
            break;
    }

    // The following assumes this function is called frequently enough that
    // the encoder cannot change more 0x8000 counts between calls, and that
    // the counter overflows from 0xffff to 0 and underflows from 0 to 0xffff
    // Should work even for a 16-bit or 32-bit counter.
    
    if ((prev_counter_ > 0xc000) && (counter < 0x4000))
    {
        overflows_ += 1; // overflow
    }

    else if ((prev_counter_ < 0x4000) && (counter > 0xc000))
    {
        overflows_ -= 1; // underflow
    }

    count32 = overflows_*0x10000 + counter;
    prev_counter_ = counter;

    return count32;
}

//*****************************************************************************
void Encoder::set(int32_t count32)
{
    if (count32 < 0)
    {
        overflows_ = count32 / ((int32_t)0x10000) - 1;
    }
    else
    {
        overflows_ = count32 / ((int32_t)0x10000);
    }

    uint16_t counter = (uint16_t)(count32 - overflows_*0x10000);
    prev_counter_ = counter;

    switch (encoder_id_)
    {
        case EncoderA:
            TIM2->CNT = counter;
            break;
        case EncoderB:
            TIM4->CNT = counter;
            break;
    }
}
