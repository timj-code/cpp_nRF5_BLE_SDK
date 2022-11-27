/*********************************************************************

File    : connection_pm_base.cpp
Purpose : BLE connection with peer manager base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "debug_printf.h"
#include <connection_pm.h>
#include <param_const.h>

connection_pm_t::connection_pm_t(storage_t& LTK_Mem, storage_t& Attr_Mem, const char* pName)
	: connection_base_t(pName)
	, Pm(LTK_Mem, Attr_Mem)
{
	pBondingConfig = &BOND_JUSTWORK;
	BondedConnection = false;
}

connection_pm_t::connection_pm_t(storage_t& LTK_Mem, storage_t& Attr_Mem, const char* pName, const ble_gap_sec_params_t* pBondingCfg)
	: connection_base_t(pName)
	, Pm(LTK_Mem, Attr_Mem)
{
	pBondingConfig = pBondingCfg;
	BondedConnection = false;
}

void connection_pm_t::evt_handler(const ble_evt_t* Event)
{
	uint32_t result;
	connection_base_t::evt_handler(Event);
	switch (Event->header.evt_id)
	{
	case BLE_GAP_EVT_DISCONNECTED:        // disconnected and store bonding info and system attr.
		if (BondedConnection)
		{
			SysAttrLen = 128;
			result = sd_ble_gatts_sys_attr_get(Event->evt.gap_evt.conn_handle, SysAttrData, &SysAttrLen, BLE_GATTS_SYS_ATTR_FLAG_USR_SRVCS);
			if (result == NRF_SUCCESS)
			{
				DEBUG_PRT("sys attr length=%d\n", SysAttrLen);
				result = Pm.save_SysAttr(encrypt_key_local.master_id, SysAttrData, SysAttrLen);
				if (result == NRF_SUCCESS)
				{
					DEBUG_PRT("System Attribute information saved\n");
				}
				else
				{
					DEBUG_PRT("Writing	Flash Error, error 0x%X\n", result);
				}
			}
			else
			{
				DEBUG_PRT("get sys attr fail, error = 0x%x\n", result);
				DEBUG_PRT("SysAttr Lenght = 0x%x\n", SysAttrLen);
			}
		}
		BondedConnection = false;
		break;
	case BLE_GAP_EVT_SEC_PARAMS_REQUEST:        // event happens when received pairing/bonding request
		result = sd_ble_gap_sec_params_reply(ConnHandle, BLE_GAP_SEC_STATUS_SUCCESS, pBondingConfig, &Keyset);
		if (result != NRF_SUCCESS)
		{
			DEBUG_PRT("sd_ble_gap_sec_params_replay failed, result	= 0x%x\n", result);
		}
		break;
	case BLE_GAP_EVT_CONN_SEC_UPDATE:        // will receive this event at legacy pairing phase 2
		BondedConnection = true;
		break;
	case BLE_GAP_EVT_AUTH_STATUS:        // receive this event when device finished bonding process
		DEBUG_PRT("Bonding finished\n");
		if (Pm.delete_LTK(encrypt_key_peer.master_id) == NRF_SUCCESS)        // delete previous LTK for this peer if exist
		{
			DEBUG_PRT("Previous LTK data exist, deleted\n");
		}
		if (Pm.save_LTK(encrypt_key_local) == NRF_SUCCESS)
		{
			DEBUG_PRT("LTK data saved.\n");
		}
		break;
	case BLE_ERROR_GATTS_SYS_ATTR_MISSING:        // receive this when bonded devices re-establish connection
		DEBUG_PRT("System Attribute data missing, reload saved data.\n");
		if (Pm.find_SysAttr(encrypt_key_local.master_id, SysAttrData, &SysAttrLen) == NRF_SUCCESS)
		{
			DEBUG_PRT("SysAttr reloaded.\n");
		}
		if (sd_ble_gatts_sys_attr_set(Event->evt.gap_evt.conn_handle, SysAttrData, SysAttrLen, BLE_GATTS_SYS_ATTR_FLAG_USR_SRVCS) != NRF_SUCCESS)
		{
			DEBUG_PRT("sd_ble_gatts_sys_attr_set failed\n");
		}
		break;
	case BLE_GAP_EVT_SEC_INFO_REQUEST:        // this event happens when bonded devices are re-connecting
		DEBUG_PRT("Bonded device reconnecting...\n");
		if (Pm.find_LTK(Event->evt.gap_evt.params.sec_info_request.master_id, &encrypt_key_local) == NRF_SUCCESS)
		{
			// peer LTK data found
			DEBUG_PRT("LTK data reloaded.\n");
			if (sd_ble_gap_sec_info_reply(ConnHandle, &encrypt_key_local.enc_info, &identity_key_local.id_info, NULL) != NRF_SUCCESS)
			{
				DEBUG_PRT("sec info reply error\n");
			}
			result = Pm.delete_SysAttr(encrypt_key_local.master_id);
			if (result == NRF_ERROR_NOT_FOUND)
			{
				DEBUG_PRT("Existing Attr not found.\n");
			}
			else
			{
				DEBUG_PRT("Previous Attr data deleted.\n");
			}
		}
		else
		{
			DEBUG_PRT("Peer not found,	host tried to re-establish secure connection but is	not	bonded with	this device\n");
		}
		break;
	default:
		break;
	}
}
