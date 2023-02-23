/** @file:    vsm_retarget.c
 *  @details:  Retarget function for UART
 *
 * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS IS". VINSMART
 * JSC MAKES NO OTHER WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR,
 * STATUTORY AND DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * SATISFACTORY QUALITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. VINSMART JSC
 * RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER
 * REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR
 * THE PRODUCTS OR TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME.
 *
 * VINSMART JSC SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE SOURCE CODE OR
 * ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO, LOST REVENUES, DATA OR
 * PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL OR CONSEQUENTIAL NATURE, PUNITIVE
 * DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS ARISING OUT OF OR IN CONNECTION
 * WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR
 * PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
 * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.
 *
 * (C)Copyright VINSMART JSC 2019 All rights reserved
 ******************************************************************************/

/***************************** Include Files *********************************/

#include "vsm_retarget.h"
#include "vsm_fifo.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/
static UART_HandleTypeDef* uart = NULL;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
// for uart ble
extern fifo_t com_fifo;
extern uint8_t u8RxByte2;
/***************** Macros (Inline Functions) Definitions *********************/

/********************** Internal Function Prototypes *************************/

/************************** Variable Definitions *****************************/
fifo_t shell_fifo;
uint8_t shell_rx_buffer[512] = {0};
uint8_t cli_rx_byte = 0;
static int retarget_init_fifo(void);
/********************* Exported Function Definitions *************************/

/*! ----------------------------------------------------------------------------
 * @fn port_stdio_init
 * @brief Initialize stdio on the given UART
 *
 * @param[in] huart Pointer to the STM32 HAL UART peripheral instance
 */
void retarget_shell(UART_HandleTypeDef* huart) {
    uart = huart;
    retarget_init_fifo();
}

/**
 * @brief      Initialize FIFO
 *
 * @return     { description_of_the_return_value }
 */
static int retarget_init_fifo(void) {
  fifo_create(
                    &shell_fifo,
                    shell_rx_buffer,
                    sizeof(shell_rx_buffer)/sizeof(uint8_t),
                    sizeof(uint8_t)
                  );
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&cli_rx_byte, 1);
  return 0;
}

/**
 * @brief      Serial get char
 *
 * @param      c     { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
int serial_get_char(uint8_t *c) {
  if(fifo_is_empty(&shell_fifo) == true) return 0;

  uint8_t item = 0;

  if(fifo_get(&shell_fifo, &item) == true) {
    *c = item;
    return 1;
  }

  return 0;
}


/***********************************************************************************************************************
* Function Name:
* Description  : send buffer data
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

//debug printf
int uart_debug_printf(const char *Format, ...)
{
	uint8_t buff[512 + 1] = {0};
	va_list args;
	va_start(args, Format);
	uint16_t len = vsprintf((char*)buff, Format, args);
    HAL_UART_Transmit(uart, (uint8_t *)buff, len, 400);
	va_end(args);
	return -1;
}


long user_putchar(const char * str)
{
    HAL_UART_Transmit(uart, (uint8_t *)str, 1, 100);
    return 0;
}


long user_getchar (void)
{
  int ch = 0;
  return (serial_get_char((uint8_t*)&ch) == 1) ? ch : 0;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
    {
      fifo_add(&shell_fifo, &cli_rx_byte);
      HAL_UART_Receive_IT(uart, &cli_rx_byte, 1);
    }
	else if (huart == &huart2)
	{
		fifo_add(&com_fifo, &u8RxByte2);
		HAL_UART_Receive_IT(&huart2, &u8RxByte2, 1);
	}
}
/********************* Internal Function Definitions *************************/

/*****************************************************************************/
