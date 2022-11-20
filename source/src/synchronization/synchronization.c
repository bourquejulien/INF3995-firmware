#include "synchronization.h"
#include "../status/status.h"
#include "../position/position.h"

#include <float.h>

#include "debug.h"
#include "radiolink.h"
#include "configblock.h"

typedef struct
{
    uint8_t id;
    float distance;
} DistanceData;

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

void init_synchronization()
{
    p2pRegisterCB(data_handler);

    min_data.id = 0;
    min_data.distance = FLT_MAX;
    max_data.id = 0;
    max_data.distance = 0;
}

void enable_synchronization()
{
    enable_status();
}

void disable_synchronization()
{
    disable_status();
}

void synchronize()
{
    static DistanceData data;
    data.distance = get_distance_from_start();
    data.id = get_id();

    static P2PPacket packet;
    packet.port = 0;
    packet.size = sizeof(DistanceData);

    memcpy(&packet.data, &data, packet.size);
    radiolinkSendP2PPacketBroadcast(&packet);
}
