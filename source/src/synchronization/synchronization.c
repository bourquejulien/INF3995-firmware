#include "synchronization.h"
#include "../status/status.h"
#include "../position/position.h"

#include <string.h>
#include <float.h>
#include <stdbool.h>

#include "debug.h"
#include "param.h"
#include "radiolink.h"
#include "configblock.h"
#include "usec_time.h"

typedef struct
{
    uint8_t id;
    float distance;
} DistanceData;

static bool is_enabled;
static float sync_update_time;

static float last_clock;
static DistanceData min_data;
static DistanceData max_data;

static void data_handler(P2PPacket *p)
{
    static DistanceData data;
    memcpy(&data, &p->data[0], sizeof(DistanceData));

    if (data.distance < min_data.distance || data.id == min_data.id)
    {
        min_data = data;
    }

    if (data.distance > max_data.distance || data.id == max_data.id)
    {
        max_data = data;
    }

    float current_distance = get_distance_from_start();

    if (current_distance < min_data.distance)
    {
        set_color(StatusGreen);
        return;
    }

    if (current_distance > max_data.distance)
    {
        set_color(StatusRed);
        return;
    }

    set_color(StatusBlue);
}

static uint8_t get_id()
{
    uint64_t address = configblockGetRadioAddress();
    return (uint8_t)(address & 0xff);
}

static void set_status()
{
    if (is_enabled)
    {
        enable_status();
    }
    else
    {
        disable_status();
    }
}

static void synchronize()
{
    if (!is_enabled)
    {
        return;
    }

    static DistanceData data;
    data.distance = get_distance_from_start();
    data.id = get_id();

    static P2PPacket packet;
    packet.port = 0;
    packet.size = sizeof(DistanceData);

    memcpy(&packet.data, &data, packet.size);
    radiolinkSendP2PPacketBroadcast(&packet);
}

void init_synchronization()
{
    p2pRegisterCB(data_handler);

    is_enabled = false;
    sync_update_time = 2000;

    last_clock = usecTimestamp();

    min_data.id = 0;
    min_data.distance = FLT_MAX;
    max_data.id = 0;
    max_data.distance = 0;
}

void synchronize_drones()
{
    uint64_t time_since_update_ms = ((usecTimestamp() - last_clock) / 1000);
    if(time_since_update_ms > sync_update_time)
    {
        last_clock = usecTimestamp();
        synchronize();
    }
}

PARAM_GROUP_START(app)
PARAM_ADD_WITH_CALLBACK(PARAM_1BYTE, sync_enabled, &is_enabled, set_status)
PARAM_ADD(PARAM_FLOAT, sync_update, &sync_update_time)
PARAM_GROUP_STOP(app)
