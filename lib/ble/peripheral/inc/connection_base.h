#ifndef CONNECTION_BASE_H
#define CONNECTION_BASE_H

/*********************************************************************

File    : connection_base.h
Purpose : header file for connection base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <ble.h>
#include <cstdio>        // printf() and NULL

class connection_base_t
{
public:
	bool     Connected;
	uint16_t ConnHandle;
	connection_base_t(const char* pName);
	uint32_t set_full_name(const char* DeviceName);
	uint32_t set_ppcp(const ble_gap_conn_params_t* Ppcp);
	void     evt_handler(const ble_evt_t* Event);
	const char* pDeviceName;
};

#endif
