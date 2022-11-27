/*********************************************************************

File    : blinky_device.cpp
Purpose : blinky device class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "blinky_device.h"
#include "debug_printf.h"

blinky_device_t::blinky_device_t(const char* pName)
	: Connection(pName)
	, Adv(pName)
{
}

void blinky_device_t::evt_handler(const ble_evt_t* Event)
{
	uint32_t result;
	Connection.evt_handler(Event);        // generic event handler in base class
	switch (Event->header.evt_id)         // project specific event handler
	{
	case BLE_GAP_EVT_DISCONNECTED:
	case BLE_GAP_EVT_ADV_SET_TERMINATED:
		Adv.start();
		DEBUG_PRT("Advertising restarted\n");
		break;
	default:
		break;
	}
}
