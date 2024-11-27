#ifndef __MIRCO_DELAY__H__
#define __MIRCO_DELAY__H__
#include <stm32f10x.h>
void mirco_delay_init(void);
void mirco_delay_us(uint32_t us);
void mirco_delay_ms(uint32_t ms);
void mirco_delay_s(uint32_t s);
#endif  //!__MIRCO_DELAY__H__