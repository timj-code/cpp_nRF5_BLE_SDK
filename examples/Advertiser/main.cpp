/*********************************************************************

File    : main.cpp
Purpose : Main code entrance for advertiser project
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/


#include <cstdio>

#include "param_const.h"
#include "adv_device.h"

void create_temp_packt();

adv_device_t AdvDevice;

int main()
{
    AdvDevice.Adv.set_adv_param(&PUBLIC_NONCONNECTABLE_ADV_PARAM);
	AdvDevice.Adv.set_timing(400, 100);        // (Interval, Duration). Interval in 0.625ms units, Duration in
												// 10ms units
	create_temp_packt();
	AdvDevice.Adv.start();

	while (1)
	{
		sd_app_evt_wait();
	}
}

void create_temp_packt()
{
	int32_t Temperature;
	uint32_t Result;
	char	NameStr[31];			// Max Adv packet size is 31 bytes.
	Result = sd_temp_get(&Temperature);
	if (Result == NRF_SUCCESS)
	{
		printf("Room Temp %d.%d 0x%x\n", Temperature/4, Temperature%4*25, Temperature);
		sprintf(NameStr, "Room Temp %d.%d °C\n", Temperature/4, Temperature%4*25);
		AdvDevice.Adv.clear_packet(&AdvDevice.Adv.MainPkt);
		AdvDevice.Adv.packet_add_flags(&AdvDevice.Adv.MainPkt);
		AdvDevice.Adv.packet_add_appear(&AdvDevice.Adv.MainPkt, BLE_APPEARANCE_GENERIC_THERMOMETER);
		AdvDevice.Adv.packet_add_name(&AdvDevice.Adv.MainPkt, NameStr);
		AdvDevice.Adv.update_adv_data();
	}
}


// user	extern "C" because this	is a cpp file while	the	base event
// handler use c linkage
extern "C" void SWI2_EGU2_IRQHandler(void)        // softdevice	event handler
{
	uint8_t __attribute__((aligned(4))) EvtBuffer[sizeof(ble_evt_t) + 23];
	uint16_t EvtLen;
	uint16_t EvtID;
	uint32_t SocEvtID;

	// all the softdevice events come here
	EvtLen = sizeof(ble_evt_t) + 23;
	while (sd_ble_evt_get(EvtBuffer, &EvtLen) == NRF_SUCCESS)
	{
		EvtID = ((ble_evt_t*)EvtBuffer)->header.evt_id;
		printf("ble	event 0x%x\n", EvtID);
		if (EvtID == BLE_GAP_EVT_ADV_SET_TERMINATED)
		{
			printf("Advertising stopped\n");
			create_temp_packt();
			AdvDevice.Adv.start();
		}
	}
	while (sd_evt_get(&SocEvtID) == NRF_SUCCESS)
	{
		printf("soc	event 0x%x\n", SocEvtID);
	}
}

