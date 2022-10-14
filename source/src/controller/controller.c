#include "controller.h"
#include "../mission_control/mission_control.h"
#include "../status/status.h"
#include "app_channel.h"
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#define DEBUG_MODULE "CONTROLLER"

int receive_command(struct CommandPacketRX* RX)
{
    DEBUG_PRINT("Command of size (%d) received\n", sizeof(*RX));
    return appchannelReceiveDataPacket(RX, sizeof(*RX), 0);
}

enum State handle_command(struct CommandPacketRX* RX)
{
    switch (RX->command_id)
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
    vTaskDelay(M2T(10));

    switch (*state)
    {
    case Idle:
        *state = handle_command(RX);
        break;

    case Takeoff:
        start_mission(RX->command_param_value, 2);
        *state = Exploration;
        break;

    case Exploration:
        if (handle_command(RX) == Landing)
        {
            *state = Landing;
        }

        update_mission();
        break;

    case Landing:
        end_mission(2);
        *state = Idle;
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
