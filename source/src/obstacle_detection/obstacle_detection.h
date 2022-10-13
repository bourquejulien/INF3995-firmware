#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    FrontDirection=0,
    BackDirection,
    LeftDirection,
    RightDirection,
    DownDirection,
    ObstacleDirectionEND
} ObstacleDirection;

float get_distance(ObstacleDirection direction);
void get_distances(float distances[]);
bool get_triggered_distances(float distances[], float trigger_distance);
