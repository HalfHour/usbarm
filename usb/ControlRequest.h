/*
 * ControlPacket.h
 *
 *  Created on: Oct 27, 2009
 *      Author: fgervais
 */

#ifndef CONTROLREQUEST_H_
#define CONTROLREQUEST_H_

#include <stdint.h>

class ControlRequest {
public:
	uint8_t requestType;
	uint8_t request;
	uint16_t value;
	uint16_t index;
	uint16_t length;

	uint8_t* toArray();

	ControlRequest();
	virtual ~ControlRequest();

private:
	uint8_t rawPacket[8];
};

#endif /* CONTROLREQUEST_H_ */