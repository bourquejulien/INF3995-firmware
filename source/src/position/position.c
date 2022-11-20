#include <math.h>
#include <stdlib.h>

#include "debug.h"
#include "log.h"
#include "param.h"

#include "../obstacle_detection/obstacle_detection.h"
#include "position.h"

static float distance_trigger = 0.5;
static float z_trigger = 0.0;

static logVarId_t logIdStateEstimateX;
static logVarId_t logIdStateEstimateY;
static logVarId_t logIdStateEstimateZ;

static float get_x() { return logGetFloat(logIdStateEstimateX); }
static float get_y() { return logGetFloat(logIdStateEstimateY); }
static float get_z() { return logGetFloat(logIdStateEstimateZ); }

static struct Vec3 initial_position;

static float get_random()
{
    // TODO Improve logic
    int random = rand();
    return (2.0 * ((random * 1.0) / RAND_MAX)) - 1.0;
}

static float normalise_distance(float distance, float trigger) { return 1 - (distance / trigger); }

static void compute_triggered_position(float* distances, struct Vec3* position)
{
    if (distances[FrontDirection])
    {
        position->x += -normalise_distance(distances[FrontDirection], distance_trigger);
    }
    if (distances[BackDirection])
    {
        position->x += normalise_distance(distances[BackDirection], distance_trigger);
    }
    if (distances[LeftDirection])
    {
        position->y += -normalise_distance(distances[LeftDirection], distance_trigger);
    }
    if (distances[RightDirection])
    {
        position->y += normalise_distance(distances[RightDirection], distance_trigger);
    }
    if (distances[UpDirection])
    {
        position->z += -normalise_distance(distances[RightDirection], distance_trigger);
    }
    if (distances[DownDirection])
    {
        position->z += normalise_distance(distances[RightDirection], z_trigger);
    }
}

void init_position()
{
    logIdStateEstimateX = logGetVarId("stateEstimate", "x");
    logIdStateEstimateY = logGetVarId("stateEstimate", "y");
    logIdStateEstimateZ = logGetVarId("stateEstimate", "z");

    srand(get_distance(FrontDirection) + get_distance(BackDirection));
    get_current_position(&initial_position);
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

float get_distance_from_start()
{
    static struct Vec3 position;
    get_current_position(&position);

    position.x = position.x - initial_position.x;
    position.y = position.y - initial_position.y;
    position.z = position.z - initial_position.z;

    return sqrt(pow(position.x, 2) + pow(position.y, 2) + pow(position.z, 2));
}

bool get_next_position(struct Vec3* position, float distance, float zdistance)
{
    float distances[ObstacleDirectionEND];
    bool is_triggered = get_triggered_distances(distances, distance_trigger, z_trigger);

    if (is_triggered)
    {
        position->x = 0.0;
        position->y = 0.0;
        position->z = 0.0;

        compute_triggered_position(distances, position);

        DEBUG_PRINT(
            "Triggered Position: (%f, %f, %f)\n", (double)position->x, (double)position->y,
            (double)position->z);
    }
    else
    {
        position->x = get_random();
        position->y = get_random();
        position->z = get_random();
    }

    float base = sqrt(pow(position->x, 2) + pow(position->y, 2));
    float x = position->x / base;
    float y = position->y / base;

    position->x = x * distance;
    position->y = y * distance;
    position->z = position->z * zdistance;

    return false;
}

PARAM_GROUP_START(app)

PARAM_ADD(PARAM_FLOAT, distance_trigger, &distance_trigger)
PARAM_ADD(PARAM_FLOAT, z_trigger, &z_trigger)

PARAM_GROUP_STOP(app)
