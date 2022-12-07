#include "telemetrics.h"

#include "app_channel.h"

void update_telemetrics_data(int state)
{
    struct Telemetrics data;

    data.type = 0;
    data.state = state;
    get_current_position(&data.position);
    data.battery_level = get_battery();

    appchannelSendDataPacket(&data, sizeof(data));
}

void update_telemetrics_map()
{
    float distances[ObstacleDirectionEND];
    get_distances(distances);

    struct MapData data;

    data.type = 1;

    for (int i = 0; i < UpDirection; i++)
    {
        data.distances[i] = distances[i];
    }

    get_current_position(&data.position);
    appchannelSendDataPacket(&data, sizeof(data));
}
