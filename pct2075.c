/**
  ******************************************************************************
  * @file		: pct2075.c
  * @author		: Bunyamin TAMAR - bunyamintamar@yandex.com
  * @brief		: HAL driver for PCT2075 temperature sensor from NXP
  *				  January 2021
  * NOTE		: Use your device specific I2C commands (400kHz)
  *				  in PCT2075_Write(..) and PCT2075_Read(..) functions 
  ******************************************************************************
  * HOW TO USE THIS DRIVER?
  *
  * Define a PCT2075_t struct
  * 		-> Id			: Device ID
  *			-> Hyst			: Hysteresis value
  *			-> OS			: Overtemperature shutdownn threshold value
  *			-> Idle			: Time out value for I2C communication
  *			-> OS_F_QUE		: OS register fault queue
  *			-> OS_POL		: OS polarity
  *			-> OS_OP_MODE	: OS operation mode
  *			-> OP_MODE		: Operation mode
  *
  * Use only these two functions
  *			-> PCT2075_Config(....) : for configuration the device
  *			-> PCT2075_Read(....)	: for reading the temperature value
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pct2075.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	TEMP_Reg 			= (0x00),	/*!< Address of the temperature register 				*/
	CONF_Reg 			= (0x01),	/*!< Address of the configuration register 				*/
	HYST_Reg 			= (0x02),	/*!< Address of the hysteresis register 				*/
	OS_Reg   			= (0x03),	/*!< Address of the overtemperature shutdown register	*/
	IDLE_Reg 			= (0x04)	/*!< Address of the idle register 						*/
}Reg_Addr_t;

/* Private define ------------------------------------------------------------*/
#define IS_PCT2075_ID(ID)		(	(	PACK_8_PIN_ADDR_1 	== (ID)		)	|| 	\
									(	PACK_8_PIN_ADDR_2 	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_3 	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_4 	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_5 	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_6 	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_7 	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_8 	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_9 	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_10	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_11	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_12	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_13	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_14	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_15	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_16	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_17	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_18	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_19	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_20	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_21	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_22	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_23	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_24	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_25	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_26	== (ID)		)	||	\
									(   PACK_8_PIN_ADDR_27	== (ID)		)	||	\
									(   PACK_6_PIN_ADDR_1 	== (ID)		)	||	\
									(   PACK_6_PIN_ADDR_2 	== (ID)		)	||	\
									(   PACK_6_PIN_ADDR_3	== (ID)		)	)

#define IS_OS_F_QUE(F_QUE)		(	(	OS_F_QUE_00			== (F_QUE)	)	|| 	\
									(	OS_F_QUE_01			== (F_QUE)	)	||	\
									(   OS_F_QUE_10			== (F_QUE)	)	||	\
									(   OS_F_QUE_11			== (F_QUE)	)	)
															   
#define IS_OS_POL(POL)			(	(	OS_POL_L			== (POL)	)	|| 	\
									(	OS_POL_H			== (POL)	)	)
															   
#define IS_OS_OP_MODE(MODE)		(	(	OS_OP_MODE_COMP		== (MODE)	)	|| 	\
									(	OS_OP_MODE_INT		== (MODE)	)	)
															   
#define IS_OP_MODE(MODE)		(	(	OP_MODE_NORMAL		== (MODE)	)	|| 	\
									(	OP_MODE_SHUTDOWN	== (MODE)	)	)
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/**
 *	@brief	: Set configuration register value
 *	@param	: *device		- device structure
 *	@return	: Regulated value
 */
static uint8_t Set_Conf_Reg_Value(PCT2075_t *device)
{
	return	(	
				(		(device->OP_MODE			)	/*!< B0 	*/
					|	(device->OS_OP_MODE	<< 1	)	/*!< B1 	*/
					|	(device->OS_POL		<< 2	)	/*!< B2 	*/
					|	(device->OS_F_QUE	<< 3	)	/*!< B[4:3]	*/
				)
				&& (0b00011111)							/*!< Masked	*/
			);
}

/**
 *	@brief	: Set hysteresis register value
 *	@param	: *device		- device structure
 *	@return	: Regulated value
 */
static uint16_t Set_Hyst_Reg_Value(PCT2075_t *device)
{
	return (uint16_t)(device->Hyst << 7);
}

/**
 *	@brief	: Set overtemperature shutdown register value
 *	@param	: *device		- device structure
 *	@return	: Regulated value
 */
static uint16_t Set_OS_Reg_Value(PCT2075_t *device)
{
	return (uint16_t)(device->OS << 7);
}

/**
 *	@brief	: Set idle register value
 *	@param	: *device		- device structure
 *	@return	: Regulated value
 */
static uint8_t Set_Idle_Value(PCT2075_t *device)
{
	return (uint8_t)(device->Idle && (0b00011111));
}

/**
 *	@brief	: Write register value via I2C
 *	@param	: id		- id of the device
 *	@param	: reg		- register of the device
 *	@param	: data		- value of the register
 *	@return	: none
 *	NOTE	: Use your device specific I2C commands
 */
static void PCT2075_Write(PCT2075_Id_t id, Reg_Addr_t reg, uint8_t data)
{
	// TODO:	Write here your device specific I2C commands
}

/* Exported functions --------------------------------------------------------*/

/**
 *	@brief	: Configure the device
 *	@param	: *device		- device structure
 *	@return	: none
 */
void PCT2075_Config(PCT2075_t *device)
{
	assert(IS_PCT2075_ID(device->Id));
	assert(IS_OP_MODE(device->OP_MODE));
	assert(IS_OS_OP_MODE(device->OS_OP_MODE));
	assert(IS_OS_POL(device->OS_POL));
	assert(IS_OS_F_QUE(device->OS_F_QUE));	
	
	uint8_t		conf_value;
	uint16_t	hyst_value;
	uint16_t	os_value;
	uint8_t		idle_value;
	
	conf_value	= Set_Conf_Reg_Value(device);	/*!< Set regulated configuration register value				*/
	hyst_value	= Set_Hyst_Reg_Value(device);	/*!< Set regulated hysteresis register value				*/
	os_value	= Set_OS_Reg_Value(device);		/*!< Set regulated overtemperature shutdown register value	*/
	idle_value	= Set_Idle_Value(device);		/*!< Set regulated idle register value						*/
	
	/* Set configuration register value */
	PCT2075_Write(device->Id, CONF_Reg, conf_value);
	
	/* Set hysteresis register value */
	PCT2075_Write(device->Id, HYST_Reg, (uint8_t)(hyst_value>>8));	/*!< MSB */
	PCT2075_Write(device->Id, HYST_Reg, (uint8_t)(hyst_value));		/*!< LSB */
	
	/* Set overtemperature shutdown register value */
	PCT2075_Write(device->Id, OS_Reg, (uint8_t)(os_value>>8));		/*!< MSB */
	PCT2075_Write(device->Id, OS_Reg, (uint8_t)(os_value));			/*!< LSB */
	
	/* Set idle register value */
	PCT2075_Write(device->Id, IDLE_Reg, idle_value);
}

/**
 *	@brief	: Read temperature value via I2C
 *	@param	: *device		- device structure
 *	@return	: signed temperature value
 *	NOTE	: Use your device specific I2C commands
 */
int8_t PCT2075_Read(PCT2075_t *device)
{
	float temporary;
	uint8_t i2c_result[2] = {0,0};
	
	/**
	 *	Use these parameters:
	 *		device->Id		: id of the device
	 *		TEMP_Reg		: address of the temperature register
	 *		device->Idle	: time out for I2C communication
	 *		i2c_result[i]	: I2C read data
	 *						  i2c_result[0] - MSByte of the temperature value
	 *						  i2c_result[1] - LSByte of the temperature value
	 */
	 
	 // TODO:	Write here your device specific I2C commands
	
	temporary = (((i2c_result[0] << 8) + i2c_result[1] ) >> 5) * (0.125f);
	return (int8_t)temporary;
}



