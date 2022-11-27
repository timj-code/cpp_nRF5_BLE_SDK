#ifndef BLINKY_SERVICE_H
#define BLINKY_SERVICE_H

/*********************************************************************

File    : blinky_service.h
Purpose : header file for blinky service class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "blinky_charact.h"
#include "service_base.h"

// UUID 5ca6d966-bdbc-429d-80d0-fabb34ebbc9d
const ble_uuid128_t UUID_BLINKY_SERVICE = {
	0x9d, 0xbc, 0xeb, 0x34, 0xbb, 0xfa, 0xd0, 0x80, 0x9d, 0x42, 0xbc, 0xbd, 0x66, 0xd9, 0xa6, 0x5c
};

class blinky_service_t : public service_base_t
{
public:
	blinky_charact_t CharactBlinky;

	blinky_service_t()
		: service_base_t(&UUID_BLINKY_SERVICE)
	{
		CharactBlinky.add_to_service(Handle);
	}
};

#endif
