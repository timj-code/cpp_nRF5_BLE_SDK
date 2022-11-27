/*********************************************************************

File    : bp_device.cpp
Purpose : blood pressure device class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "bp_device.h"

bp_device_t::bp_device_t(storage_t& LTK_Mem, storage_t& Attr_Mem, const char* pName)
	: Adv(pName)
	, Connection(LTK_Mem, Attr_Mem, pName)
{
}

void bp_device_t::evt_handler(const ble_evt_t* Event)
{
	uint32_t           result;
	ble_gap_enc_key_t* pSecKey;
	Connection.evt_handler(Event);        // generic event handler in base class
	switch (Event->header.evt_id)         // project specific event handler
	{
	case BLE_GAP_EVT_DISCONNECTED:
		Adv.start();
		break;
	default:
		break;
	}
}
