#include "obstacle_detection.h"
#include <range.h>

#include "debug.h"

float get_distance(ObstacleDirection direction) { return rangeGet(direction) / 1000; }

void get_distances(float distances[])
{
    for (int direction = FrontDirection; direction < ObstacleDirectionEND; ++direction)
    {
        distances[direction] = get_distance(direction);
    }
}

bool get_triggered_distances(float distances[], float trigger_distance, float z_trigger)
{
    bool is_triggered = false;

    for (int direction = FrontDirection; direction <= UpDirection; ++direction)
    {
        float distance = get_distance(direction);

        if (distance > trigger_distance)
        {
            distances[direction] = 0;
            continue;
        }

        distances[direction] = distance;
        is_triggered = true;
    }

    float down_distance = get_distance(DownDirection);

    if (down_distance <= z_trigger)
    {
        distances[DownDirection] = down_distance;
        return true;
    }

    distances[DownDirection] = 0;

    return is_triggered;
}
