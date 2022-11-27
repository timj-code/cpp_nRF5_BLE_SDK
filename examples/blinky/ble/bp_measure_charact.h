#ifndef BP_MEASURE_CHARACT_H
#define BP_MEASURE_CHARACT_H

#include "charact_base.h"

//#define BLE_GATTS_VLOC_STACK	       0x01  /**< Attribute Value is located in	stack memory, no user memory is	required. */
//#define BLE_GATTS_VLOC_USER	       0x02  /**< Attribute Value is located in	user memory. This requires the user to maintain	a valid	buffer through the lifetime of the attribute, since the	stack

const uint16_t UUID_BLOOD_PRESSURE = 0x2A35;        // blood pressure measurement

const ble_gatts_attr_md_t BLOOD_ATTR_META_DATA = {
	.read_perm = ENCRYPTED_LINK,
	.write_perm = ENCRYPTED_LINK,
	.vlen = 0,        // variable	length
	.vloc = BLE_GATTS_VLOC_USER,
	.rd_auth = 0,        // doesn't need authorization	to read
	.wr_auth = 0         // doesn't need authorization	to write
};

#pragma pack(1)
typedef struct
{
	uint16_t Year;
	uint8_t  Month;
	uint8_t  Day;
	uint8_t  Hours;
	uint8_t  Minutes;
	uint8_t  Seconds;
} time_stamp_t;

typedef struct
{
	struct
	{
		uint8_t UnitsType : 1;               // 0=mmHg, 1=kpa
		uint8_t TimeStampPresent : 1;        // 0=not present, 1=present
		uint8_t PulseRatePresent : 1;        // 0=not present, 1=present
		uint8_t UserIDPresent : 1;           // 0=not present, 1=present
		uint8_t StatusPresent : 1;           // 0=not present, 1=present
		uint8_t Reserved : 3;
	} Flags;
	uint16_t     Systolic;        // in SFLOAT format but it's 16bit wide and it's same as uint16 when it's positive interger and <2048
	uint16_t     Diastolic;
	uint16_t     MAP;
	time_stamp_t TimeStamp;
	uint16_t     PulseRate;        // in SFLOAT format
								   // uint8_t UserID;
	// struct {
	//	uint16_t BodyMovement : 1;		// 0=no movement, 1=yes
	//	uint16_t CuffFitDetect : 1;		// 0=fit properly, 1=loose
	//	uint16_t IrregularPulse : 1;		// 0=no, 1=yes
	//	uint16_t PulseRangeDetection : 2;	// 0=within range, 1=exceed upper limit, 2=less than lower limit
	//	uint16_t PositionDetection : 1;	// 0=proper, 1=improper
	//	uint16_t Reserved : 10;
	// } MeasureStatus;
} bp_measure_t;
#pragma pack()

///**@brief GATT Characteristic	Presentation Format. */
// typedef struct
//{
//   uint8_t	     format;	  /**< Format of the value, see	@ref BLE_GATT_CPF_FORMATS. */
//   int8_t	     exponent;	  /**< Exponent	for integer data types.	*/
//   uint16_t	     unit;	  /**< Unit from Bluetooth Assigned Numbers. */
//   uint8_t	     name_space;  /**< Namespace from Bluetooth	Assigned Numbers, see @ref BLE_GATT_CPF_NAMESPACES. */
//   uint16_t	     desc;	  /**< Namespace description from Bluetooth Assigned Numbers, see @ref BLE_GATT_CPF_NAMESPACES.	*/
// } ble_gatts_char_pf_t;

///**@brief GATT Characteristic	metadata. */
// typedef struct
//{
//   ble_gatt_char_props_t	char_props;		  /**< Characteristic Properties. */
//   ble_gatt_char_ext_props_t	char_ext_props;		  /**< Characteristic Extended Properties. */
//   uint8_t const	       *p_char_user_desc;	  /**< Pointer to a UTF-8 encoded string (non-NULL terminated),	NULL if	the descriptor is not required.	*/
//   uint16_t			char_user_desc_max_size;  /**< The maximum size	in bytes of the	user description descriptor. */
//   uint16_t			char_user_desc_size;	  /**< The size	of the user description, must be smaller or equal to char_user_desc_max_size. */
//   ble_gatts_char_pf_t	const  *p_char_pf;		  /**< Pointer to a presentation format	structure or NULL if the CPF descriptor	is not required. */
//   ble_gatts_attr_md_t	const  *p_user_desc_md;		  /**< Attribute metadata for the User Description descriptor, or NULL for default values. */
//   ble_gatts_attr_md_t	const  *p_cccd_md;		  /**< Attribute metadata for the Client Characteristic	Configuration Descriptor, or NULL for default values. */
//   ble_gatts_attr_md_t	const  *p_sccd_md;		  /**< Attribute metadata for the Server Characteristic	Configuration Descriptor, or NULL for default values. */
// } ble_gatts_char_md_t;

class bp_measure_charact_t : public charact_base_t
{
public:
	bp_measure_charact_t()
		: charact_base_t(UUID_BLOOD_PRESSURE, &RW_INDICATE_CHARACT_META_DATA, &BLOOD_ATTR_META_DATA)
	{
		// character value attributes
		ValueAttribute.init_len = sizeof(bp_measure_t);
		ValueAttribute.init_offs = 0;
		ValueAttribute.max_len = sizeof(bp_measure_t);        // blood pressure measurement length
		ValueAttribute.p_value = (uint8_t*)&BPMeasure;        // by SDK default, characteristic values are stored in user memory(as class member)
	}
	volatile bp_measure_t BPMeasure;
};

#endif
