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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "app_cli.h"
#include "vsm_shell.h"
#include "vsm_retarget.h"
#include "../peripheral/timer.h"
#include "../../vsm_app/test_func/test_func.h"

static int32_t cli_reset_system(p_shell_context_t context, int32_t argc, char **argv);

static const shell_command_context_t cli_command_table[] = {
    {"reset", "\"reset\": reset system\r\n", cli_reset_system, 0},
    {"show_data", "\"show_data\": show all data\r\n", show_data_process, 0},
    {"show_defrost", "\"show_defrost\": show state defrost\r\n", show_defrost, 0},
    {"pc_damper", "\"pc_damper\": 1 on pc 0 off pc\r\n", pc_damper, 1},
    {"pf_damper", "\"pf_damper\": 1 on pf 0 off pf\r\n", pf_damper, 1},
    {"damper_show", "\"damper_show\": show status damper\r\n", damper_show, 0},
    {"fc_fan_level", "\"fc_fan_level\": 1 on pf 0 off pf\r\n", fc_fan_level, 1},
    {"speed_fc_fan", "\"speed_fc_fan\": 1 on pf 0 off pf\r\n", speed_fc_fan, 0},
    {"clear_level", "\"clear_level\": clear priority\r\n", clear_level, 0},
    {"adc_value", "\"adc_value\": adc_value read\r\n", adc_value, 0},
    {"heater_value_high", "\"heater_value_high\": heater_value_high \r\n", heater_value_high, 0},
    {"heater_value_low", "\"heater_value_low\": heater_value_low \r\n", heater_value_low, 0},
    {"compress_set_speed", "\"compress_set_speed\": compress_set_speed \r\n", compress_set_speed, 1},
    {"compress_clear", "\"compress_clear\": compress_clear \r\n", compress_clear, 0},
    {"temp_on_off_show", "\"temp_on_off_show\": temp_on_off_show \r\n", temp_on_off_show, 0},
    {"door_sw_show", "\"door_sw_show\": door_sw_show \r\n", door_sw_show, 0},
    {"motorstart", "\"motorstart\": motorstart \r\n", motorstart, 0},
    {"motorstop", "\"motorstop\": motorstop \r\n", motorstop, 0},
    {"clearfault", "\"clearfault\": clearfault \r\n", clearfault, 0},
    {"motorsetspeed", "\"motorsetspeed\": motorsetspeed \r\n", motorsetspeed, 1},
    {"ct_fan_speed", "\"ct_fan_speed\": ct_fan_speed \r\n", ct_fan_speed, 1},
    {"fc_show_sate", "\"fc_show_sate\": fc_show_sate \r\n", fc_show_sate, 0},
    {"pc_show_sate", "\"pc_show_sate\": pc_show_sate \r\n", pc_show_sate, 0},
    {"pfc_show_sate", "\"pfc_show_sate\": pfc_show_sate \r\n", pfc_show_sate, 0},
    {"defrost_time", "\"defrost_time\": defrost_time \r\n", defrost_time, 0},
    {"camera_take_picture", "\"camera_take_picture\": camera_take_picture \r\n", camera_take_picture, 1},
    {"ct_fan_get_speed", "\"ct_fan_get_speed\": ct_fan_get_speed \r\n", ct_fan_get_speed, 0},
    {"user_function", "\"user_function\": user_function \r\n", user_function, 0},
    {"atc_set_temp", "\"atc_set_temp\": atc_set_temp \r\n", atc_set_temp, 1},
    {"set_refri_mode", "\"set_refri_mode\": set_refri_mode from 0 to 6 \r\n", set_refri_mode, 1}};

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
    if (argc < 1)
    {
        printf("Missing parameter\r\n");
        return -1;
    }

    printf("sys reset\r\n");

    return 0;
}
