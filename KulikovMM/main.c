#include "mcu_support_package/inc/stm32f10x.h"
#include <stm32f10x.h>
#include <stdio.h>

//Делаем функцию-обработчик прерываний
volatile uint32_t count;
void SysTick_Handler(void)
{
  count++;
}

int main(void)
{
  //Настраиваем выводы с 1 по 7
  RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
  for (int i = 4; i <= 28; i+=4)
  {
    GPIOA->CRL |= 1<<i|1<<(i+1);
    GPIOA->CRL &= ~(1<<(i+2)) &~(1<<(i+3));
  }
  //Настраиваем выводы 8 и 9
  GPIOA->CRH &= ~0xFF;
  GPIOA->CRH |= 0x33;
  //Гасим светодиоды, если какие-то вдруг горят
  GPIOA->ODR &= ~0x3FE;
  
  const uint32_t msec[9] = {1, 10, 20, 50, 150, 200, 300, 400, 500};
  uint32_t j[9] = {0};
  
  //Настраиваем прерывание. Частоты мигания получатся от 100 до 0.2 Гц
  __disable_irq();
  SysTick_Config(SystemCoreClock/100);
  __enable_irq();
  
  while(1)
  {
    for (int k = 1; k < 10; k++)
    {
      if ((count - j[k]) == msec[k-1])
      {
        GPIOA->ODR ^= (1<<k);
        j[k]=count;
      }
    }
  }
  return 0;
}
