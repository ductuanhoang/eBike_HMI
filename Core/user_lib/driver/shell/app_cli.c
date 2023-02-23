/******************************************************************************
 * @file:    vsm_cli.c
 * @brief:
 * @version: V0.0.0
 * @date:    2019/11/12
 * @author:
 * @email:
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
#include "app_cli.h"
#include "vsm_shell.h"
#include "vsm_retarget.h"
static int32_t cli_reset_system(p_shell_context_t context, int32_t argc, char **argv);
static int32_t test_ble(p_shell_context_t context, int32_t argc, char **argv);
static int32_t sensor_list(p_shell_context_t context, int32_t argc, char **argv);
static int32_t rise_compute(p_shell_context_t context, int32_t argc, char **argv);

static const shell_command_context_t cli_command_table[] = {
    {"reset", "\"reset\": reset system\r\n", cli_reset_system, 0},
    {"test_ble", "\"test_ble\": test bluetooth\r\n", test_ble, 1},
	{"sensor_list", "\"sensor_list\": sensor_list\r\n", sensor_list, 0},
	{"rise_compute", "\"rise_compute\": rise_compute\r\n", rise_compute, 1},
};

shell_context_struct user_context;

int user_put_char(uint8_t *buf, uint32_t len)
{
    return user_putchar((const char *)*buf);
}

int user_get_char(uint8_t *buf, uint32_t len)
{
    uint8_t ch = user_getchar();
    *buf = ch;
    return ch;
}

void vsm_cli_start(void)
{
    SHELL_Init(&user_context,
               user_put_char,
               user_get_char,
               uart_debug_printf,
               ">");

    /* Register CLI commands */
    for (int i = 0; i < sizeof(cli_command_table) / sizeof(shell_command_context_t); i++)
    {
        SHELL_RegisterCommand(&cli_command_table[i]);
    }
}

void vsm_cli_main_loop(void *pvParameters)
{
    SHELL_Process(&user_context);
}

/* Reset System */
static int32_t cli_reset_system(p_shell_context_t context, int32_t argc, char **argv)
{
    return 0;
}


static int32_t test_ble(p_shell_context_t context, int32_t argc, char **argv)
{
    char *param = argv[1];
    uart_ble_printf("%s\r\n", param);
    return 0;
}

static int32_t sensor_list(p_shell_context_t context, int32_t argc, char **argv)
{
	APP_LOGD("ble status = %s", sensor_read.ble_status == 1 ? "CONNECT":"DISCONNECT");
	APP_LOGD("sensor press = %d", sensor_read.fPressure_sender_percent);
	APP_LOGD("sensor vBat charg = %d", sensor_read.fVbat_charging);
	APP_LOGD("sensor vBat lipo= %d", sensor_read.fVbat_lipo);
	APP_LOGD("sensor Knock1 = %d", sensor_read.knock_sensor_channel_1);
	APP_LOGD("sensor Knock2 = %d", sensor_read.knock_sensor_channel_2);
	APP_LOGD("sensor rpm = %d", sensor_read.rpm);
    return 0;
}

static int32_t rise_compute(p_shell_context_t context, int32_t argc, char **argv)
{
	uint16_t data = atoi(argv[1]);
	int32_t value = 0;
	value = oild_pressuare_table(data);
	APP_LOGI("compute = %d", value);
}
