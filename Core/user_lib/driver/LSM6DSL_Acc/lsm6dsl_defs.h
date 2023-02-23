/**
 * @file lsm6dsl_defs.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef LSM6DSL_DEFS_H_
#define LSM6DSL_DEFS_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/** Error code definitions */
#define LSM6DSL_OK INT8_C(0)
/* Errors */
#define LSM6DSL_E_NULL_PTR INT8_C(-1)
#define LSM6DSL_E_COM_FAIL INT8_C(-2)
#define LSM6DSL_E_DEV_NOT_FOUND INT8_C(-3)
#define LSM6DSL_E_INVALID_LENGTH INT8_C(-4)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/** Type definitions */
/*!
 * Generic communication function pointer
 * @param[in] dev_id: Place holder to store the id of the device structure
 *                    Can be used to store the index of the Chip select or
 *                    I2C address of the device.
 * @param[in] reg_addr:	Used to select the register the where data needs to
 *                      be read from or written to.
 * @param[in/out] reg_data: Data array to read/write
 * @param[in] len: Length of the data array
 */
typedef int8_t (*lsm6dsl_com_fptr_t)(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);

/*!
 * Delay function pointer
 * @param[in] period: Time period in milliseconds
 */
typedef void (*lsm6dsl_delay_fptr_t)(uint32_t period);

typedef void (*lsm6dsl_gpio_fptr_t)(void);

typedef struct
{
    /*! Chip ID*/
    uint8_t chip_id;
    /*! Device ID*/
    uint8_t device_id;
    /*! Bus read function pointer */
    lsm6dsl_com_fptr_t read;
    /*! Bus write function pointer */
    lsm6dsl_com_fptr_t write;
    /*! Bus write function pointer */
    lsm6dsl_com_fptr_t writeMultiple;
    /*! delay function pointer */
    lsm6dsl_delay_fptr_t delay_ms;

} LSM6DSL_dev_t;

extern void SENSOR_IO_Init(void);
extern void SENSOR_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
extern uint8_t SENSOR_IO_Read(uint8_t Addr, uint8_t Reg);
extern uint16_t SENSOR_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);

extern void SENSOR_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

#endif /* LSM6DSL_DEFS_H_ */
