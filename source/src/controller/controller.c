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
    case 4:
        return Crashed;
    default:
        return Idle;
    }
}

bool checkCrashedState(enum State* state)
{
    bool crashed = supervisorIsTumbled();
    if(crashed))
    {
        *state = Crashed;
    }
    return crashed;
}

void handle_state(struct CommandPacketRX* RX, enum State* state)
{
    vTaskDelay(M2T(10));

    switch (*state)
    {
    case Idle:
    {
        if(checkCrashedState(state)){
            break;
        }
        *state = get_state(RX);
        break;
    }
    case Takeoff:
    {
        if(checkCrashedState(state)){
            break;
        }
        if (start_mission(RX->command_param_value))
        {
            *state = Exploration;
        }
        break;
    }
    case Exploration:
    {
        if(checkCrashedState(state)){
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
        if(checkCrashedState(state)){
            break;
        }
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
        if(checkCrashedState(state)){
            break;
        }
        else
        {
            *state = get_state(RX);
        }
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
