/*********************************************************************

File    : peripheral_base.cpp
Purpose : BLE peripheral device base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "peripheral_base.h"
#include "debug_printf.h"

nrf_nvic_state_t nrf_nvic_state;        // nvic	status keeper, used	by sd_nvic_EnableIRQ()
										// anywhere	uses sd_nvic_XXX() must	use
										// "extern nrf_nvic_state_t nrf_nvic_state" to declare it;

peripheral_base_t::peripheral_base_t()
	: pClockConfig(&CLK_CFG)
{
	uint32_t result = 0;
	result = sd_softdevice_enable(pClockConfig, ble_fault_handler);
	if (result != NRF_SUCCESS)
	{
		while (1)
			;        // SoftDevice enable error
	}

	result = sd_nvic_EnableIRQ(SWI2_IRQn);        // enable	softdevice event IRQ
	if (result != NRF_SUCCESS)
	{
		while (1)
			;        // SWI2 interrupt enable error
	}
	result = sd_ble_enable(&RamStart);
	if (result != NRF_SUCCESS)
	{
		// Probably not enough memeory is assigned, required minimum RAM size is now in RamStart
		while (1)
			;        // BLE stack enable error
	}
}

peripheral_base_t::peripheral_base_t(const nrf_clock_lf_cfg_t* pClockCfg)
	: pClockConfig(pClockCfg)
{
	uint32_t result = 0;
	result = sd_softdevice_enable(pClockConfig, ble_fault_handler);
	if (result != NRF_SUCCESS)
	{
		while (1)
			;        // SoftDevice enable error
	}

	result = sd_nvic_EnableIRQ(SWI2_IRQn);        // enable	softdevice event IRQ
	if (result != NRF_SUCCESS)
	{
		while (1)
			;        // SWI2 interrupt enable error
	}
	result = sd_ble_enable(&RamStart);
	if (result != NRF_SUCCESS)
	{
		// Probably not enough memeory is assigned, required minimum RAM size is now in RamStart
		while (1)
			;        // BLE stack enable error
	}
}

void peripheral_base_t::ble_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
	DEBUG_PRT("ble module hard fault\n");
	while (1)
		;
}
