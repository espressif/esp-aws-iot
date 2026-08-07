#ifndef LDO_CONTROL_H
#define LDO_CONTROL_H

#include "driver/gpio.h"

#define LDO_CONTROL_GPIO GPIO_NUM_10

void ldo_init();
void ldo_on();
void ldo_off();

#endif /* LDO_CONTROL_H */
