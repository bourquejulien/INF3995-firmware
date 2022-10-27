#include "telemetrics.h"

#include "app_channel.h"


void update_telemetrics_data(int state)
{
    struct Vec3 position;
    get_current_position(&position);

    struct Telemetrics data;
    data.type = 0;
    data.position = position;
    data.state = state;
    
    appchannelSendDataPacket(&data, sizeof(data));
}

void update_telemetrics_map()
{
    struct MapData data;
    float distances [ObstacleDirectionEND];
    get_distances(distances);

    for (int i = 0; i < ObstacleDirectionEND; i++)
    {
        data.distances[i] = distances[i];
    }

    data.type = 1;
    
    appchannelSendDataPacket(&data, sizeof(data));
}
