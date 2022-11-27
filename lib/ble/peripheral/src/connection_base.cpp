/*********************************************************************

File    : connection_base.cpp
Purpose : BLE connection base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "connection_base.h"
#include "param_const.h"        // BLE related constants
#include <ble.h>                // sd_ble_enable()
#include <cstring>              // strlen()
#include "debug_printf.h"

connection_base_t::connection_base_t(const char* pName)
{
	pDeviceName = pName;
	Connected = false;
	if (sd_ble_gap_device_name_set(&OPEN_LINK, (const uint8_t*)pDeviceName, strlen(pDeviceName)) != NRF_SUCCESS)
	{
		while (1)
			;        // connection_base init error
	}
}

uint32_t connection_base_t::set_full_name(const char* DeviceName)
{
	return sd_ble_gap_device_name_set(&OPEN_LINK, (const uint8_t*)DeviceName, strlen(DeviceName));
}

uint32_t connection_base_t::set_ppcp(const ble_gap_conn_params_t* Ppcp)
{
	return sd_ble_gap_ppcp_set(Ppcp);
}

// generic ble event handler, for all the application independent events
// for application specific events handling, see derived class project_ble.cpp
void connection_base_t::evt_handler(const ble_evt_t* Event)
{
	//	Nvm	Storage;
	uint32_t result;
	switch (Event->header.evt_id)
	{
	case BLE_GAP_EVT_CONNECTED:
		Connected = true;
		ConnHandle = Event->evt.gap_evt.conn_handle;
		break;
	case BLE_GAP_EVT_DISCONNECTED:
		Connected = false;
		break;
	case BLE_GAP_EVT_DATA_LENGTH_UPDATE_REQUEST:
		if (sd_ble_gap_data_length_update(Event->evt.gap_evt.conn_handle, NULL, NULL) != NRF_SUCCESS)
		{
			DEBUG_PRT("sd_ble_gap_data_length_update() failed\n");
		}
		break;
	case BLE_GAP_EVT_CONN_PARAM_UPDATE:
		DEBUG_PRT("\t conn	param update: min_conn_interval	= 0x%x\n", Event->evt.gap_evt.params.conn_param_update.conn_params.min_conn_interval);
		DEBUG_PRT("\t max_conn_interval = 0x%x\n", Event->evt.gap_evt.params.conn_param_update.conn_params.max_conn_interval);
		DEBUG_PRT("\t slave_latercy = 0x%x\n", Event->evt.gap_evt.params.conn_param_update.conn_params.slave_latency);
		DEBUG_PRT("\t conn_sup_timeout = 0x%x\n", Event->evt.gap_evt.params.conn_param_update.conn_params.conn_sup_timeout);
		break;
	default:
		break;
	}
}
