#ifndef	NRF_RTC_H
#define NRF_RTC_H

/*********************************************************************

File    : nrf_rtc.h
Purpose : header file for RTC class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <nrf52.h>

class nrf_rtc_t
{
public:
	nrf_rtc_t(NRF_RTC_Type* pRTCn) : pBase(pRTCn)
	{}

	NRF_RTC_Type*	pBase;
	
	void start()
	{
		pBase->TASKS_START = 1;
	}

	void stop()
	{
		pBase->TASKS_STOP = 1;
	}

	void clear_counter()
	{
		pBase->TASKS_CLEAR = 1;
	}

	void clear_overflow_event()
	{
		pBase->EVENTS_OVRFLW = 0;
	}

	void clear_tick_event()
	{
		pBase->EVENTS_TICK = 0;
	}

	void clear_compare_event(uint8_t Comparator)	// Comparator is 0-3
	{
		pBase->EVENTS_COMPARE[Comparator] = 0;
	}

	void set_prescaler(uint16_t Prescaler)		// max value for Prescaler is 0xfff(4095), which is 8Hz@32.768kHz LFCLK
	{
		pBase->PRESCALER = Prescaler;
	}

	void event_enable(uint32_t BitMap)			// see nrf52832 datasheet page 251 for bit mapping
	{
		pBase->EVTENSET = BitMap;
	}

	void event_disable(uint32_t BitMap)
	{
		pBase->EVTENCLR = BitMap;
	}

	void interrupt_enable(uint32_t BitMap)		// see nrf52832 datasheet page 249 for bit mapping
	{
		pBase->INTENSET = BitMap;
	}

	void interrupt_disable(uint32_t BitMap)
	{
		pBase->INTENCLR;
	}

	void set_compare(uint8_t Comparator, uint32_t Value)			// Comparator is 0-3, Value is 24bit
	{
		pBase->CC[Comparator] = Value;
	}
};

#endif