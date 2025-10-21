#include "timebase.h"

uint32_t millis(void){ return HAL_GetTick(); }

bool elapsed(uint32_t *t0, uint32_t period){
  if ((int32_t)(millis() - *t0) >= (int32_t)period){
      *t0 += period;
      return true;
  }
  return false;
}
