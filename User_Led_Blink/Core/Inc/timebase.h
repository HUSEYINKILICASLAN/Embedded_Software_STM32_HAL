#ifndef TIMEBASE_H
#define TIMEBASE_H
#include "main.h"
#include <stdbool.h>

uint32_t millis(void);
bool elapsed(uint32_t *t0, uint32_t period);

#endif
