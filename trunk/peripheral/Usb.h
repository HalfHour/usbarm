/*
 * Usb.h
 *
 *  Created on: 2009-09-29
 *      Author: oex
 */

#ifndef USB_H_
#define USB_H_

#include "Peripheral.h"
#include "GpioPinEventListener.h"
#include "GamepadReport.h"

class MAX3421E;
class GpioPin;
class ControlRequest;
//class GamepadReport;

class Usb: public Peripheral, GpioPinEventListener {
public:
	Usb(MAX3421E *controller, GpioPin *extInterrupt);
	virtual ~Usb();

	void listenForDevice();
	void enumerateDevice();
	void serviceHID();
	uint8_t deviceDetected() { return devDetected; };
	uint8_t deviceEnumerated() { return devEnumerated; };

	// GpioPinEventListener interface implementation
	void stateChanged(GpioPin* pin);

private:
	MAX3421E *controller;
	GpioPin* interruptPin;
	GamepadReport* report;

	// Maximum EP0 packet size
	uint8_t maxPacketSize;

	enum State { Disconnect, Connect, Default, Reset, Normal } state;

	// Global flag
	uint8_t devDetected;
	uint8_t devEnumerated;

	void waitFrames(uint32_t number);
	uint8_t sendRequest(ControlRequest* request);
	uint8_t receiveRawData(uint8_t* rawData, uint16_t length,
			uint8_t endpoint, uint8_t packetSize);
	uint8_t launchTransfer(uint8_t token, uint8_t endpoint);
	void busReset();
	GamepadReport getReport() { return *report; };
};

#endif /* USB_H_ */