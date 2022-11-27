#ifndef PEER_MANAGER_H
#define PEER_MANAGER_H

/*********************************************************************

File    : peer_manager.h
Purpose : header file for BLE bonding peer manager class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <stdint.h>       // for general data type definitions
#include <storage.h>      // storage API definitions


class peer_manager_t
{
public:
	peer_manager_t(storage_t& LTK_Mem, storage_t& Attr_Mem) : LTKMem(LTK_Mem), AttrMem(Attr_Mem)
	{}
	uint32_t save_LTK(const ble_gap_enc_key_t &LocalLTK);                 // save LTK for bonded peer, NRF_SUCCESS if success, otherwise error code
	uint32_t find_LTK(const ble_gap_master_id_t &Master_ID, ble_gap_enc_key_t *LTK);	// find peer LTK by masterID, return NRF_ERROR_NOT_FOUND if not found
	uint32_t save_SysAttr(const ble_gap_master_id_t &Master_ID, const uint8_t *SysAttrData, uint16_t AttrLen);  // save System Attributes  
	uint32_t find_SysAttr(const ble_gap_master_id_t &Master_ID, uint8_t *SysAttr, uint16_t *Len);	// find system attributes by masterID
	uint32_t delete_LTK(const ble_gap_master_id_t &Master_ID);
	uint32_t delete_SysAttr(const ble_gap_master_id_t &Master_ID);
	uint32_t erase_all_LTK();
	uint32_t erase_all_SysAttr();

private:
	storage_t& LTKMem;
	storage_t& AttrMem;
	save_unit_t     PeerDataBuf;
};

#endif
