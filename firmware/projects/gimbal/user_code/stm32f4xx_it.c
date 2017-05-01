#include "stm32f4xx.h"

extern void sys_tick_callback();


/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

void NMI_Handler(void)
{
    while(1);
}
// *************************************************************
void HardFault_Handler(void)
{
    while (1);
}
// *************************************************************
void MemManage_Handler(void)
{
    while (1);
}
// *************************************************************
void BusFault_Handler(void)
{
    while (1);
}
// *************************************************************
void UsageFault_Handler(void)
{
    while (1);
}
// *************************************************************
void SVC_Handler(void)
{
    while (1);
}
// *************************************************************
void DebugMon_Handler(void)
{
    while (1);
}
// *************************************************************
void PendSV_Handler(void)
{
    while (1);
}
// *************************************************************
void SysTick_Handler(void)
{
   sys_tick_callback();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/
