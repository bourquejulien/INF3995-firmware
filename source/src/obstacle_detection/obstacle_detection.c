#include "obstacle_detection.h"
#include <range.h>

float get_distance(ObstacleDirection direction)
{
    return rangeGet(direction);
}

void get_distances(float distances[]) 
{
    for (int direction = FrontDirection; direction <= DownDirection; ++direction)
    {
        distances[direction] = get_distance(direction);
    }
}

bool get_triggered_distances(float distances[], float trigger_distance)
{
    bool is_triggered = false;
    for (int direction = FrontDirection; direction <= DownDirection; ++direction)
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

    return is_triggered;
}
