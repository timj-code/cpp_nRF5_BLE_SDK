#ifndef PERIPHERAL_BASE_H
#define PERIPHERAL_BASE_H

/*********************************************************************

File    : peripheral_base.h
Purpose : header file for BLE peripheral device base class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <ble.h>
#include <cstdio>            //	printf() and	NULL
#include <nrf_nvic.h>        //	nrf_nvic functions
#include <nrf_sdm.h>         //	LF clock definitions, sd_softdevice_xxx	functions
#include "param_const.h"


extern nrf_nvic_state_t nrf_nvic_state;        // used in sd_nvic_EnableIRQ(), in case this function will be used elsewhere.

class peripheral_base_t
{
public:
	peripheral_base_t();
	peripheral_base_t(const nrf_clock_lf_cfg_t* pClockCfg);

private:
	uint32_t                  RamStart = USER_RAM_START;
	const nrf_clock_lf_cfg_t* pClockConfig;
	static void               ble_fault_handler(uint32_t id, uint32_t pc, uint32_t info);
};

#endif
