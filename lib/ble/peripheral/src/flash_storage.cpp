/*********************************************************************

File    : flash_storage.cpp
Purpose : flash storage class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "flash_storage.h"
#include <ble.h>        // for data	type definitions
#include <cstddef>
#include <nrf_soc.h>        // for sd_flash_xxxx functions

flash_storage_t::flash_storage_t(const uint32_t Page_Address)
{
	pPage = (const flash_page_t*)Page_Address;
}

uint32_t flash_storage_t::save_unit(const save_unit_t* pSource)
{
	uint32_t Result;
	uint16_t UnitNum;

	UnitNum = find_first_free_unit();
	if (UnitNum == 0xffff)        // if storage is full
	{
		return NRF_ERROR_NO_MEM;
	}

	Result = sd_flash_write((uint32_t*)&pPage->Unit[UnitNum], (uint32_t*)pSource, sizeof(save_unit_t) / sizeof(uint32_t));
	return Result;
}

uint32_t flash_storage_t::read_unit(save_unit_t* pTarget)
{
	uint16_t i;
	if (pTarget != NULL)
	{
		for (i = 0; i < (sizeof(save_unit_t) / sizeof(uint32_t)); i++)
		{
			*((uint32_t*)pTarget + i) = *((uint32_t*)&pPage->Unit[CurrentUnit] + i);
		}
		return NRF_SUCCESS;
	}
	return NRF_ERROR_INVALID_PARAM;
}

uint32_t flash_storage_t::delete_unit()
{
	uint32_t Result;
	Result = sd_flash_write((uint32_t*)&pPage->Unit[CurrentUnit].PlaceHolder.Mark, &DELETED_MARK, 1);
	return Result;
}

uint32_t flash_storage_t::erase_all()
{
	return sd_flash_page_erase(((uint32_t)pPage) / FLASH_PAGE_SIZE);
}

uint16_t flash_storage_t::find_first_free_unit()
{
	for (uint16_t i = 0; i < TOTAL_UNITS; i++)
	{
		if (pPage->Unit[i].PlaceHolder.Mark == 0xffffffff)        // if there is free unit
		{
			return i;
		}
	}
	return 0xffff;
}

uint32_t flash_storage_t::goto_begin()
{
	for (CurrentUnit = 0; CurrentUnit < TOTAL_UNITS; CurrentUnit++)
	{
		if ((pPage->Unit[CurrentUnit].PlaceHolder.Mark != 0) && (pPage->Unit[CurrentUnit].PlaceHolder.Mark != 0xffffffff))
		{
			return NRF_SUCCESS;
		}
	}
	CurrentUnit = 0;
	return NRF_ERROR_NOT_FOUND;
}

uint32_t flash_storage_t::goto_next()
{
	for (uint16_t i = CurrentUnit + 1; i < TOTAL_UNITS; i++)
	{
		if ((pPage->Unit[i].PlaceHolder.Mark != 0) && (pPage->Unit[i].PlaceHolder.Mark != 0xffffffff))
		{
			CurrentUnit = i;
			return NRF_SUCCESS;
		}
	}
	return NRF_ERROR_NOT_FOUND;
}
