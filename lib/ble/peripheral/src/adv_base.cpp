/*********************************************************************

File    : adv_base.cpp
Purpose : BLE advertising class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "adv_base.h"
#include "param_const.h"
#include <stdio.h>
#include <string.h>        // strlen()
#include "debug_printf.h"

adv_base_t::adv_base_t()
{
	uint32_t Result;
	packet_add_flags(&MainPkt);
	PrivacyParam = DEFAULT_PRIVACY_PARAM;
	Params = PUBLIC_CONNECTABLE_ADV_PARAM;
	Result = update_adv_data();
	if (Result != NRF_SUCCESS)
	{
		while (1)
			;        // Adv Init Error
	}
}

adv_base_t::adv_base_t(const char* pDevName)
{
	uint32_t Result;
	packet_add_flags(&MainPkt);
	packet_add_name(&MainPkt, pDevName);
	PrivacyParam = DEFAULT_PRIVACY_PARAM;
	Params = PUBLIC_CONNECTABLE_ADV_PARAM;
	Result = update_adv_data();
	if (Result != NRF_SUCCESS)
	{
		while (1)
			;        // Avd Init	Error
	}
}

void adv_base_t::clear_packet(adv_packet_t* pPacket)
{
	pPacket->Len = 0;
}

uint32_t adv_base_t::packet_add_flags(adv_packet_t* pPacket)
{
	if (pPacket->Len + 3 > 31)
	{
		return NRF_ERROR_INVALID_LENGTH;
	}
	pPacket->Data[pPacket->Len] = 2;
	pPacket->Data[pPacket->Len + 1] = 0x01;        // flags	type value
	pPacket->Data[pPacket->Len + 2] = 0x06;        // 0x06=General	discoverable, no BR/EDR
	pPacket->Len += 3;
	return NRF_SUCCESS;
}

uint32_t adv_base_t::packet_add_name(adv_packet_t* pPacket, const char* pName)
{
	uint8_t StrLen;
	StrLen = strlen(pName);
	if (pPacket->Len + StrLen + 2 > 31)
	{
		return NRF_ERROR_INVALID_LENGTH;
	}
	pPacket->Data[pPacket->Len] = StrLen + 1;
	pPacket->Data[pPacket->Len + 1] = 0x09;        // value for complete local name type
	for (int i = 0; i < StrLen; i++)
	{
		pPacket->Data[pPacket->Len + 2 + i] = *(pName + i);
	}
	pPacket->Len += StrLen + 2;
	return NRF_SUCCESS;
}

uint32_t adv_base_t::packet_add_appear(adv_packet_t* pPacket, const uint16_t Appearance)
{
	if (pPacket->Len + 4 > 31)
	{
		return NRF_ERROR_INVALID_LENGTH;
	}
	pPacket->Data[pPacket->Len] = 3;
	pPacket->Data[pPacket->Len + 1] = 0x19;        // value for appearance type
	pPacket->Data[pPacket->Len + 2] = Appearance & 0xff;
	pPacket->Data[pPacket->Len + 3] = Appearance >> 8;
	pPacket->Len += 4;
	return NRF_SUCCESS;
}

uint32_t adv_base_t::packet_add_tx_pwr(adv_packet_t* pPacket, const uint8_t TxPower)
{
	if (pPacket->Len + 3 > 31)
	{
		return NRF_ERROR_INVALID_LENGTH;
	}
	pPacket->Data[pPacket->Len] = 2;
	pPacket->Data[pPacket->Len + 1] = 0x0A;        // value for tx power type
	pPacket->Data[pPacket->Len + 2] = TxPower;
	pPacket->Len += 3;
	return NRF_SUCCESS;
}

uint32_t adv_base_t::update_adv_data()
{
	Data = { { MainPkt.Data, MainPkt.Len }, { RspPkt.Data, RspPkt.Len } };
	return sd_ble_gap_adv_set_configure(&Handle, &Data, &Params);
}

uint32_t adv_base_t::config_adv_type(const uint8_t AdvType, const ble_gap_addr_t* const pPeerAddr)
{
	Params.properties = { AdvType, 0, 0 };        // type, anonymous,	include_tx_power (later	2 only available for extended mode)
	Params.p_peer_addr = pPeerAddr;

	return sd_ble_gap_adv_set_configure(&Handle, &Data, &Params);        // Handle get set when	this function is called
}

uint32_t adv_base_t::set_timing(const uint32_t Interval, const uint16_t Duration)
{
	Params.interval = Interval;
	Params.duration = Duration;

	return sd_ble_gap_adv_set_configure(&Handle, &Data, &Params);
}

uint32_t adv_base_t::set_adv_param(const ble_gap_adv_params_t* pParam)
{
	Params = *pParam;
	return sd_ble_gap_adv_set_configure(&Handle, &Data, &Params);
}

uint32_t adv_base_t::set_public()
{
	uint32_t Result;
	PrivacyParam = DEFAULT_PRIVACY_PARAM;
	Result = sd_ble_gap_privacy_set(&PrivacyParam);
	if (Result != NRF_SUCCESS)
	{
		DEBUG_PRT("privacy set OFF error\n");
		return Result;
	}
	Result = config_adv_type(BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED, NULL);
	if (Result != NRF_SUCCESS)
	{
		DEBUG_PRT("pubic adv setting error\n");
		return Result;
	}
	return NRF_SUCCESS;
}

uint32_t adv_base_t::set_private()
{
	uint32_t Result;
	PrivacyParam.privacy_mode = BLE_GAP_PRIVACY_MODE_DEVICE_PRIVACY;
	PrivacyParam.private_addr_type = BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE;
	PrivacyParam.private_addr_cycle_s = 120;        //	address	avaible	time in seconds
	PrivacyParam.p_device_irk = NULL;               //	NULL to use device default IRK
	Result = sd_ble_gap_privacy_set(&PrivacyParam);
	if (Result != NRF_SUCCESS)
	{
		DEBUG_PRT("privacy set ON error\n");
		return Result;
	}
	Result = config_adv_type(BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED, NULL);
	if (Result != NRF_SUCCESS)
	{
		DEBUG_PRT("private adv setting error\n");
		return Result;
	}
	return NRF_SUCCESS;
}

uint32_t adv_base_t::start()
{
	return sd_ble_gap_adv_start(Handle, 0);
}

uint32_t adv_base_t::stop()
{
	return sd_ble_gap_adv_stop(Handle);
}

