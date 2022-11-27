#ifndef SAVE_UNIT_H
#define SAVE_UNIT_H

#include <ble.h>		// BLE related data types

// Unified store unit for LTK and System Attributes saving

// When internal flash is used as storage,
// save unit size must > (flash page size/Nwrite), where Nwrite is the maximum number of writes can be done in a page
// before a page or chip erase must be executed. For nRF52832, Nwrite=181
// Save Unit size must be an integer mupltiple of 4( sizeof(uint32_t) )
#define SAVE_UNIT_SIZE 64

union save_unit_t
{
	struct
	{
		uint32_t Mark;
		uint8_t  Payload[SAVE_UNIT_SIZE - sizeof(uint32_t)];
	} PlaceHolder;
	struct
	{
		uint32_t          Mark;       // used for fast checking of storage unit status, 0xffffffff=free, 0x00000000=deleted, other=active
		ble_gap_enc_key_t EncKeys;    // in current default SD settings, the size of ble_gap_enc_key is 27 bytes
	} LTK;
	struct
	{
		uint32_t            Mark;    // used for fast checking of storage unit status, 0xffffffff=free, 0x00000000=deleted, other=active
		ble_gap_master_id_t MasterID;
		uint16_t            Len;
		uint8_t             Data[SAVE_UNIT_SIZE - sizeof(uint32_t) - sizeof(ble_gap_master_id_t) - sizeof(uint16_t)];
	} Attr;
};


#endif
