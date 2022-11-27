#ifndef BP_DEVICE_H
#define BP_DEVICE_H

/*********************************************************************

File    : bp_device.h
Purpose : header file for blood pressure class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "adv_base.h"
#include "bp_service.h"
#include "connection_pm.h"
#include "peripheral_base.h"
#include <peer_manager_base.h>

// When	bonding	manager	is needed, derive base class as	a template with	BondingManager class as	argument
// Class Storage must provide the following	methods: save_unit(), read_unit(), delete_unit(), goto_begin(), goto_next(), erase_all()
class bp_device_t : public peripheral_base_t
{
public:
	bp_device_t(storage_t& LTK_Mem, storage_t& Attr_Mem, const char* pName);
	void evt_handler(const ble_evt_t* Event);

	connection_pm_t Connection;
	bp_service_t   BloodPressureService;
	adv_base_t      Adv;
};

#endif
