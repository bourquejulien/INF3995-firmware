#include "status.h"
#include "debug.h"
#include "led.h"
#include "ledseq.h"

#include "FreeRTOS.h"
#include "task.h"

#define DEBUG_MODULE "STATUS"

void enable_status()
{
    ledseqEnable(false);
    ledClearAll();
}

void disable_status()
{
    ledClearAll();
    ledseqEnable(true);
}

void set_color(StatusColor color)
{
    ledSet(color, true);
}

void identify_drone()
{
    DEBUG_PRINT("Starting identify routine\n");
    enable_status();
    for (int i = 0; i < 10; i++)
    {
        ledSet(LED_RED_L, true);
        vTaskDelay(M2T(250));
        ledSet(LED_RED_L, false);
        vTaskDelay(M2T(250));
    }
    disable_status();
    DEBUG_PRINT("Identify routine successful\n");
}
