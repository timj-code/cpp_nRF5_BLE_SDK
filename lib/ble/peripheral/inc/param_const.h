#ifndef PARAM_CONST_H
#define PARAM_CONST_H

/*********************************************************************

File    : param_const.h
Purpose : header file for constant parameters
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <ble.h>            // ble	related	data types and sd_ble_xxxx calls
#include <nrf_sdm.h>        //	LF clock definitions, sd_softdevice_xxx	functions
#include <stddef.h>         // definition of NULL


// default ram start address for softdevice 132	(found in softdevice release notes)
const uint32_t USER_RAM_START = 0x20003400;        // this	must be	set equal to the user ram start	address	in link	configure (ses_nrf52832_xxaa.icf).

const ble_gap_conn_sec_mode_t OPEN_LINK = {
	.sm = 1,
	.lv = 1
};

const ble_gap_conn_sec_mode_t ENCRYPTED_LINK = {
	.sm = 1,
	.lv = 1
};

const ble_gatts_attr_md_t DEFAULT_ATTR_META_DATA = {
	.read_perm = OPEN_LINK,
	.write_perm = OPEN_LINK,
	.vlen = 0,        // not variable length
	.vloc = BLE_GATTS_VLOC_USER,
	.rd_auth = 0,        // doesn't need authorization to read
	.wr_auth = 0         // doesn't need authorization to write
};

const ble_gatt_char_props_t RW_NOTIFY_CHAR_PROPS = {
	.broadcast = 0,
	.read = 1,
	.write_wo_resp = 0,
	.write = 1,
	.notify = 1,
	.indicate = 0,
	.auth_signed_wr = 0
};

const ble_gatt_char_props_t RW_INDICATE_CHAR_PROPS = {
	.broadcast = 0,
	.read = 1,
	.write_wo_resp = 0,
	.write = 1,
	.notify = 0,
	.indicate = 1,
	.auth_signed_wr = 0
};

const ble_gatt_char_props_t RW_CHAR_PROPS = {
	.broadcast = 0,
	.read = 1,
	.write_wo_resp = 0,
	.write = 1,
	.notify = 0,
	.indicate = 0,
	.auth_signed_wr = 0
};

const ble_gatt_char_props_t READONLY_CHAR_PROPS = {
	.broadcast = 0,
	.read = 1,
	.write_wo_resp = 0,
	.write = 0,
	.notify = 0,
	.indicate = 0,
	.auth_signed_wr = 0
};

const ble_gatt_char_ext_props_t DEFAULT_EXT_PROPS = {
	.reliable_wr = 0,
	.wr_aux = 0
};

const ble_gatts_char_md_t RW_CHARACT_META_DATA = {
	.char_props = RW_CHAR_PROPS,
	.char_ext_props = DEFAULT_EXT_PROPS,
	.p_char_user_desc = NULL,
	.char_user_desc_max_size = 0,
	.char_user_desc_size = 0,
	.p_char_pf = NULL,
	.p_user_desc_md = NULL,
	.p_cccd_md = NULL,
	.p_sccd_md = NULL
};

const ble_gatts_char_md_t RW_NOTIFY_CHARACT_META_DATA = {
	.char_props = RW_NOTIFY_CHAR_PROPS,
	.char_ext_props = DEFAULT_EXT_PROPS,
	.p_char_user_desc = NULL,
	.char_user_desc_max_size = 0,
	.char_user_desc_size = 0,
	.p_char_pf = NULL,
	.p_user_desc_md = NULL,
	.p_cccd_md = NULL,
	.p_sccd_md = NULL
};

const ble_gatts_char_md_t RW_INDICATE_CHARACT_META_DATA = {
	.char_props = RW_INDICATE_CHAR_PROPS,
	.char_ext_props = DEFAULT_EXT_PROPS,
	.p_char_user_desc = NULL,
	.char_user_desc_max_size = 0,
	.char_user_desc_size = 0,
	.p_char_pf = NULL,
	.p_user_desc_md = NULL,
	.p_cccd_md = NULL,
	.p_sccd_md = NULL
};

const ble_gatts_char_md_t READONLY_CHARACT_META_DATA = {
	.char_props = READONLY_CHAR_PROPS,
	.char_ext_props = DEFAULT_EXT_PROPS,
	.p_char_user_desc = NULL,
	.char_user_desc_max_size = 0,
	.char_user_desc_size = 0,
	.p_char_pf = NULL,
	.p_user_desc_md = NULL,
	.p_cccd_md = NULL,
	.p_sccd_md = NULL
};

// keys	can be sent to peer
const ble_gap_sec_kdist_t LTK_IRK = {
	.enc = 1,         // LTK, will always provided by peripheral
	.id = 1,          // IRK
	.sign = 0,        // key for signing, NOT SUPPORTED
	.link = 0         // for LESC paring
};

// keys	will get from peer
const ble_gap_sec_kdist_t NO_KEYS = {
	.enc = 0,
	.id = 0,
	.sign = 0,
	.link = 0
};

// default settings for	bonding	ablility (just work)
const ble_gap_sec_params_t BOND_JUSTWORK =        // params for	'just	works' bonding
	{
		.bond = 1,                              //	1-bit 0	or 1, whether support bond
		.mitm = 0,                              //	1-bit 0	or 1, whether support mitm
		.lesc = 0,                              //	1-bit 0	or 1, whether support lesc bonding
		.keypress = 0,                          //	1-bit 0	or 1, whether support keypress
		.io_caps = BLE_GAP_IO_CAPS_NONE,        //	io capabilites , see defines above
		.oob = 0,                               //	1-bit 0	or 1, whether support out of band
		.min_key_size = 7,                      //	minimum	encryption key size
		.max_key_size = 16,                     //	maximum	key size
		.kdist_own = LTK_IRK,                   //	keys local device will distribute
		.kdist_peer = NO_KEYS                   //	keys peer device will distribute
	};

// default main	clock source settings (affect connection interval and slave latency)
const nrf_clock_lf_cfg_t CLK_CFG = {
	.source = NRF_CLOCK_LF_SRC_XTAL,                // set this according to actual	circuit
	.rc_ctiv = 0,                                   // time	between	calibration, unit 0.25s	(range 1-32). must be 0	if .source != RC, if =RC then setting must keep	temperture change below	0.5C between calibration
	.rc_temp_ctiv = 0,                              // intervals between calibration. must be 0 if .source != RC.
	.accuracy = NRF_CLOCK_LF_ACCURACY_50_PPM        // adjust this according to actual board component
};

const ble_gap_privacy_params_t DEFAULT_PRIVACY_PARAM = {
	BLE_GAP_PRIVACY_MODE_OFF,               // advertise in	no private mode	by default
	BLE_GAP_ADDR_TYPE_RANDOM_STATIC,        // use public address by default
	120,                                    // if in private mode, change resolvable address change	every 120 seconds
	NULL                                    // by default, set IRK pointer to NULL to use default IRK
};

// Advertising type : Public Connectable
const ble_gap_adv_params_t PUBLIC_CONNECTABLE_ADV_PARAM = {
	{ BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED, 0, 0 },        // default advertising mode	and	anonymous and including-tx-power property, later 2 only	available for extended adv mode
	NULL,                                                               // default peer	address	pointer	is set to NULL,	if set to direct mode, this	must be	set	to the receiving device's addr
	400,                                                                // default advertising interval, unit in 0.625ms
	12000,                                                              // default duration	of advertising,	unit in	10ms
	0,                                                                  // don't limit maximum adv event numbers happened before stop adv. automatically
	{ 0, 0, 0, 0, 0 },                                                  // don't mask any channel
	BLE_GAP_ADV_FP_ANY,                                                 // set filter policy to	accept any device
	BLE_GAP_PHY_1MBPS,                                                  // primary advertising channel PHY
	BLE_GAP_PHY_1MBPS,                                                  // secondary advertising channel PHY
	0,                                                                  // advertising set id. (4-bit, only	available from extended	advertising	mode)
	0                                                                   // disable scan	request	event (1-bit, only available if	in scannable advertising mode)
};

// Advertising type : Public nonconnectable
const ble_gap_adv_params_t PUBLIC_NONCONNECTABLE_ADV_PARAM = {
	{ BLE_GAP_ADV_TYPE_NONCONNECTABLE_SCANNABLE_UNDIRECTED, 0, 0 },        // default advertising mode	and	anonymous and including-tx-power property, later 2 only	available for extended adv mode
	NULL,                                                                  // default peer	address	pointer	is set to NULL,	if set to direct mode, this	must be	set	to the receiving device's addr
	400,                                                                   // default advertising interval, unit in 0.625ms
	12000,                                                                 // default duration	of advertising,	unit in	10ms
	0,                                                                     // don't limit maximum adv event numbers happened before stop adv. automatically
	{ 0, 0, 0, 0, 0 },                                                     // don't mask any channel
	BLE_GAP_ADV_FP_ANY,                                                    // set filter policy to	accept any device
	BLE_GAP_PHY_1MBPS,                                                     // primary advertising channel PHY
	BLE_GAP_PHY_1MBPS,                                                     // secondary advertising channel PHY
	0,                                                                     // advertising set id. (4-bit, only	available from extended	advertising	mode)
	0                                                                      // disable scan	request	event (1-bit, only available if	in scannable advertising mode)
};

// Prefered	connection parameters for BLE, if need to change clock source settings
// refer to	server_base.cpp
const ble_gap_conn_params_t DEFAULT_PPCP = {
	.min_conn_interval = 50,         //	in 1.25ms units
	.max_conn_interval = 500,        //	in 1.25ms units
	.slave_latency = 1,
	.conn_sup_timeout = 400        // supervisor timeout in 10ms units
};

#endif
