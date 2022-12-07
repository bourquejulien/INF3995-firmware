#include "synchronization.h"
#include "../position/position.h"
#include "../status/status.h"

#include <float.h>
#include <stdbool.h>
#include <string.h>

#include "configblock.h"
#include "debug.h"
#include "param.h"
#include "radiolink.h"
#include "usec_time.h"

#define LIST_SIZE 10

typedef struct
{
    int8_t id;
    float distance;
    float received_time;
} DistanceData;

#define SIZE_OFF_PACKET sizeof(DistanceData)

static bool is_enabled;
static float sync_update_time;

static float last_clock;
static uint8_t color;
static DistanceData drones_data[LIST_SIZE];

static void data_handler(P2PPacket* packet)
{
    static DistanceData data;
    memcpy(&data, &packet->data[0], SIZE_OFF_PACKET);

    for (int i = 0; i < LIST_SIZE; ++i)
    {
        if (drones_data[i].id == data.id)
        {
            drones_data[i].distance = data.distance;
            drones_data[i].received_time = usecTimestamp();
            return;
        }
    }

    float oldest_timestamp = FLT_MAX;
    int target_index = 0;

    for (int i = 0; i < LIST_SIZE; ++i)
    {
        if (drones_data[i].received_time < oldest_timestamp)
        {
            oldest_timestamp = drones_data[i].received_time;
            target_index = i;
        }
    }

    drones_data[target_index].id = data.id;
    drones_data[target_index].distance = data.distance;
    drones_data[target_index].received_time = usecTimestamp();
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
        set_color(StatusBlue);
    }
    else
    {
        disable_status();
    }
}

static void render_color()
{
    static uint8_t step;
    step = (step + 1) % LIST_SIZE;

    if (step >= color)
    {
        set_color(StatusGreen);
        return;
    }

    set_color(StatusRed);
}

static void update_color()
{
    int connected_drone_count = 0;
    int closer_drones_count = 0;
    float current_distance = get_distance_from_start();

    for (int i = 0; i < LIST_SIZE; ++i)
    {
        if (drones_data[i].id >= 0)
        {
            ++connected_drone_count;
            if (drones_data[i].distance < current_distance)
            {
                ++closer_drones_count;
            }
        }
    }

    color = (uint8_t)LIST_SIZE * ((closer_drones_count * 1.0) / ((connected_drone_count)*1.0));
}

static void send_data()
{
    static DistanceData data;
    data.distance = get_distance_from_start();
    data.id = get_id();

    static P2PPacket packet;
    packet.port = 0;
    packet.size = SIZE_OFF_PACKET;

    memcpy(&(packet.data), &data, packet.size);
    radiolinkSendP2PPacketBroadcast(&packet);
}

void init_synchronization()
{
    p2pRegisterCB(data_handler);

    is_enabled = false;
    color = 0.0;
    sync_update_time = 500;

    last_clock = usecTimestamp();

    for (int i = 0; i < LIST_SIZE; ++i)
    {
        drones_data[i].id = -1;
        drones_data[i].distance = 0;
        drones_data[i].received_time = 0.0;
    }
}

void synchronize_drones()
{
    uint64_t time_since_update_ms = ((usecTimestamp() - last_clock) / 1000);
    if (time_since_update_ms > sync_update_time)
    {
        last_clock = usecTimestamp();

        if (is_enabled)
        {
            update_color();
            send_data();
        }
    }

    if (is_enabled)
    {
        render_color();
    }
}

PARAM_GROUP_START(app)
PARAM_ADD_WITH_CALLBACK(PARAM_1BYTE, sync_enabled, &is_enabled, set_status)
PARAM_ADD(PARAM_FLOAT, sync_update, &sync_update_time)
PARAM_GROUP_STOP(app)
