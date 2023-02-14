/*
 * DB_Control.h
 *
 *  Created on: Jun 10, 2020
 *      Author: Yolo
 */

#ifndef VSM_APP_DATA_BASE_DB_CONTROL_H_
#define VSM_APP_DATA_BASE_DB_CONTROL_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "vsm_common.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
	PRIORITY_LEVEL_0,
	PRIORITY_LEVEL_1,
	PRIORITY_LEVEL_2,
	PRIORITY_LEVEL_3,
	PRIORITY_LEVEL_4,
	PRIORITY_LEVEL_5,
	PRIORITY_LEVEL_6,
	PRIORITY_LEVEL_7,
	PRIORITY_LEVEL_8,
	PRIORITY_LEVEL_9,
	PRIORITY_LEVEL_10,
	PRIORITY_LEVEL_11,
	PRIORITY_LEVEL_12,
	PRIORITY_LEVEL_13,
	PRIORITY_LEVEL_14,
	PRIORITY_LEVEL_MAX,
	E_end_off_table_priority
} ePriority;

typedef enum
{
	E_CTR_OFF,
	E_CTR_ON
} e_ctrl_on_off;

typedef enum
{
	E_CTR_MODE_MIN,
	E_CTR_MODE_MED,
	E_CTR_MODE_MAX,
	// user mode for fc and pc not include pf
	E_CTR_MODE_USER_1,
	E_CTR_MODE_USER_2,
	E_CTR_MODE_USER_3,
	E_CTR_MODE_USER_4,
	E_CTR_MODE_USER_5,
	E_CTR_MODE_USER_6,
	E_CTR_MODE_USER_7,
	E_CTR_MODE_USER_8,
	E_CTR_MODE_USER_9,
} eCtrlMode;

typedef enum
{
	CTR_MOTOR_SPEED = 0,
	CTR_BRAKE,
	CTR_LEDS_PHASE_COS,
	CTR_BUZZER_ALARM,
	
	E_end_off_table_control
} e_DB_Control;

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

/***********************************************************************************************************************
 * Function Name: DB_Ctr_init
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void DB_Control_init(void);
/***********************************************************************************************************************
 * Function Name: DB_Ctr_Write
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void DB_Control_Write(e_DB_Control id, uint16_t value, uint8_t priority);

uint16_t DB_Control_Read(e_DB_Control id);

/***********************************************************************************************************************
 * Function Name: DB_Control_Clear_Priority
 * Description  : set priority = 0
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void DB_Control_Clear_Priority(e_DB_Control id, uint8_t priority);

#endif /* VSM_APP_DATA_BASE_DB_CONTROL_H_ */
