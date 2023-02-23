
/**
 * @file buttons.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "buttons.h"
static uint8_t u8ButtonData[MAX_BTN_SUPPORT] = {0};
static uint32_t u32ButtonPressEvent;
static uint32_t u32ButtonReleaseEvent;
static uint32_t u32ButtonHoldEvent = 0;
static uint32_t u32ButtonHoldEventExec = 0;
static uint32_t u32ButtonOnHoldEvent = 0;
static uint8_t u8ButtonCount;
static uint32_t pu32ButtonHoldTimeCount[MAX_BTN_SUPPORT] = {0};
static uint32_t pu32ButtonOnHoldTimeCount[MAX_BTN_SUPPORT] = {0};
static uint32_t pu32ButtonLastPressTime[MAX_BTN_SUPPORT] = {0};
static uint8_t u8ButtonPressCount[MAX_BTN_SUPPORT] = {0};
static tsButtonConfig *hwParams;
static pHardButtonEventHandler tsCallbackTable[E_EVENT_HARD_MAX] = {0};
static void *pvCustomData[E_EVENT_HARD_MAX] = {0};
static uint32_t (*gettick)(void);

static uint32_t u32ButtonHasHoldEvent = 0;

void vHardButtonInit(tsButtonConfig *params, uint8_t u8BtnCount)
{
    uint8_t i = 0;

    if (NULL == params)
    {
        APP_LOGE("Invalid init pointer");
        return;
    }

    if (MAX_BTN_SUPPORT < u8BtnCount)
    {
        APP_LOGE("Not support %d buttons", u8BtnCount);
        return;
    }

    hwParams = params;

    u8ButtonCount = u8BtnCount;

    for (i = 0; i < u8BtnCount; i++)
    {
        /* Set Idle state */
        hwParams[i].u8BtnLastState = hwParams[i].u32IdleLevel;
        u8ButtonData[i] = hwParams[i].u32IdleLevel;
    }
}

void vHardButtonSetGetTickCallback(uint32_t (*gettickCb)(void))
{
    gettick = gettickCb;
}

void vHardButtonTask(void *params)
{
    uint8_t i = 0;

    if (NULL == hwParams)
    {
        APP_LOGE("Invalid init pointer");
        return;
    }

    if (MAX_BTN_SUPPORT < u8ButtonCount)
    {
        APP_LOGE("Not support %d buttons", u8ButtonCount);
        return;
    }

    // if (u8ButtonStateUpdateFlag)
    // {
    //     u8ButtonStateUpdateFlag = 0;

    for (i = 0; i < u8ButtonCount; i++)
    {
        if (hwParams[i].btnReadCb == NULL)
        {
            continue;
        }
        u8ButtonData[i] = hwParams[i].btnReadCb(i);

        // APP_DEBUG("button status: %d", u8ButtonData[i]);
        if (u8ButtonData[i] ^ hwParams[i].u8BtnLastState)
        {
            if (u8ButtonData[i] != hwParams[i].u32IdleLevel)
            {
                if (hwParams[i].debound_en == 0)
                {
                    u32ButtonPressEvent |= 1 << i;
                }
                pu32ButtonHoldTimeCount[i] = gettick();
            }
            else
            {
                if (hwParams[i].debound_en == 1)
                {
                    uint32_t execTime = gettick() - pu32ButtonHoldTimeCount[i];
                    if (execTime > BUTTON_PRESS_DEBOUND_TIME)
                    {
                        u32ButtonReleaseEvent |= 1 << i;
                    }
                }
                else
                {
                    u32ButtonReleaseEvent |= 1 << i;
                }

                pu32ButtonHoldTimeCount[i] = 0;
                pu32ButtonOnHoldTimeCount[i] = 0;
            }
        }
        else
        {
            if (hwParams[i].debound_en == 1)
            {
                if (u8ButtonData[i] != hwParams[i].u32IdleLevel)
                {
                    uint32_t execTime = gettick() - pu32ButtonHoldTimeCount[i];
                    if (execTime > BUTTON_PRESS_DEBOUND_TIME)
                    {
                        u32ButtonPressEvent |= 1 << i;
                    }
                }
            }
        }

        hwParams[i].u8BtnLastState = u8ButtonData[i];
    }
    // }

    for (i = 0; i < u8ButtonCount; i++)
    {
        // if(hwParams[i].button_type == 2) // one level button, ingore hold and onhold event
        // {
        //     continue;
        // }
        if (u8ButtonData[i] != hwParams[i].u32IdleLevel)
        {
            if (gettick() - pu32ButtonHoldTimeCount[i] >= BUTTON_HOLD_TIME)
            {
                if (((u32ButtonHoldEvent >> i) & 0x1))
                {
                    if (gettick() - pu32ButtonOnHoldTimeCount[i] >= BUTTON_ON_HOLD_TIME_FIRE_EVENT)
                    {
                        pu32ButtonOnHoldTimeCount[i] = gettick();
                        u32ButtonOnHoldEvent |= (1 << i);
                        u32ButtonHasHoldEvent |= (1 << i);
                    }
                }
                else
                {
                    pu32ButtonOnHoldTimeCount[i] = gettick();
                    u32ButtonHoldEvent |= (1 << i);
                    u32ButtonHoldEventExec &= ~(1 << i); // clear executed flag
                }
            }
        }

        if ((u32ButtonPressEvent >> i) & 0x01)
        {
            // VSM_DEBUG_INFO("Hard button press event, button idx [%d]", i);
            if (pu32ButtonLastPressTime[i] == 0)
            {
                pu32ButtonLastPressTime[i] = gettick();
            }

            if (gettick() - pu32ButtonLastPressTime[i] >= BUTTON_DOUBLE_CLICK_TIME)
            {
                u8ButtonPressCount[i] = 0;
            }
            else
            {
                u8ButtonPressCount[i] += 1;

                uint32_t u32Event = E_EVENT_HARD_MAX;

                if (u8ButtonPressCount[i] == 1) // double click
                {
                    u32Event = E_EVENT_HARD_BUTTON_DOUBLE_CLICK;
                }
                else if (u8ButtonPressCount[i] == 2)
                {
                    u32Event = E_EVENT_HARD_BUTTON_TRIPLE_CLICK;
                    u8ButtonPressCount[i] = 0;
                }

                if (u32Event != E_EVENT_HARD_MAX)
                {
                    // VSM_DEBUG_INFO("Hard button event[%d], button idx [%d]", u32Event, i);
                    if (NULL != tsCallbackTable[u32Event])
                    {
                        tsCallbackTable[u32Event](i, u32Event, pvCustomData[u32Event]);
                    }
                }
            }

            pu32ButtonLastPressTime[i] = gettick();

            if (NULL != tsCallbackTable[E_EVENT_HARD_BUTTON_PRESS])
            {
                tsCallbackTable[E_EVENT_HARD_BUTTON_PRESS](i, E_EVENT_HARD_BUTTON_PRESS, pvCustomData[E_EVENT_HARD_BUTTON_PRESS]);
            }
            u32ButtonPressEvent &= ~(1 << i);
        }
        if ((u32ButtonReleaseEvent >> i) & 0x01)
        {
            // APP_DEBUG("Hard button release event, button idx [%d]", i);
            if (NULL != tsCallbackTable[E_EVENT_HARD_BUTTON_RELEASE])
            {
                tsCallbackTable[E_EVENT_HARD_BUTTON_RELEASE](i, E_EVENT_HARD_BUTTON_RELEASE, (void *)u32ButtonHasHoldEvent);
            }
            u32ButtonReleaseEvent &= ~(1 << i);
            u32ButtonHoldEvent &= ~(1 << i); // clear
            u32ButtonHasHoldEvent &= !(1 << i);
        }

        if (!((u32ButtonHoldEventExec >> i) & 0x1) && ((u32ButtonHoldEvent >> i) & 0x1))
        {
            u32ButtonHoldEventExec |= (1 << i);
            //            APP_LOGD("E_EVENT_HARD_BUTTON_HOLD exec event %d %d", i, E_EVENT_HARD_BUTTON_HOLD);
            uint32_t u32HoldTime = gettick() - pu32ButtonHoldTimeCount[i];
            if (NULL != tsCallbackTable[E_EVENT_HARD_BUTTON_HOLD])
            {
                tsCallbackTable[E_EVENT_HARD_BUTTON_HOLD](i, E_EVENT_HARD_BUTTON_HOLD, (void *)u32HoldTime);
            }
        }

        if ((u32ButtonOnHoldEvent >> i) & 0x1)
        {
            uint32_t u32HoldTime = gettick() - pu32ButtonHoldTimeCount[i];
            //            APP_LOGD(" E_EVENT_HARD_BUTTON_ON_HOLD exec event %d %d", i, E_EVENT_HARD_BUTTON_ON_HOLD);
            if (NULL != tsCallbackTable[E_EVENT_HARD_BUTTON_ON_HOLD])
            {
                tsCallbackTable[E_EVENT_HARD_BUTTON_ON_HOLD](i, E_EVENT_HARD_BUTTON_ON_HOLD, (void *)u32HoldTime);
            }
            u32ButtonOnHoldEvent &= ~(1 << i);
        }
    }
}

void vHardButtonSetCallback(eHardButtonEventType event, pHardButtonEventHandler cb, void *data)
{
    if (NULL != cb)
    {
        tsCallbackTable[event] = cb;
        pvCustomData[event] = data;
    }
}
