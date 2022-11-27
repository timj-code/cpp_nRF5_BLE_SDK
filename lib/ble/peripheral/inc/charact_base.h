#ifndef CHARACT_BASE_H
#define CHARACT_BASE_H

/*********************************************************************

File    : charact_base.h
Purpose : header file for characteristic base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "param_const.h"        // BLE related constants
#include <ble.h>                // ble	related	data types and sd_ble_xxxx calls
#include <cstdio>               // definition of NULL

/************************************************/

class charact_base_t
{
public:
	ble_gatts_char_handles_t Handles;

	charact_base_t(const ble_uuid128_t* pUuid128,
		const ble_gatts_char_md_t*      pCharactMeta = &RW_CHARACT_META_DATA,        // set default characteristic property as R/W
		const ble_gatts_attr_md_t*      pValueAttrMeta = &DEFAULT_ATTR_META_DATA);        // set default characteristic VALUE property
	charact_base_t(const uint16_t  Uuid16,
		const ble_gatts_char_md_t* pCharactMeta = &RW_CHARACT_META_DATA,
		const ble_gatts_attr_md_t* pValueAttrMeta = &DEFAULT_ATTR_META_DATA);

	uint32_t add_to_service(const uint16_t ServiceHandle);
	uint32_t read(const uint16_t ConnHandle, ble_gatts_value_t* pValue);         // when user memory is used for attribute Value, read and write function are not much necessary, as user can access it directly
	uint32_t write(const uint16_t ConnHandle, ble_gatts_value_t* pValue);        // when user memory is used for attribute Value, read and write function are not much necessary, as user can access it directly
	void     set_notify_mode(const uint8_t Mode);                                // Mode must be BLE_GATT_HVX_NOTIFICATION or BLE_GATT_HVX_INDICATION
	uint32_t notify(const uint16_t ConnHandle);

protected:
	ble_gatts_hvx_params_t Hvx;
	ble_gatts_attr_t       ValueAttribute;

private:
	ble_gatts_char_md_t CharactMetaData;
	ble_uuid_t          Uuid;
};

#endif
