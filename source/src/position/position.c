#include <stdlib.h>
#include <math.h>

#include "log.h"

#include "../obstacle_detection/obstacle_detection.h"
#include "position.h"

// #include "stm32f4xx.h"
// #include "stm32f4xx_rng.h"

static float distance_trigger;

static logVarId_t logIdStateEstimateX;
static logVarId_t logIdStateEstimateY;
static logVarId_t logIdStateEstimateZ;

static float get_x() { return logGetFloat(logIdStateEstimateX); }
static float get_y() { return logGetFloat(logIdStateEstimateY); }
static float get_z() { return logGetFloat(logIdStateEstimateZ); }

void init_position(float trigger)
{
    distance_trigger = trigger;
    logIdStateEstimateX = logGetVarId("stateEstimate", "x");
    logIdStateEstimateY = logGetVarId("stateEstimate", "y");
    logIdStateEstimateZ = logGetVarId("stateEstimate", "z");

    srand(get_distance(FrontDirection) + get_distance(BackDirection));
} 

static float get_random()
{
    int random = rand();
    return (2.0 * ((random * 1.0) / RAND_MAX)) - 1.0;
}

static float normalise_distance(float distance)
{
    return 1 - (distance / distance_trigger);
}

static void compute_trggered_position(float* distances, struct Vec3* position)
{
    if (distances[FrontDirection])
    {
        position->x += -normalise_distance(distances[FrontDirection]);
    }
    if (distances[BackDirection])
    {
        position->x += normalise_distance(distances[BackDirection]);
    }
    if (distances[LeftDirection])
    {
        position->y += -normalise_distance(distances[LeftDirection]);
    }
    if (distances[RightDirection])
    {
        position->y += normalise_distance(distances[RightDirection]);
    }
}

void get_current_position(struct Vec3* position)
{
    position->x = get_x();
    position->y = get_y();
    position->z = get_z();
}

bool get_next_position(struct Vec3* position, float distance, float zdistance)
{
    float distances[ObstacleDirectionEND];
    bool is_triggered = get_triggered_distances(distances, distance_trigger);

    float x = get_random();
    float y = get_random();

    if (is_triggered)
    {
        position->x = 0.0;
        position->y = 0.0;
        position->z = 0.0;
        
        compute_trggered_position(distances, position);

        return true;
    }

    float base = sqrt(pow(x, 2) + pow(y, 2));
    x = x / base;
    y = y / base;

    position->x = x * distance;
    position->y = y * distance;
    position->z = zdistance * get_random();

    return false;
}
