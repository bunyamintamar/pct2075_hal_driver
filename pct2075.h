/**
  ******************************************************************************
  * @file		: pct2075.h
  * @author		: Bunyamin TAMAR - bunyamintamar@yandex.com
  * @brief		: HAL driver for PCT2075 temperature sensor from NXP
  *				  January 2021
  ******************************************************************************
  */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef enum
{
	PACK_8_PIN_ADDR_1 	= (0b10010000),		/*!< 0x90 */
	PACK_8_PIN_ADDR_2 	= (0b10010010),		/*!< 0x92 */
	PACK_8_PIN_ADDR_3 	= (0b10010100),		/*!< 0x94 */
	PACK_8_PIN_ADDR_4 	= (0b10010110),		/*!< 0x96 */
	PACK_8_PIN_ADDR_5 	= (0b10011000),		/*!< 0x98 */
	PACK_8_PIN_ADDR_6 	= (0b10011010),		/*!< 0x9A */
	PACK_8_PIN_ADDR_7 	= (0b10011100),		/*!< 0x9C */
	PACK_8_PIN_ADDR_8 	= (0b10011110),		/*!< 0x9E */
	PACK_8_PIN_ADDR_9 	= (0b10100000),		/*!< 0xA0 */
	PACK_8_PIN_ADDR_10	= (0b10100010),		/*!< 0xA2 */
	PACK_8_PIN_ADDR_11	= (0b10100100),		/*!< 0xA4 */
	PACK_8_PIN_ADDR_12	= (0b10100110),		/*!< 0xA6 */
	PACK_8_PIN_ADDR_13	= (0b10101000),		/*!< 0xA8 */
	PACK_8_PIN_ADDR_14	= (0b10101010),		/*!< 0xAA */
	PACK_8_PIN_ADDR_15	= (0b10101100),		/*!< 0xAC */
	PACK_8_PIN_ADDR_16	= (0b10101110),		/*!< 0xAE */
	PACK_8_PIN_ADDR_17	= (0b01010000),		/*!< 0x50 */
	PACK_8_PIN_ADDR_18	= (0b01010010),		/*!< 0x52 */
	PACK_8_PIN_ADDR_19	= (0b01010100),		/*!< 0x54 */
	PACK_8_PIN_ADDR_20	= (0b01010110),		/*!< 0x56 */
	PACK_8_PIN_ADDR_21	= (0b01011000),		/*!< 0x58 */
	PACK_8_PIN_ADDR_22	= (0b01011010),		/*!< 0x5A */
	PACK_8_PIN_ADDR_23	= (0b01011100),		/*!< 0x5C */
	PACK_8_PIN_ADDR_24	= (0b01011110),		/*!< 0x5E */
	PACK_8_PIN_ADDR_25	= (0b01101010),		/*!< 0x6A */
	PACK_8_PIN_ADDR_26	= (0b01101100),		/*!< 0x6C */
	PACK_8_PIN_ADDR_27	= (0b01101110),		/*!< 0x6E */
	PACK_6_PIN_ADDR_1 	= (0b10010000),		/*!< 0x90 */
	PACK_6_PIN_ADDR_2 	= (0b10010010),		/*!< 0x92 */
	PACK_6_PIN_ADDR_3	= (0b10010100)		/*!< 0x94 */
}PCT2075_Id_t;

typedef enum
{
	OS_F_QUE_00	     	= (0x00),
	OS_F_QUE_01	     	= (0x01),
	OS_F_QUE_10	     	= (0x10),
	OS_F_QUE_11	     	= (0x11)
}Os_F_Que_t;

typedef enum
{
	OS_POL_L	     	= (0x0),
	OS_POL_H	     	= (0x1)
}Os_Pol_t;

typedef enum
{
	OS_OP_MODE_COMP	 	= (0x0),
	OS_OP_MODE_INT	 	= (0x1)	
}Os_Op_Mode_t;

typedef enum
{
	OP_MODE_NORMAL	 	= (0x0),
	OP_MODE_SHUTDOWN 	= (0x1)
}Op_Mode_t;

typedef struct
{
	PCT2075_Id_t		Id;
	uint16_t 			Hyst;
	uint16_t 			OS;
	uint16_t 			Idle;
	Os_F_Que_t			OS_F_QUE;
	Os_Pol_t			OS_POL;
	Os_Op_Mode_t		OS_OP_MODE;
	Op_Mode_t			OP_MODE;
}PCT2075_t;

void PCT2075_Config(PCT2075_t *device);
int8_t PCT2075_Read(PCT2075_t *device);
