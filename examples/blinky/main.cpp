/*********************************************************************

File    : main.cpp
Purpose : Main code entrance for blinky project
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <cstdio>

#include "blinky_device.h"
#include "debug_printf.h"
#include "nrf_gpio.h"
#include "param_const.h"

blinky_device_t BlinkyDevice("Cpp SDK Blinky");

nrf_pin_t P0_17(17);

int main()
{
	P0_17.set_output();
	P0_17.set_0();

	BlinkyDevice.Connection.set_ppcp(&DEFAULT_PPCP);
	BlinkyDevice.Adv.set_public();
	BlinkyDevice.Adv.set_timing(400, 36000);        // (Interval, Duration). Interval in 0.625ms units, Duration in
													// 10ms units
	BlinkyDevice.Adv.start();

	while (1)
	{
		sd_app_evt_wait();
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
		DEBUG_PRT("ble	event 0x%x\n", EvtID);

		BlinkyDevice.evt_handler((ble_evt_t*)EvtBuffer);
		switch (EvtID)
		{
		case BLE_GATTS_EVT_WRITE:        // if it's write event, update I/O status according to characteristic data
			if (BlinkyDevice.BlinkyService.CharactBlinky.Blinky.OutputBit == 1)
			{
				P0_17.set_1();
			}
			else
			{
				P0_17.set_0();
			}
			break;
		default:
			break;
		}
	}
	while (sd_evt_get(&SocEvtID) == NRF_SUCCESS)
	{
		DEBUG_PRT("soc	event 0x%x\n", SocEvtID);
	}
}
