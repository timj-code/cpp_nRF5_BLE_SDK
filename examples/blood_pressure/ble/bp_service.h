#ifndef BP_SERVICE_H
#define BP_SERVICE_H

#include "bp_feature_charact.h"
#include "bp_measure_charact.h"
#include "service_base.h"

const uint16_t UUID_BPS_SERVICE = 0x1810;

class bp_service_t : public service_base_t
{
public:
	bp_feature_charact_t CharactBPFeature;
	bp_measure_charact_t CharactBloodPressure;

	bp_service_t()
		: service_base_t(UUID_BPS_SERVICE)
	{
		CharactBPFeature.add_to_service(Handle);
		CharactBloodPressure.add_to_service(Handle);
	}
};

#endif
