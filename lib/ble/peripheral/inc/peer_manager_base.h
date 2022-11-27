#ifndef PEER_MANAGER_BASE_H
#define PEER_MANAGER_BASE_H

#include <ble.h>          // for ble related data type definitions
#include <stdint.h>       // for general data type definitions

class Peer_Manager_Base
{
public:
	virtual uint32_t save_LTK(const ble_gap_enc_key_t &LocalLTK) = 0;                 // save LTK for bonded peer, NRF_SUCCESS if success, otherwise error code
	virtual uint32_t find_LTK(const ble_gap_master_id_t &Master_ID, ble_gap_enc_key_t *LTK) = 0;	// find peer LTK by masterID, return NRF_ERROR_NOT_FOUND if not found
	virtual uint32_t save_SysAttr(const ble_gap_master_id_t &Master_ID, const uint8_t *SysAttrData, uint16_t AttrLen) = 0;  // save System Attributes  
	virtual uint32_t find_SysAttr(const ble_gap_master_id_t &Master_ID, uint8_t *SysAttr, uint16_t *Len) = 0;	// find system attributes by masterID
	virtual uint32_t delete_LTK(const ble_gap_master_id_t &Master_ID) = 0;
	virtual uint32_t delete_SysAttr(const ble_gap_master_id_t &Master_ID) = 0;
	virtual uint32_t erase_all_LTK() = 0;
	virtual uint32_t erase_all_SysAttr() = 0;
};


#endif