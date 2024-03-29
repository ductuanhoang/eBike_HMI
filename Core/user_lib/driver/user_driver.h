/**
 * @file user_driver.h
 * @author tuanhd (ductuanhoang820@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef USER_DRIVER_H_
#define USER_DRIVER_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "common.h"
#include "app_cli.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

/**
 * @brief initialzation driver
 *
 */
void userDriverInitial(void);

/**
 * @brief process all driver
 * Note: this function is called in loop thread
 */
void userDriverProcess(void);
#endif /* USER_DRIVER_H_ */
