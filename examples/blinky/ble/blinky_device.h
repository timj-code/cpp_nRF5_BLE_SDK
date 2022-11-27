#ifndef BLINKY_DEVICE_H
#define BLINKY_DEVICE_H

/*********************************************************************

File    : blinky_device.h
Purpose : header file for blinky class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "adv_base.h"
#include "blinky_service.h"
#include "connection_base.h"
#include "peripheral_base.h"

// When	bonding	manager	is needed, derive base class as	a template with	BondingManager class as	argument
// Class Storage must provide the following	methods: save_unit(), read_unit(), delete_unit(), goto_begin(), goto_next(), erase_all()
class blinky_device_t : public peripheral_base_t
{
public:
	blinky_device_t(const char* pName);
	void evt_handler(const ble_evt_t* Event);

	connection_base_t Connection;
	blinky_service_t  BlinkyService;
	adv_base_t        Adv;
};

#endif
