#include "ldo_control.h"
#include "driver/gpio.h"


#define LDO_CONTROL_GPIO GPIO_NUM_10

void ldo_init()
{
    // Initialization code remains the same
    gpio_pad_select_gpio(LDO_CONTROL_GPIO);
    gpio_set_direction(LDO_CONTROL_GPIO, GPIO_MODE_OUTPUT);
}

void ldo_on()
{
    gpio_set_level(LDO_CONTROL_GPIO, 1);
}


void ldo_off()
{
    gpio_set_level(LDO_CONTROL_GPIO, 0);
}
