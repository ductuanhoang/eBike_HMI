/*
 *
 *  Created on: Sep 22, 2019
 *      Author: ADMIN94
 */

#ifndef COMMON_H_
#define COMMON_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

 #include "stm32f4xx_hal.h"
#include "../driver/shell/vsm_retarget.h"
/* Console color */
#define kRESET "\x1B[0m"
#define kKRED "\x1B[31m"
#define kKGRN "\x1B[32m"
#define kKYEL "\x1B[33m"
#define kKBLU "\x1B[34m"
#define kKMAG "\x1B[35m"
#define kKCYN "\x1B[36m"
#define KWHT "\x1B[37m"

//#define APP_DEBUG printf

//#define VSM_DEBUG_COLOR

enum
{
	E_LOG_LVL_NONE,
	E_LOG_LVL_ERROR,
	E_LOG_LVL_INFO,
	E_LOG_LVL_DEBUG,
	E_LOG_LVL_NEVER
};

#ifdef VSM_DEBUG_COLOR
	#define LOG_SHOULD_I( level ) ( level <= LOG_BUILD_LEVEL && level <= E_LOG_LVL_DEBUG)
	#define LOG(level, tag,...) do {	\
		if ( LOG_SHOULD_I(level) ) { \
			uart_debug_printf("[%s] %s:%d: "RESET , tag, __func__,__LINE__);\
			uart_debug_printf(__VA_ARGS__); \
			uart_debug_printf("\r\n");\
		} \
	} while(0)

	#define APP_LOGE(...)  LOG(E_LOG_LVL_ERROR, KRED"ERROR"RESET, __VA_ARGS__)
	#define APP_LOGI(...)  LOG(E_LOG_LVL_INFO, KGRN"INFOR"RESET, __VA_ARGS__)
	#define APP_LOGD(...)  LOG(E_LOG_LVL_DEBUG, KYEL"DEBUG"RESET, __VA_ARGS__)
	#define APP_LOGW(...)  LOG(E_LOG_LVL_WARNING, BG_KOLORS_YEL"ALARM"RESET, __VA_ARGS__)
#else
	#define LOG_SHOULD_I( level ) ( level <= LOG_BUILD_LEVEL && level <= E_LOG_LVL_DEBUG)
	#define LOG(level, tag,...) do {	\
		if ( LOG_SHOULD_I(level) ) { \
			uart_debug_printf("[%s] %s:%d: " , tag, __func__,__LINE__);\
			uart_debug_printf(__VA_ARGS__); \
			uart_debug_printf("\r\n");\
		} \
	} while(0)

	#define APP_LOGE(...) 	LOG(E_LOG_LVL_ERROR, "E", __VA_ARGS__)
	#define APP_LOGI(...) 	LOG(E_LOG_LVL_ERROR, "I", __VA_ARGS__)
	#define APP_LOGD(...)  	LOG(E_LOG_LVL_ERROR, "D", __VA_ARGS__)
	#define APP_LOGW(...)  	LOG(E_LOG_LVL_ERROR, "A", __VA_ARGS__)

#endif
#define LOG_BUILD_LEVEL E_LOG_LVL_DEBUG
//#define LOG_BUILD_LEVEL			E_LOG_LVL_NONE
typedef enum
{
	E_BLE_DISCONNECT,
	E_BLE_CONNECTED
}e_ble_status;

typedef struct
{
	uint16_t fVbat_lipo; //multiplier 10
	uint16_t fVbat_charging; //multiplier 10
	uint16_t fPressure_sender_percent; // bar 0-10 bar multiplier 10
	uint32_t rpm;
	uint16_t knock_sensor_channel_1; //multiplier 10
	uint16_t knock_sensor_channel_2; //multiplier 10
	e_ble_status ble_status;
} sensor_read_t;

typedef enum
{
	E_VBAT_LIPO,
	E_VBAT_CHARGING,
	E_PRESSURE_SENDER,
	E_KNOCK_SENSOR,
	E_END_OF_ENUM,
} e_number_sensor;



extern sensor_read_t sensor_read;

extern uint32_t timer_get_tick(void);
#ifdef __cplusplus
}
#endif
#endif /* COMMON_H_ */
