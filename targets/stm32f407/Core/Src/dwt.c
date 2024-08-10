/**
 * @file dwt.c
 * @author Viacheslav (v.konovalov@crystals.ru)
 * @brief Dwt timer and us delay with it
 * @version 0.1
 * @date 2022-12-29
 *
 * @copyright CSI Copyright (c) 2022
 *
 */
#include "dwt.h"
#include "stm32f4xx.h"

/**
 * @brief dwt counter init
 *
 */
void dwt_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // counter enable
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // run counter
}

/**
 * @brief us delay
 *
 * @param us
 */
void dwt_delay_us(uint32_t us)
{
    const uint32_t us_count_tic = us * (SystemCoreClock / 1000000U);
    dwt_reset();
    while (dwt_read() < us_count_tic)
    {
    };
}

/**
 * @brief dwt reset counter
 *
 * @return uint32_t
 */
void dwt_reset(void)
{
    DWT->CYCCNT = 0U;
}

/**
 * dwt read counter
 * sys_mult = (SystemCoreClock / 1000000U) = 96
 * us = ticks / sys_mult
 * ms = (ticks / sys_mult) / 1000U
 * return core ticks
 */
uint32_t dwt_read(void)
{
    return DWT->CYCCNT;
}
