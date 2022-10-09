#include <string.h>
#include "app_channel.h"
#include "debug.h"
#include "controller.h"
#include "../status/status.h"
#include "../mission_control/mission_control.h"

#include "FreeRTOS.h"
#include "task.h"

#define DEBUG_MODULE "CONTROLLER"

int receive_command(struct CommandPacketRX *RX)
{
    return appchannelReceiveDataPacket(RX, sizeof(*RX), 0);
}

enum State handle_command(struct CommandPacketRX* RX)
{
    switch(RX->command_id)
    {
        case 0:
            return Identify;
        case 1:
            return Takeoff;
        case 2:
            return Landing;
        default:
            return Idle;
    }
}

void handle_state(struct CommandPacketRX* RX, enum State* state)
{
    switch (*state)
    {
    case Idle:
        if(RX->command_id > -1)
        {
            *state = handle_command(RX);
        }
        vTaskDelay(M2T(1));
        break;
    
    case Takeoff:
        
        break;

    case Landing:
        break;

    case Identify:
        identify_drone();
        *state = Idle;
        break;

    default:
        *state = Idle;
        break;
    }
}