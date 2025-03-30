#include "main.h"


//tim2 is 1MHZ
#define micros() (TIM2->CNT)
#define usDelay(x) micros()=0;while(micros()<x)
#define delay(x) HAL_Delay(x)
#define millis() HAL_GetTick()