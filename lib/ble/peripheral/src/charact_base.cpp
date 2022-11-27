/*********************************************************************

File    : charact_base.cpp
Purpose : characteristic base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "charact_base.h"

charact_base_t::charact_base_t(const ble_uuid128_t* pUuid128,
	const ble_gatts_char_md_t*                      pCharactMeta,
	const ble_gatts_attr_md_t*                      pValueAttrMeta)
{
	sd_ble_uuid_vs_add(pUuid128, &Uuid.type);
	Uuid.uuid = *((uint16_t*)(pUuid128->uuid128 + 12));

	ValueAttribute.p_uuid = &Uuid;
	ValueAttribute.p_attr_md = pValueAttrMeta;

	CharactMetaData = *pCharactMeta;
}

charact_base_t::charact_base_t(const uint16_t Uuid16,
	const ble_gatts_char_md_t*                pCharactMeta,
	const ble_gatts_attr_md_t*                pValueAttrMeta)
{
	Uuid.uuid = Uuid16;
	Uuid.type = BLE_UUID_TYPE_BLE;

	ValueAttribute.p_uuid = &Uuid;
	ValueAttribute.p_attr_md = pValueAttrMeta;
	// for vloc=user :
	//	ValueAttr.init_len = ValueLen;
	//	ValueAttr.init_offs = 0;
	//	ValueAttr.max_len = ValueLen;
	//	ValueAttr.p_value = new	uint8_t[ValueLen];

	CharactMetaData = *pCharactMeta;
}

uint32_t charact_base_t::add_to_service(const uint16_t ServiceHandle)
{
	uint32_t result;
	result = sd_ble_gatts_characteristic_add(ServiceHandle, &CharactMetaData, &ValueAttribute, &Handles);
	return result;
}

uint32_t charact_base_t::read(const uint16_t ConnHandle, ble_gatts_value_t* pValue)
{
	return sd_ble_gatts_value_get(ConnHandle, Handles.value_handle, pValue);
}

uint32_t charact_base_t::write(const uint16_t ConnHandle, ble_gatts_value_t* pValue)
{
	return sd_ble_gatts_value_set(ConnHandle, Handles.value_handle, pValue);
}

void charact_base_t::set_notify_mode(const uint8_t Mode)
{
	Hvx.handle = Handles.value_handle;
	Hvx.offset = 0;
	Hvx.type = Mode;        // Mode must be BLE_GATT_HVX_NOTIFICATION or BLE_GATT_HVX_INDICATION
}

uint32_t charact_base_t::notify(const uint16_t ConnHandle)
{
	Hvx.p_data = NULL;        // NULL to use current attribute value
	Hvx.p_len = NULL;
	return sd_ble_gatts_hvx(ConnHandle, &Hvx);
}
