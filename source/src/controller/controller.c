#include "controller.h"
#include "../mission_control/mission_control.h"
#include "../status/status.h"
#include "../telemetrics/telemetrics.h"

#include "app_channel.h"
#include "debug.h"
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "supervisor.h"

#define DEBUG_MODULE "CONTROLLER"

int receive_command(struct CommandPacketRX* RX)
{
    return appchannelReceiveDataPacket(RX, sizeof(*RX), 0);
}

static enum State get_state(struct CommandPacketRX* RX)
{
    switch (RX->command_id)
    {
    case 0:
        return Identify;
    case 1:
        return Takeoff;
    case 2:
        return Landing;
    case 3:
        return EmergencyStop;
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
    {
        *state = get_state(RX);
        break;
    }
    case Takeoff:
    {
        if (start_mission(RX->command_param_value))
        {
            *state = Exploration;
        }
        break;
    }
    case Exploration:
    {
        if (supervisorIsTumbled())
        {
            *state = Crashed;
            break;
        }

        int next_state = get_state(RX);
        if (next_state == Landing || next_state == EmergencyStop)
        {
            *state = next_state;
        }
        update_mission();
        break;
    }
    case Landing:
    {
        end_mission(2);
        *state = Idle;
        break;
    }
    case EmergencyStop:
    {
        force_end_mission();
        *state = Idle;
        break;
    }
    case Identify:
    {
        identify_drone();
        *state = Idle;
        break;
    }
    case Crashed:
    {
        force_end_mission();

        // qqchose
        // attendre nouvel état, si nouvel état arrive, deset état crashed
        break;
    }
    default:
    {
        *state = Idle;
        break;
    }
    }
}

void update_status(enum State* state)
{
    update_telemetrics_data(*state);
    update_telemetrics_map();
}
