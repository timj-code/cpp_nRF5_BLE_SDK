#ifndef ADV_BASE_H
#define ADV_BASE_H

/*********************************************************************

File    : adv_base.h
Purpose : header file for advertising class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <ble_gap.h>        // ble_gap_xxxx	data types

struct adv_packet_t
{
	uint8_t Data[31];        //	adv packet is 31 bytes max
	uint8_t Len;
};

class adv_base_t
{
public:
	uint8_t      Handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;        // =0xff, must be set 0xff for initialization
	adv_packet_t MainPkt, RspPkt;

	adv_base_t();
	adv_base_t(const char* pName);
	void     clear_packet(adv_packet_t* pPacket);
	uint32_t packet_add_flags(adv_packet_t* pPacket);                         // Add ADV flags as 0x06=General discoverable, no BR/EDR
	uint32_t packet_add_name(adv_packet_t* pPacket, const char* Name);        // return value: operation result
	uint32_t packet_add_appear(adv_packet_t* pPacket, const uint16_t Appearance);
	uint32_t packet_add_tx_pwr(adv_packet_t* pPacket, const uint8_t TxPower);
	uint32_t update_adv_data();
	uint32_t config_adv_type(const uint8_t AdvType, const ble_gap_addr_t* const pPeerAddr);
	uint32_t set_adv_param(const ble_gap_adv_params_t* pParam);
	uint32_t set_public();                                                        // shortcut to set public connectable advertising
	uint32_t set_private();                                                       // shortcut to set directed private advertising
	uint32_t set_timing(const uint32_t Interval, const uint16_t Duration);        // Interval unit in 0.625ms, Duration unit in 10ms
	uint32_t start();
	uint32_t stop();
private:
	ble_gap_adv_data_t       Data = { { MainPkt.Data, MainPkt.Len }, { RspPkt.Data, RspPkt.Len } };
	ble_gap_privacy_params_t PrivacyParam;
	ble_gap_adv_params_t     Params;
};

#endif
