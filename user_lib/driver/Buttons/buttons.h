/**
 * @file buttons.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _VSM_HARDBUTTON_H_
#define _VSM_HARDBUTTON_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define APP_LOGE(...)

#define MAX_BTN_SUPPORT (16u)
#define PERIOD_SCAN_IN_MS (200)
#define BUTTON_HOLD_TIME (2000)
#define BUTTON_ON_HOLD_TIME_FIRE_EVENT (200)
#define IDLE_TIME_COUNT_IN_MS (30000)
#define BUTTON_DOUBLE_CLICK_TIME (500)
#define BUTTON_PRESS_DEBOUND_TIME (50)

/**
 * Enum to define the event type of hard button
 */
typedef enum
{
	E_EVENT_HARD_BUTTON_PRESS = 0,
	E_EVENT_HARD_BUTTON_RELEASE,
	E_EVENT_HARD_BUTTON_HOLD,
	E_EVENT_HARD_BUTTON_ON_HOLD,
	E_EVENT_HARD_BUTTON_DOUBLE_CLICK,
	E_EVENT_HARD_BUTTON_TRIPLE_CLICK,
	E_EVENT_HARD_ILDE,
	E_EVENT_HARD_ILDE_BREAK,
	E_EVENT_HARD_MAX
} eHardButtonEventType;

typedef uint8_t (*p_btnReadValue)(uint8_t);

typedef struct
{
	uint8_t u8BtnLastState;
	uint32_t u32IdleLevel;
	uint8_t button_type; // hardware button = 1, ADC = 0, One level button = 2
	uint8_t debound_en;	 // Debound enable;
	p_btnReadValue btnReadCb;
} tsButtonConfig;

/**
 * Event callback function type
 * button idx, event type, custom data
 */
typedef void (*pHardButtonEventHandler)(int, int, void *);

void vHardButtonInit(tsButtonConfig *params, uint8_t u8BtnCount);

void vHardButtonTask(void *params);

void vHardButtonSetCallback(eHardButtonEventType event, pHardButtonEventHandler cb, void *data);

void vHardButtonSetGetTickCallback(uint32_t (*gettickCb)(void));
#endif
