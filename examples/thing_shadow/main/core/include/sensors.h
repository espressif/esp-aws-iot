
#ifndef SENSORS_H
#define SENSORS_H

#include "driver/temp_sensor.h"

#define FLOAT_STRING_SIZE 10

void sensors_initialize();
char* get_board_temp();

#endif /* SENSORS_H */