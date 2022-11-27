#ifndef ADV_DEVICE_H
#define ADV_DEVICE_H

/*********************************************************************

File    : adv_device.h
Purpose : header file for blood pressure class
Author  : Jian Jiao, jj8431@gmail.com
Date    : 20 Nov. 2022
Version : V1.0

*********************************************************************/

#include "adv_base.h"
#include "peripheral_base.h"

class adv_device_t : public peripheral_base_t
{
public:
	adv_device_t()
    {}
    adv_device_t(const char* pName) : Adv(pName)
    {}

	adv_base_t      Adv;
};

#endif
