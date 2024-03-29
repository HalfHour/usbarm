/*
 * GpioPin.h
 *
 *  Created on: Oct 2, 2009
 *      Author: fgervais
 */

#ifndef GPIOPIN_H_
#define GPIOPIN_H_

#include "stm32f10x.h"
#include "Vector.h"

class GpioPinEventListener;
class GpioPinConfiguration;

/**
 * General purpose IO pin.
 */
class GpioPin {
public:
	GpioPin(GPIO_TypeDef *gpioRegisters,
			uint8_t pinNumber, uint8_t portNumber);
	virtual ~GpioPin();
	void extInterrupt();
	void configure(GpioPinConfiguration config);
	uint32_t isHigh();
	void setHigh();
	void setLow();
	void addEventListener(GpioPinEventListener *listener);
	uint8_t getPinNumber() { return pinNumber; }
	uint8_t getPortNumber() { return portNumber; }

private:
	uint8_t pinNumber;
	uint32_t portNumber;
	GPIO_TypeDef *gpioRegisters;
	// TODO: This vector thing is buggy
	//Vector<GpioPinEventListener*> listeners;
	GpioPinEventListener* listener;
	uint8_t extiConfigured;

	void configureInterrupt();
};

#endif /* GPIOPIN_H_ */
