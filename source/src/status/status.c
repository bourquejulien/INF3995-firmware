#include "status.h"
#include "debug.h"
#include "led.h"
#include "ledseq.h"

#include "FreeRTOS.h"
#include "task.h"

#define DEBUG_MODULE "STATUS"

void identify_drone() 
{
    DEBUG_PRINT("Identify\n");
    ledseqEnable(false);
    for (int i = 0; i < 10; i++)
    {
        ledSet(LOWBAT_LED, true);
        vTaskDelay(M2T(250));
        ledSet(LOWBAT_LED, false);
        vTaskDelay(M2T(250));
    }
    ledseqEnable(true);
}
