#ifndef	NRF_GPIO_H
#define NRF_GPIO_H

/*********************************************************************

File    : nrf_gpio.h
Purpose : header file for gpio class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <nrf52.h>

class nrf_p0_t
{
public:
	nrf_p0_t();

	void set_1(uint32_t PinMap)			// when bit in PinMap is 1, set corresponding pins at P0 to HIGH 
	{
		NRF_P0->OUTSET = PinMap;
	}

	void set_0(uint32_t PinMap)
	{
		NRF_P0->OUTCLR = PinMap;
	}

	void set_output(uint32_t PinMap)
	{
		NRF_P0->DIRSET = PinMap;		// DIR register set 1 to set pin output
	}

	void set_input(uint32_t PinMap)
	{
		NRF_P0->DIRCLR = PinMap;
	}

	uint32_t read_port()
	{
		return NRF_P0->IN;
	}
};

class nrf_pin_t
{
public:
	nrf_pin_t(uint8_t PinNumber) : PinNum(PinNumber)		// PinNumber must < 32
	{}

	uint8_t PinNum;
	uint8_t GPIOTE_Ch;

	void set_0()
	{
		NRF_P0->OUTCLR = (0x01UL)<<PinNum;
	}

	void set_1()
	{
		NRF_P0->OUTSET = (0x01UL)<<PinNum;
	}

	bool read()
	{
		return NRF_P0->IN&((0x01UL)<<PinNum);
	}

	void set_input()			// this is power-on default mode
	{
		NRF_P0->PIN_CNF[PinNum] &= ~(0x01UL);
	}

	void set_output()
	{
		NRF_P0->PIN_CNF[PinNum] |= 0x01UL;
	}

	void set_pullup()
	{
		NRF_P0->PIN_CNF[PinNum] |= 0x0cUL;
	}

	void set_pulldown()
	{
		disable_pull();
		NRF_P0->PIN_CNF[PinNum] |= 0x04UL;
	}

	void disable_pull()			// this is power-on default mode
	{
		NRF_P0->PIN_CNF[PinNum] &= ~(0x0cUL);
	}

	// drive mode:
	// 0 - standard 0 and 1
	// 1 - high drive 0, standart 1
	// 2 - standard 0, high drive 1
	// 3 - high drive 0, high drive 1
	// 4 - open 0, standard 1
	// 5 - open 0, high drive 1
	// 6 - standard 0, open 1 (OD)
	// 7 - high drive 0, open 1 (OD)
	void set_drive_mode(uint32_t Mode)
	{
		NRF_P0->PIN_CNF[PinNum] &= ~(0x70UL);
		NRF_P0->PIN_CNF[PinNum] |= (Mode&0x07)<<8;
	}

	void disable_sense()		// this is power-on default mode
	{
		NRF_P0->PIN_CNF[PinNum] &= ~(0x00030000UL);
	}

	void set_sense_1()
	{
		disable_sense();
		NRF_P0->PIN_CNF[PinNum] |= 0x00020000UL;
	}

	void set_sense_0()
	{
		NRF_P0->PIN_CNF[PinNum] |= 0x00030000UL;
	}

	void disable_gpiote(uint8_t Ch)		// GPIOTE is disabled by default, Ch must < 8, will have no effect if GPIOTE channel Ch is not set to this pin
	{
		if (((NRF_GPIOTE->CONFIG[Ch]>>8)&0x1f) == PinNum)
		{
			NRF_GPIOTE->CONFIG[Ch] &= ~(0x03UL);
		}
	}

	// Parameter values:
	// Ch -- 0-7
	// Mode -- 0-disabled, 1-Event, 3-Task
	// Polarity -- 0-None, 1-Lo to Hi, 2-Hi to Lo, 3-Toggle
	// InitState -- 0-low, 1-high
	void set_gpiote(uint8_t Ch, uint8_t Mode, uint8_t Polarity, uint8_t InitState)
	{
		GPIOTE_Ch = Ch;
		NRF_GPIOTE->CONFIG[Ch] = (((uint32_t)InitState&0x01)<<20) | (((uint32_t)Polarity&0x03)<<16) | ((uint32_t)PinNum<<8) | ((uint32_t)Mode&0x03);
	}
};

#endif