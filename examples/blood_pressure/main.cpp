/*********************************************************************

File    : main.cpp
Purpose : Main code entrance for blood pressure project
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <cstdio>

#include "bp_device.h"
#include "debug_printf.h"
#include "flash_storage.h"
#include "nrf_gpio.h"
#include "nrf_rtc.h"
#include "param_const.h"

const uint32_t BONDING_DATA_START_ADDR = 0x7F000;
const uint32_t SYSTEM_ATTR_DATA_START_ADDR = 0x7E000;

void init_blood_pressure();
void update_blood_pressure();

flash_storage_t LTK_Storage(BONDING_DATA_START_ADDR);
flash_storage_t ATTR_Storage(SYSTEM_ATTR_DATA_START_ADDR);

bp_device_t BPDevice(LTK_Storage, ATTR_Storage, "Cpp BLE BP Device");

nrf_rtc_t RTC(NRF_RTC1);
bool      BPDueUpdate;

int main()
{
	uint32_t result;
	uint32_t PendingIRQ;


	RTC.stop();
	result = sd_nvic_SetPriority(RTC1_IRQn, 7);
	result = sd_nvic_EnableIRQ(RTC1_IRQn);
	if (result != NRF_SUCCESS)
	{
		DEBUG_PRT("EnableIRQ error 0x%x\n", result);
	}
	RTC.set_prescaler(1023);        // set RTC tick frequency to 32Hz
	RTC.set_compare(0, 64);         // set comarator 1 time to 2s
	RTC.clear_counter();
	RTC.interrupt_enable(0x00010000);        // enable RTC compare0 event
	RTC.start();

	init_blood_pressure();
	BPDueUpdate = false;

	BPDevice.Connection.set_ppcp(&DEFAULT_PPCP);
	BPDevice.Adv.set_public();
	BPDevice.Adv.set_timing(400, 36000);        // (Interval, Duration). Interval in 0.625ms units, Duration in
												// 10ms units
	BPDevice.Adv.packet_add_appear(&BPDevice.Adv.MainPkt, BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE);
	BPDevice.Adv.update_adv_data();
	BPDevice.Adv.start();
	BPDevice.BloodPressureService.CharactBloodPressure.set_notify_mode(BLE_GATT_HVX_INDICATION);

	while (1)
	{
		if (BPDevice.Connection.Connected)
		{
			if (BPDueUpdate)
			{
				update_blood_pressure();
				BPDueUpdate = false;
				result = BPDevice.BloodPressureService.CharactBloodPressure.notify(BPDevice.Connection.ConnHandle);
				if (result != NRF_SUCCESS)
				{
					DEBUG_PRT("send bps notification error=%X\n", result);
				}
			}
		}
		sd_app_evt_wait();
	}
}

void init_blood_pressure()
{
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.Flags.PulseRatePresent = 1;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.Flags.UnitsType = 0;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.Flags.TimeStampPresent = 1;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.Systolic = 120;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.Diastolic = 70;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.MAP = 100;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.PulseRate = 50;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Year = 2022;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Month = 11;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Day = 15;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Hours = 17;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Minutes = 22;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Seconds = 1;
}

void update_blood_pressure()
{
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.Systolic++;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.Diastolic++;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.MAP++;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.PulseRate++;
	BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Seconds++;
	if (BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Seconds >= 60)
	{
		BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Seconds = 0;
		BPDevice.BloodPressureService.CharactBloodPressure.BPMeasure.TimeStamp.Minutes++;
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
		DEBUG_PRT("ble event 0x%x\n", EvtID);
		BPDevice.evt_handler((ble_evt_t*)EvtBuffer);
	}
	while (sd_evt_get(&SocEvtID) == NRF_SUCCESS)
	{
		DEBUG_PRT("soc	event 0x%x\n", SocEvtID);
		switch (SocEvtID)
		{
		case NRF_EVT_FLASH_OPERATION_SUCCESS:
			DEBUG_PRT("Flash Operation Success\n");
			break;
		case NRF_EVT_FLASH_OPERATION_ERROR:
			DEBUG_PRT("Flash Operation Error\n");
			break;
		default:
			break;
		}
	}
}


extern "C" void RTC1_IRQHandler(void)
{
	RTC.clear_compare_event(0);
	RTC.clear_counter();
	BPDueUpdate = true;
}
