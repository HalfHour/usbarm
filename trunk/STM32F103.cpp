/*
 * STM32F103.cpp
 *
 *  Created on: Oct 1, 2009
 *      Author: fgervais
 */

#include "STM32F103.h"

#include "Uart.h"
#include "Gpio.h"

/* Static init. Required to make the compiler happy */
Uart* STM32F103::uart1 = 0;

STM32F103::STM32F103() {
	// TODO Auto-generated constructor stub

}

STM32F103::~STM32F103() {
	// TODO Auto-generated destructor stub
}

/**
 * @brief	This function return an instance to the actual Uart1.
 * @return	The STM32F103 Uart1
 */
Uart* STM32F103::getUart1() {
	if(uart1 == 0) {
		uart1 = new Uart();
		// Uart should somehow be initialized here
	}
	return uart1;
}

void STM32F103::ioctl(Gpio* port) {
	uint32_t direction = port->getDirection();
}
