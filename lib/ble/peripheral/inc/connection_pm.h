#ifndef CONNECTION_PM_H
#define CONNECTION_PM_H

/*********************************************************************

File    : connection_pm_base.h
Purpose : header file for connection with peer manager base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "connection_base.h"
#include "peer_manager.h"
#include <ble.h>

class connection_pm_t : public connection_base_t
{
public:
	connection_pm_t(storage_t& LTK_Mem, storage_t& Attr_Mem, const char* pName);
	connection_pm_t(storage_t& LTK_Mem, storage_t& Attr_Mem, const char* pName, const ble_gap_sec_params_t* pBondingCfg);
	void evt_handler(const ble_evt_t* Event);

	bool BondedConnection;

private:
	// these are the 4 keys may	be exchanged in	a pairing/bonding process
	ble_gap_enc_key_t      encrypt_key_local, encrypt_key_peer;          // actual key value storage for local and received LTK
	ble_gap_id_key_t       identity_key_local, identity_key_peer;        // actual key value storage for local and receivd IRK
	ble_gap_sign_info_t    sign_key_local, sign_key_peer;
	ble_gap_lesc_p256_pk_t lesc_key_local, lesc_key_peer;

	// System attribute data
	uint8_t  SysAttrData[128];
	uint16_t SysAttrLen;

	const ble_gap_sec_params_t* pBondingConfig;
	// a collection of pointers	towards	local and peer keys
	ble_gap_sec_keyset_t Keyset = {
		{ &encrypt_key_local, &identity_key_local, &sign_key_local, &lesc_key_local },
		{ &encrypt_key_peer, &identity_key_peer, &sign_key_peer, &lesc_key_peer }
	};

	peer_manager_t Pm;
};

#endif
