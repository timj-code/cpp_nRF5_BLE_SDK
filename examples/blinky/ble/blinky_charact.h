#ifndef BLINKY_CHARACT_H
#define BLINKY_CHARACT_H

/*********************************************************************

File    : blinky_charact.h
Purpose : header file for blinky characteristic class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "charact_base.h"

// UUID cee8483a-65f1-413c-88f0-1250181b4fe9
const ble_uuid128_t UUID_BLINKY_CHARACT = {
	0xe9, 0x4f, 0x1b, 0x18, 0x50, 0x12, 0xf0, 0x88, 0x3c, 0x41, 0xf1, 0x65, 0x3a, 0x48, 0xe8, 0xce
};

struct blinky_t
{
	uint8_t OutputBit : 1;
	uint8_t Reserved : 7;
};

class blinky_charact_t : public charact_base_t
{
public:
	blinky_charact_t()
		: charact_base_t(&UUID_BLINKY_CHARACT)
	{
		// character value attributes
		ValueAttribute.init_len = sizeof(blinky_t);
		ValueAttribute.init_offs = 0;
		ValueAttribute.max_len = sizeof(blinky_t);
		ValueAttribute.p_value = (uint8_t*)&Blinky;        // by SDK default, characteristic values are stored in user memory(as class member)
	}
	volatile blinky_t Blinky;        // set volatile as it can be changed by BLE stack
};

#endif
