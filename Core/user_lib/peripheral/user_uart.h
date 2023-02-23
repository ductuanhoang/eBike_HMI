/*
 * MyUart.h
 *
 *  Created on: Sep 22, 2019
 *      Author: ADMIN94
 */

#ifndef MYUART_H_
#define MYUART_H_
#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"
//#include "stm32f4xx_hal.h"

#define MAX_UART_RECEIVE 100

/**
* Khoi tao ham uart.
* @author Tuanhd2
* @param void
* @date 09/22/2019
*/
void user_uart_init(void);
/**
* Khoi tao ham uart.
* @author Tuanhd2
* @param void
* @date 03/23/2021
*/
void user_uart_deinit(void);

bool uart_trasmit(uint8_t *buffer, uint16_t Size);

#endif /* MYUART_H_ */
