/*
 * STM32F103.cpp
 *
 *  Created on: Oct 1, 2009
 *      Author: fgervais
 */

#include "STM32F103.h"

#include "Uart.h"
#include "Gpio.h"
#include "Spi.h"
#include "GpioPin.h"
#include "GpioPinConfiguration.h"
#include "Usb.h"
#include "MAX3421E.h"
#include "UartConfiguration.h"
#include "Timer.h"

#include "stm32f10x.h"
#include "core_cm3.h"

/* Static init. Required to make the compiler happy */
Uart* STM32F103::uart1 = 0;
Uart* STM32F103::uart2 = 0;
Gpio* STM32F103::gpioA = 0;
Gpio* STM32F103::gpioB = 0;
Gpio* STM32F103::gpioC = 0;
Gpio* STM32F103::gpioD = 0;
Spi* STM32F103::spi1 = 0;
Usb* STM32F103::usb = 0;
Timer* STM32F103::timer2 = 0;

STM32F103::STM32F103() {

}

STM32F103::~STM32F103() {

}

/**
 * @brief	This function return an instance to the actual Uart1.
 * @return	The STM32F103 Uart1
 */
Uart* STM32F103::getUart1() {
	systemClocksFreq systemClock;

	getRccClockFreq(&systemClock);

	if(uart1 == 0) {

		uart1 = new Uart(USART1, 1, systemClock.PCLK2_Frequency);
		// Send clock to USART 1
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;


		// Configure Uart1 IOs
		GpioPinConfiguration uartTxPinConfig, uartRxPinConfig;
		Gpio *portA = getGpioA();
		// NSS
		uartTxPinConfig.pin = Gpio::AF_PUSH_PULL_OUTPUT | Gpio::OUTPUT_SPEED_50MHZ;
		portA->getPin(9)->configure(uartTxPinConfig);

		uartRxPinConfig.pin = Gpio::FLOATING_INPUT;
		portA->getPin(10)->configure(uartRxPinConfig);

		//enable UART1 Interrupt
		//NVIC->ISER[1] = 0x20;

		//set priority to UART1 Interrupt
		//NVIC->IP[USART1_IRQn] = 0;
	}

	return uart1;
}

/**
 * @brief	This function return an instance to the actual Uart1.
 * @return	The STM32F103 Uart1
 */
Uart* STM32F103::getUart2() {
	systemClocksFreq systemClock;


	getRccClockFreq(&systemClock);

	if(uart2 == 0) {
		uart2 = new Uart(USART2, 2, systemClock.PCLK1_Frequency);
		// Send clock to USART 2
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN;
	}
	return uart2;
}

Gpio* STM32F103::getGpioA() {
	if(gpioA == 0) {
		gpioA = new Gpio(GPIOA, 1);
		// Send clock to GPIO A
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	}
	return gpioA;
}

Gpio* STM32F103::getGpioB() {
	if(gpioB == 0) {
		gpioB = new Gpio(GPIOB, 2);
		// Send clock to GPIO B
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	}
	return gpioB;
}

Gpio* STM32F103::getGpioC() {
	if(gpioC == 0) {
		gpioC = new Gpio(GPIOC, 3);
		// Send clock to GPIO C
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	}
	return gpioC;
}

Gpio* STM32F103::getGpioD() {
	if(gpioD == 0) {
		gpioD = new Gpio(GPIOD, 4);
		// Send clock to GPIO D
		RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	}
	return gpioD;
}

Spi* STM32F103::getSpi1() {
	if(spi1 == 0) {
		// Configure SPI IOs
		GpioPinConfiguration spiPinConfig;
		Gpio *portA = getGpioA();
		// NSS
		spiPinConfig.pin = Gpio::GP_PUSH_PULL_OUTPUT | Gpio::OUTPUT_SPEED_50MHZ;
		portA->getPin(4)->configure(spiPinConfig);
		// SCLK
		spiPinConfig.pin = Gpio::AF_PUSH_PULL_OUTPUT | Gpio::OUTPUT_SPEED_50MHZ;
		portA->getPin(5)->configure(spiPinConfig);
		// MISO
		spiPinConfig.pin = Gpio::AF_PUSH_PULL_OUTPUT | Gpio::OUTPUT_SPEED_50MHZ;
		portA->getPin(6)->configure(spiPinConfig);
		// MOSI
		spiPinConfig.pin = Gpio::AF_PUSH_PULL_OUTPUT | Gpio::OUTPUT_SPEED_50MHZ;
		portA->getPin(7)->configure(spiPinConfig);

		// Create a new Spi with these parameters (ID, Registers, SlaveSelect pin)
		spi1 = new Spi(SPI1, 1, portA->getPin(4));
		// Send clock to SPI1
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_AFIOEN;
	}
	return spi1;
}

Usb* STM32F103::getUsb() {
	if(usb == 0) {
		MAX3421E* controller = new MAX3421E(getSpi1());
		GpioPin* interruptPin = getGpioA()->getPin(1);
		Timer* timer = getTimer2();
		// Create the new USB port with GpioA1 as external interrupt pin
		// and Timer2 to schedule HID interrupt service.
		usb = new Usb(controller, interruptPin, timer);

	}
	return usb;
}

Timer* STM32F103::getTimer2() {
	if(timer2 == 0) {
		timer2 = new Timer(TIM2, 2);
		// Send clock to Timer2
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	return timer2;
}

/**
  * @brief  Returns the frequencies of different on chip clocks.
  * @param  RCC_Clocks: pointer to a RCC_ClocksTypeDef structure which will hold
  *   the clocks frequencies.
  * @retval None
  */
void STM32F103::getRccClockFreq(systemClocksFreq* systemClock) {
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & CFGR_SWS_Mask;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
    	systemClock->SYSCLK_Frequency = HSI_Value;
      break;
    case 0x04:  /* HSE used as system clock */
    	systemClock->SYSCLK_Frequency = HSE_Value;
      break;
    case 0x08:  /* PLL used as system clock */

      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
      pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;

      pllmull = ( pllmull >> 18) + 2;

      if (pllsource == 0x00)
      {/* HSI oscillator clock divided by 2 selected as PLL clock entry */
    	  systemClock->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
      }
      else
      {/* HSE selected as PLL clock entry */
        if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (uint32_t)RESET)
        {/* HSE oscillator clock divided by 2 */
        	systemClock->SYSCLK_Frequency = (HSE_Value >> 1) * pllmull;
        }
        else
        {
        	systemClock->SYSCLK_Frequency = HSE_Value * pllmull;
        }
      }
      break;

    default:
    	systemClock->SYSCLK_Frequency = HSI_Value;
      break;
  }

  /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
  /* Get HCLK prescaler */
  tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
  tmp = tmp >> 4;
  presc = APBAHBPrescTable[tmp];
  /* HCLK clock frequency */
  systemClock->HCLK_Frequency = systemClock->SYSCLK_Frequency >> presc;
  /* Get PCLK1 prescaler */
  tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
  tmp = tmp >> 8;
  presc = APBAHBPrescTable[tmp];
  /* PCLK1 clock frequency */
  systemClock->PCLK1_Frequency = systemClock->HCLK_Frequency >> presc;
  /* Get PCLK2 prescaler */
  tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
  tmp = tmp >> 11;
  presc = APBAHBPrescTable[tmp];
  /* PCLK2 clock frequency */
  systemClock->PCLK2_Frequency = systemClock->HCLK_Frequency >> presc;
  /* Get ADCCLK prescaler */
  tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
  tmp = tmp >> 14;
  presc = ADCPrescTable[tmp];
  /* ADCCLK clock frequency */
  systemClock->ADCCLK_Frequency = systemClock->PCLK2_Frequency / presc;
}
