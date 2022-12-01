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

#include "log.h"

#define DEBUG_MODULE "CONTROLLER"

static float min_voltage = 3.0; // Voltage representing low battery. Should be 3.77 according to voltage table but this seems too high in practice.
static int battery_counter = 0;

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

    if(supervisorIsTumbled())
    {
        *state = Crashing;
    }

    int next_state = get_state(RX);

    if (next_state == EmergencyStop)
    {
        *state = next_state;
    }

    switch (*state)
    {
    case Idle:
    {
        *state = next_state;
        
        if (low_battery() && next_state != Identify) 
        {
            *state = Idle;
        }
        break;
    }
    case Takeoff:
    {
        if (start_mission(RX->command_param_value))
        {
            battery_counter = 0;
            *state = Exploration;
        }
        break;
    }
    case Exploration:
    {
        if (next_state == Landing || low_battery())
        {
            *state = Landing;
        }

        update_mission();
        break;
    }
    case Landing:
    {
        if (end_mission()) 
        {
            *state = Idle;
        }
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
    case Crashing:
    {
        if(supervisorIsTumbled())
        {
            force_end_mission();
            *state = Crashed;
        }
        else
        {
            // Drone not necessarily idle, but idle state allows the drone to choose the next appropriate state
            *state = Idle;
        }
        break;
    }
    case Crashed:
    {
        if(!supervisorIsTumbled())
        {
            *state = Idle;
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

bool low_battery()
{
    logVarId_t vbatid = logGetVarId("pm", "vbat");
    float vbat = logGetFloat(vbatid);

    if (vbat < min_voltage) 
    {
        battery_counter++;
    }
    else
    {
        battery_counter = 0;
    }

    // Battery level must stay below threshold for 10 consecutive ticks to avoid false positive due to fluctuations. 
    return battery_counter >= 10;
}
