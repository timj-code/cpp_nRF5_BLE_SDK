#ifndef	SERVICE_BASE_H
#define	SERVICE_BASE_H

/*********************************************************************

File    : pservice_base.h
Purpose : header file for BLE service base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <ble.h>	// ble related data types and sd_ble_xxxx calls

class service_base_t
{
public:
    service_base_t(const uint16_t	Uuid16);
    service_base_t(const ble_uuid128_t* Uuid128);

protected:
    uint16_t Handle;

private:
    ble_uuid_t Uuid;
};

#endif