//
// blink.c
//
// blink STM32 LED
//
// Neil Gershenfeld 3/11/13
//
// includes
//
#include "stm32f10x.h"
//
// definitions
//
#define STACK_TOP 0x20001000
//
// prototypes
//
int main(void);
void delay(unsigned long delay_count);
//
// declarations
//
GPIO_InitTypeDef GPIO_InitStructure;
//
// vector table
//
unsigned int * myvectors[2] 
   __attribute__ ((section("vectors"))) = {
   (unsigned int *) STACK_TOP, // stack pointer
   (unsigned int *) main // code entry point
   };
//
// functions
//
void delay(unsigned long delay_count) { 
   while(delay_count > 0)
      --delay_count;
   }
//
// main
//
int main(void) {
   //
   // init clocks
   //
   RCC_HSEConfig(RCC_HSE_ON); // enable external HSE clock (8 MHz)
   while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET); // wait until HSE is ready
   RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9); // set PLL source to HSE, multiplier to 9 (72 MHz)
   RCC_PLLCmd(ENABLE); // enable PLL
   while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); // wait until PLL is ready
   FLASH_SetLatency(FLASH_Latency_2); // flash 2 wait state 
   RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // set PLL as system clock
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // enable GPIOC clock
   //
   // init output
   //
   GPIO_StructInit(&GPIO_InitStructure);
   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   //
   // main loop
   //
   while(1) {    
      GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
      delay(1000000);
      GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_SET);
      delay(1000000);
      }
   }
