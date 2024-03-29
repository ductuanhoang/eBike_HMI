/*
 * DB_Measure.h
 *
 *  Created on: Jun 10, 2020
 *      Author: Yolo
 */

#ifndef VSM_APP_DATA_BASE_DB_MEASURE_H_
#define VSM_APP_DATA_BASE_DB_MEASURE_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "../common.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
	
	E_end_off_table_measure
}e_DB_Measure;

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

/***********************************************************************************************************************
* Function Name: DB_Measure_init
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void DB_Measure_init( void );
/***********************************************************************************************************************
* Function Name: DB_Measure_Write
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void DB_Measure_Write(e_DB_Measure id, float value);
/***********************************************************************************************************************
* Function Name: DB_Measure_Read
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

float DB_Measure_Read(e_DB_Measure Id);

#endif /* VSM_APP_DATA_BASE_DB_MEASURE_H_ */
