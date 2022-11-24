#include <math.h>
#include <stdlib.h>

#include "debug.h"
#include "log.h"
#include "param.h"

#include "../obstacle_detection/obstacle_detection.h"
#include "position.h"

static float distance_trigger = 0.5;
static float z_trigger = 0.0;
static float angle = 0.0;

static logVarId_t logIdStateEstimateX;
static logVarId_t logIdStateEstimateY;
static logVarId_t logIdStateEstimateZ;

static float get_x() { return logGetFloat(logIdStateEstimateX); }
static float get_y() { return logGetFloat(logIdStateEstimateY); }
static float get_z() { return logGetFloat(logIdStateEstimateZ); }

static float random_range(float min, float max)
{
    float scale = rand() / (float) RAND_MAX;
    return min + scale * ( max - min );
}

static float get_angle(float* distances)
{
    int wallsClose = 0;
    float x = 0.0;
    float y = 0.0;

    if (distances[FrontDirection] && distances[FrontDirection] <= distance_trigger)
    {
        x -= 1.0f / distances[FrontDirection];
        wallsClose++;
    }
    if (distances[BackDirection] && distances[BackDirection] <= distance_trigger)
    {
        x += 1.0f / distances[BackDirection];
        wallsClose++;
    }
    if (distances[LeftDirection] && distances[LeftDirection] <= distance_trigger)
    {
        y -= 1.0f / distances[LeftDirection];
        wallsClose++;
    }
    if (distances[RightDirection] && distances[RightDirection] <= distance_trigger)
    {
        y += 1.0f / distances[RightDirection];
        wallsClose++;
    }

    
    if (wallsClose == 0) 
    {
        // If no walls are close, i.e. the drone just took off, choose a completely random direction 
        return random_range(0.0, M_PI * 2.0);
    } else 
    {
        // Else, find the angle of the vector above
        float angleRange = M_PI_4;
        float rangeCenter = atan2(y, x);
        return random_range(rangeCenter - angleRange, rangeCenter + angleRange);
    }
}

void init_position()
{
    logIdStateEstimateX = logGetVarId("stateEstimate", "x");
    logIdStateEstimateY = logGetVarId("stateEstimate", "y");
    logIdStateEstimateZ = logGetVarId("stateEstimate", "z");

    srand(get_distance(FrontDirection) + get_distance(BackDirection));
}

void start_position(float z_trigger)
{
    z_trigger = z_trigger;
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
    bool is_triggered = get_triggered_distances(distances, distance_trigger, z_trigger);

    position->x = 0.0;
    position->y = 0.0;
    position->z = 0.0;

    if (is_triggered)
    {
        angle = get_angle(distances);
    }

    position->x = (float)cos(angle) * distance;
    position->y = (float)sin(angle) * distance;
    position->z = position->z * zdistance;

    return false;
}

PARAM_GROUP_START(app)

PARAM_ADD(PARAM_FLOAT, distance_trigger, &distance_trigger)
PARAM_ADD(PARAM_FLOAT, z_trigger, &z_trigger)

PARAM_GROUP_STOP(app)
