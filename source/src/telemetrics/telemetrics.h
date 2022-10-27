#pragma once

#include "../position/position.h"
#include "../obstacle_detection/obstacle_detection.h"

struct Telemetrics
{
    uint8_t type;
    uint8_t state;
    struct Vec3 position;
} __attribute__((packed));

struct MapData
{
    uint8_t type;
    float distances[ObstacleDirectionEND];
} __attribute__((packed));

void update_telemetrics_data(int state);
void update_telemetrics_map();
