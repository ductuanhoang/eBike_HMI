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
#include "stdint.h"
#include "Config_SCI9_Debug.h"
#include "vsm_fifo.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/********************** Internal Function Prototypes *************************/

/************************** Variable Definitions *****************************/
fifo_t shell_fifo;
uint8_t shell_rx_buffer[128] = {0};
uint8_t u8RxByte = 0;

// sci9 debug
volatile bool sci9_rx_finish_flag = false;
volatile bool sci9_tx_finish_flag = false;


/********************* Exported Function Definitions *************************/

void HAL_UART_RxCpltCallback(void)
{
	R_Config_SCI9_Debug_Serial_Receive( &u8RxByte, 1);
	fifo_add(&shell_fifo, &u8RxByte);
}

/**
 * @brief      Initialize FIFO
 *
 * @return     { description_of_the_return_value }
 */
int retarget_init_fifo(void) {
  fifo_create(
                    &shell_fifo,
                    shell_rx_buffer,
                    sizeof(shell_rx_buffer)/sizeof(uint8_t),
                    sizeof(uint8_t)
                  );

  R_Config_SCI9_Debug_Start();
  R_Config_SCI9_Debug_Serial_Receive( &u8RxByte, 1 );
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
//	uint32_t timeout = 0;
	uint8_t buff[128 + 1] = {0};
	va_list args;
	va_start(args, Format);
	uint16_t len = vsprintf((char*)buff, Format, args);
	sci9_tx_finish_flag = false;
	R_Config_SCI9_Debug_Serial_Send( (uint8_t*)buff,  len);
	while(sci9_tx_finish_flag == false);
	va_end(args);
	return -1;
}


long user_putchar(const char * str)
{
  sci9_tx_finish_flag = false;
  R_Config_SCI9_Debug_Serial_Send( (uint8_t *)str,  strlen(str));
  while(sci9_tx_finish_flag == false);
  return 0;
}


long user_getchar (void)
{
  int ch = 0;
  return (serial_get_char((uint8_t*)&ch) == 1) ? ch : 0;
}



/********************* Internal Function Definitions *************************/

/*****************************************************************************/
