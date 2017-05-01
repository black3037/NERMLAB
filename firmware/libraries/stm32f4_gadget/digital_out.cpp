// Includes
#include "digital_out.h"

//*****************************************************************************
DigitalOut::DigitalOut(digital_out_pin_t pin,
                       GPIOOType_TypeDef out_type,
                       GPIOPuPd_TypeDef pull_up_pull_down,
                       uint8_t initial_state)
{
    switch (pin)
    {
        case PC10:
            gpio_pin_ = GPIO_Pin_10;
            rcc_ahb1periph_ = RCC_AHB1Periph_GPIOC;
            port_ = GPIOC;
            break;
        case PC11:
            gpio_pin_ = GPIO_Pin_11;
            rcc_ahb1periph_ = RCC_AHB1Periph_GPIOC;
            port_ = GPIOC;
            break;
        case PC12:
            gpio_pin_ = GPIO_Pin_12;
            rcc_ahb1periph_ = RCC_AHB1Periph_GPIOC;
            port_ = GPIOC;
            break;
    }

    // GPIOx Periph clock enable
    RCC_AHB1PeriphClockCmd(rcc_ahb1periph_, ENABLE);

    // Configure the GPIO pin
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = gpio_pin_;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = out_type;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = pull_up_pull_down;

    GPIO_Init(port_, &GPIO_InitStructure);

    if (initial_state)
    {
        set();
    }
    else
    {
        clear();
    }
}

