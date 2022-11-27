#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>			// definition of data types
#include <save_unit.h>    // save unit data structure

// This is the virtual base class for storage class APIs
// Storage class for LTK and System Attributes should be derived from this class
class storage_t
{
public:
	// save passed "save_unit_t" to storage, return NRF_SUCCESS if no error
	// Source should be saved at the first available(neither deleted nor used) free space in media
	virtual uint32_t save_unit(const save_unit_t* pSource) = 0;

	// read current unit to pTarget, return NRF_SUCCESS if successful , return NRF_ERROR_INVALID_PARAM is Target is NULL
	// Storage class should have an internal pointer to current accessable memory unit
	virtual uint32_t read_unit(save_unit_t* pTarget) = 0;
	
	// delete current memory unit(mark as deleted), return NRF_SUCCESS if successful
	virtual uint32_t delete_unit() = 0;

	// erase all memory, return NRF_SUCCESS if successful
	virtual uint32_t erase_all() = 0;

	// goto first valid(containing data, not deleted, not empty) memory unit
	// return NRF_SUCCESS if successful, NRF_ERROR_NOT_FOUND if no valid unit found(all deleted or free)
	virtual uint32_t goto_begin() = 0;

	// goto next valid(containing data, not deleted or empty) memory unit
	// return NRF_SUCCESS if success, NRF_ERROR_NOT_FOUND if no more valid(not deleted or free) unit available
	virtual uint32_t goto_next() = 0;
};

#endif