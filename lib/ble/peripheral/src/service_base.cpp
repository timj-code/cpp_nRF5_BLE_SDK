/*********************************************************************

File    : service_base.cpp
Purpose : BLE service base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "service_base.h"

service_base_t::service_base_t(const uint16_t Uuid16)
{
	Uuid.type = BLE_UUID_TYPE_BLE;
	Uuid.uuid = Uuid16;

	sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &Uuid, &Handle);
}

service_base_t::service_base_t(const ble_uuid128_t* Uuid128)
{
	sd_ble_uuid_vs_add(Uuid128, &Uuid.type);
	Uuid.uuid = *((uint16_t*)(Uuid128->uuid128 + 12));

	sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &Uuid, &Handle);
}
