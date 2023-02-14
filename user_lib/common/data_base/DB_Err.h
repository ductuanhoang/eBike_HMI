/*
 * DB_Err.h
 *
 *  Created on: Jun 10, 2020
 *      Author: Yolo
 */

#ifndef VSM_APP_DATA_BASE_DB_ERR_H_
#define VSM_APP_DATA_BASE_DB_ERR_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "common.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
	// motor err
	ERR_MOTOR_OVERTEMPERATURE = 0,
	ERR_MOTOR_EMERGENCY,
	ERR_MOTOR_OVERVOLTAGE,
	ERR_MOTOR_NOMOTOR,
	ERR_MOTOR_SWUNDERVOLTAGE,
	ERR_MOTOR_SWOVERVOLTAGE,
	ERR_MOTOR_SWOVERCURRENT,
	ERR_MOTOR_WRONGDIRECTION,
	ERR_MOTOR_SPEEDREDUCTION,
	ERR_MOTOR_STALLDETECTED,
	ERR_MOTOR_LOST_CONNECT,
	// LTE Module
	ERR_LTE_MODULE_LOST_CONNECT,
	// BlueTooth Module

	// sensor humidity err
	ERR_HUMI_HVC,
	ERR_HUMI_HAV ,
	// fan err
	ERR_FAN_CT,
	ERR_FAN_FC,
	// cảnh báo mở cửa quá lâu
	ERR_LED_FC_OPEN,
	E_end_off_table_err
} e_DB_Err;



/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

void DB_Err_init( void );

void DB_Err_Write( e_DB_Err id, uint8_t value);

uint8_t DB_Err_Read( e_DB_Err id);

uint16_t DB_Err_Motor_Read(void);

#endif /* VSM_APP_DATA_BASE_DB_ERR_H_ */
