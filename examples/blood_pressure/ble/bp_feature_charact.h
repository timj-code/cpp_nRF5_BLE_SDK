#ifndef BP_FEATURE_CHARACT_H
#define BP_FEATURE_CHARACT_H

#include "charact_base.h"


const uint16_t UUID_BLOOD_FEATURE = 0x2A49;        // blood pressure measurement features

struct bp_features_t
{
	uint16_t BodyMoveDect : 1;		// 0=body move detection not supported, 1=supported
	uint16_t CuffFitDect : 1;		// 0=not supported, 1=supported
	uint16_t IrrugularPulseDect : 1;	// 0=not supported, 1=supported
	uint16_t PulseRangeDect : 1;		// 0=not supported, 1=supported
	uint16_t MeasurePosDect : 1;		// 0=not supported, 1=supported
	uint16_t MultiBondSupport : 1;		// 0=not supported, 1= supported
	uint16_t Reserved : 10;
};

const bp_features_t BP_FEATURES = {
	.BodyMoveDect = 0,
	.CuffFitDect = 0,
	.IrrugularPulseDect = 0,
	.PulseRangeDect = 0,
	.MeasurePosDect = 0,
	.MultiBondSupport = 1,
	.Reserved = 0
};

class bp_feature_charact_t : public charact_base_t
{
public:
	bp_feature_charact_t()
		: charact_base_t(UUID_BLOOD_FEATURE, &READONLY_CHARACT_META_DATA)
	{
		// character value attributes
		ValueAttribute.init_len = sizeof(bp_features_t);
		ValueAttribute.init_offs = 0;
		ValueAttribute.max_len = sizeof(bp_features_t);
		ValueAttribute.p_value = (uint8_t*)&BPFeatures;        // by SDK default, characteristic values are stored in user memory(as class member)
	}
	bp_features_t BPFeatures = BP_FEATURES;
};

#endif
