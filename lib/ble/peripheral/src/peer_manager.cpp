/*********************************************************************

File    : peer_manager.cpp
Purpose : BLE bonding peer manager class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include <ble.h>          // for ble related data type definitions
#include <cstddef>        // definition of NULL
#include <peer_manager.h>

uint32_t peer_manager_t::save_LTK(const ble_gap_enc_key_t& LocalLTK)
{
	PeerDataBuf.LTK.Mark = 0xffffff00;
	PeerDataBuf.LTK.EncKeys = LocalLTK;
	//	delete_LTK(LocalLTK.master_id);		// delete LTK for current peer device if exist
	return LTKMem.save_unit(&PeerDataBuf);
}

uint32_t peer_manager_t::save_SysAttr(const ble_gap_master_id_t& Master_ID, const uint8_t* SysAttrData, uint16_t AttrLen)
{
	PeerDataBuf.Attr.Mark = 0xffffff00;
	PeerDataBuf.Attr.MasterID = Master_ID;
	if ((SysAttrData != NULL) && (AttrLen != 0))
	{
		for (int i = 0; i < AttrLen; i++)
		{
			PeerDataBuf.Attr.Data[i] = *(SysAttrData + i);
		}
		PeerDataBuf.Attr.Len = AttrLen;
		//		delete_SysAttr(Master_ID);		// if Attr data exist for current peer device, delete it.
		return AttrMem.save_unit(&PeerDataBuf);
	}
	else
	{
		return NRF_ERROR_INVALID_PARAM;
	}
}

// find peer data from sotrage by MasterID, if found, peer LTK and corresponding system attribute data and its length
// will be written to LTK and SysAttr, Len (if the pointer is not NULL)
uint32_t peer_manager_t::find_LTK(const ble_gap_master_id_t& Master_ID, ble_gap_enc_key_t* LTK)
{
	bool     RandEqual;
	uint16_t i;
	if (LTKMem.goto_begin() == NRF_SUCCESS)
	{
		do
		{
			LTKMem.read_unit(&PeerDataBuf);
			if (PeerDataBuf.LTK.EncKeys.master_id.ediv == Master_ID.ediv)
			{
				RandEqual = true;
				for (i = 0; i < BLE_GAP_SEC_RAND_LEN; i++)
				{
					if (PeerDataBuf.LTK.EncKeys.master_id.rand[i] != Master_ID.rand[i])
					{
						RandEqual = false;
						break;
					}
				}
				if (RandEqual)        // matching peer data found
				{
					if (LTK != NULL)
					{
						*LTK = PeerDataBuf.LTK.EncKeys;
					}
					return NRF_SUCCESS;
				}
			}
		} while (LTKMem.goto_next() != NRF_ERROR_NOT_FOUND);
	}
	return NRF_ERROR_NOT_FOUND;
}

uint32_t peer_manager_t::find_SysAttr(const ble_gap_master_id_t& Master_ID, uint8_t* SysAttr, uint16_t* Len)
{
	bool     RandEqual;
	uint16_t i;
	if (AttrMem.goto_begin() == NRF_SUCCESS)
	{
		do
		{
			AttrMem.read_unit(&PeerDataBuf);
			if (PeerDataBuf.Attr.MasterID.ediv == Master_ID.ediv)
			{
				RandEqual = true;
				for (i = 0; i < BLE_GAP_SEC_RAND_LEN; i++)
				{
					if (PeerDataBuf.Attr.MasterID.rand[i] != Master_ID.rand[i])
					{
						RandEqual = false;
						break;
					}
				}
				if (RandEqual)        // matching peer data found
				{
					if (SysAttr != NULL)
					{
						for (i = 0; i < PeerDataBuf.Attr.Len; i++)
						{
							*(SysAttr + i) = PeerDataBuf.Attr.Data[i];
						}
						if (Len != NULL)
						{
							*Len = PeerDataBuf.Attr.Len;
						}
					}
					return NRF_SUCCESS;
				}
			}
		} while (AttrMem.goto_next() != NRF_ERROR_NOT_FOUND);
	}
	return NRF_ERROR_NOT_FOUND;
}

uint32_t peer_manager_t::delete_LTK(const ble_gap_master_id_t& MasterID)
{
	if (find_LTK(MasterID, NULL) == NRF_SUCCESS)
	{
		return LTKMem.delete_unit();
	}
	return NRF_ERROR_NOT_FOUND;
}

uint32_t peer_manager_t::delete_SysAttr(const ble_gap_master_id_t& MasterID)
{
	if (find_SysAttr(MasterID, NULL, NULL) == NRF_SUCCESS)
	{
		return AttrMem.delete_unit();
	}
	return NRF_ERROR_NOT_FOUND;
}

uint32_t peer_manager_t::erase_all_LTK()
{
	return LTKMem.erase_all();
}

uint32_t peer_manager_t::erase_all_SysAttr()
{
	return AttrMem.erase_all();
}
