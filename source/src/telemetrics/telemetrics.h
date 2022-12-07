#pragma once

#include "../obstacle_detection/obstacle_detection.h"
#include "../position/position.h"
#include "../controller/controller.h"

struct Telemetrics
{
    uint8_t type;
    uint8_t state;
    struct Vec3 position;
    float battery_level;
} __attribute__((packed));

struct MapData
{
    uint8_t type;
    float distances[UpDirection];
    struct Vec3 position;
} __attribute__((packed));

void update_telemetrics_data(int state);
void update_telemetrics_map();
