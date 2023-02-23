/*
 * MyUart.c
 *
 *  Created on: Sep 22, 2019
 *      Author: ADMIN94
 */
#include "user_driver.h"
#include "vsm_fifo.h"
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* Static variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

#define UART_COM huart2

extern fifo_t com_fifo;
extern uint8_t com_rx_buffer[128];

uint8_t u8RxByte2 = 0;

static void user_com_init(void);

/**
* Khoi tao ham uart.
* @author Tuanhd2
* @param void
* @date 09/22/2019
*/
void user_uart_init(void)
{
	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
//	huart2.Instance = USART2;
//	huart2.Init.BaudRate = 115200;
//	huart2.Init.WordLength = UART_WORDLENGTH_8B;
//	huart2.Init.StopBits = UART_STOPBITS_1;
//	huart2.Init.Parity = UART_PARITY_NONE;
//	huart2.Init.Mode = UART_MODE_TX_RX;
//	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
//	if (HAL_UART_Init(&huart2) != HAL_OK)
//	{
//		Error_Handler();
//	}
	/* USER CODE BEGIN USART1_Init 2 */
	user_com_init();
	// register call back

	/* USER CODE END USART1_Init 2 */
}

static void user_com_init(void)
{
	fifo_create(
		&com_fifo,
		com_rx_buffer,
		sizeof(com_rx_buffer) / sizeof(uint8_t),
		sizeof(uint8_t));
	/* Infinite loop */
	HAL_UART_Receive_IT(&UART_COM, (uint8_t *)&u8RxByte2, 1);
}

/**
* Xoa ham khoi tao uart.
* @author Tuanhd2
* @param void
* @date 09/22/2019
*/
void user_uart_deinit(void)
{
	HAL_UART_DeInit(&UART_COM);
}

bool uart_trasmit(uint8_t *buffer, uint16_t Size)
{
	HAL_StatusTypeDef status;
	status = HAL_UART_Transmit(&UART_COM, (uint8_t *)buffer, Size, 200);

	return (status ? HAL_OK : 1, 0);
}

int uart_ble_printf(const char *Format, ...)
{
	uint8_t buff[512 + 1] = {0};
	va_list args;
	va_start(args, Format);
	uint16_t len = vsprintf((char*)buff, Format, args);
    HAL_UART_Transmit(&UART_COM, (uint8_t *)buff, len, 100);
	va_end(args);
	return -1;
}
