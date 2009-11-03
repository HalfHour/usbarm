/*
 * ControlPacket.cpp
 *
 *  Created on: Oct 27, 2009
 *      Author: fgervais
 */

#include "ControlPacket.h"

#include <stdint.h>

ControlPacket::ControlPacket() {
}

ControlPacket::~ControlPacket() {

}

uint8_t* ControlPacket::toArray() {
	rawPacket[0] = requestType;
	rawPacket[1] = request;
	rawPacket[2] = (uint8_t)(value & 0x00FF);
	rawPacket[3] = (uint8_t)((value >> 8) & 0x00FF);
	rawPacket[4] = (uint8_t)(index & 0x00FF);
	rawPacket[5] = (uint8_t)((index >> 8) & 0x00FF);
	rawPacket[6] = (uint8_t)(length & 0x00FF);
	rawPacket[7] = (uint8_t)((length >> 8) & 0x00FF);

	return rawPacket;
}