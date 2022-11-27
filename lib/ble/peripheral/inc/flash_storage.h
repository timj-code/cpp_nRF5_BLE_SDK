#ifndef FLASH_STORAGE_H
#define FLASH_STORAGE_H

/*********************************************************************

File    : flash_storage.h
Purpose : header file for flash class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <save_unit.h>        // save unit data structure
#include <stdint.h>
#include <storage.h>

const uint32_t DELETED_MARK = 0;
const uint32_t FLASH_PAGE_SIZE = 0x1000;        // size of 1 flash page (min. erase block size) for nRF52832 is 0x1000 bytes (4K)
const uint16_t TOTAL_UNITS = FLASH_PAGE_SIZE / sizeof(save_unit_t);

union flash_page_t
{
	uint32_t    Words[FLASH_PAGE_SIZE / sizeof(uint32_t)];
	save_unit_t Unit[FLASH_PAGE_SIZE / sizeof(save_unit_t)];
};

// define the bonding data storage start address (using	1 flash	memory page 0x1000 bytes)
// The number 0x26000 comes from the softdevice	release	notes, also the	RAM start address
// so the code will be placed from 0x27000 (need to edit project options)
// option -> Common -> linker -> Section Placement Marcros -> $FLASH_START

// A storage unit is unused(blank) if all data in it are 0xff
// A unit is deleted if all data are 0x00

class flash_storage_t : public storage_t
{
public:
	flash_storage_t(const uint32_t Page_Address);          // use page start address as start
	uint32_t save_unit(const save_unit_t* pSource);        // save passed Save_Unit to flash, return NRF_SUCCESS	if no error
	uint32_t read_unit(save_unit_t* pTarget);              // read current unit to Target
	uint32_t delete_unit();                                // delete current unit
	uint32_t erase_all();                                  // erase all data unit
	uint32_t goto_begin();                                 // return NRF_SUCCESS if success, NRF_ERROR_NOT_FOUND if no valid unit found(all deleted or free)
	uint32_t goto_next();                                  // return NRF_SUCCESS if success, NRF_ERROR_NOT_FOUND if no more valid(not deleted or free) unit available

private:
	uint16_t            CurrentUnit;
	const flash_page_t* pPage;
	bool                AtEnd;
	bool                IsFull;
	uint16_t            find_first_free_unit();        // return first free unit, if none available, return 0xffff;
};

#endif
