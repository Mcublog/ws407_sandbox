
#ifndef DWT_TIMER_H
#define DWT_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void dwt_init(void);
void dwt_reset(void);
uint32_t dwt_read(void);

void dwt_delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif // DWT_TIMER_H