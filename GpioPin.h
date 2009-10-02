/*
 * GpioPin.h
 *
 *  Created on: Oct 2, 2009
 *      Author: fgervais
 */

#ifndef GPIOPIN_H_
#define GPIOPIN_H_

#include "stm32f10x.h"

class Gpio;

class GpioPin {
public:
	GpioPin(Gpio *port, uint32_t pinFlag);
	virtual ~GpioPin();

	void High();
	void Low();
	bool isHigh();
	bool isLow();

private:
	uint32_t pinFlag;
	Gpio *port;
};

#endif /* GPIOPIN_H_ */