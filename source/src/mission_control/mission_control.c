#include "mission_control.h"
#include <string.h>

#include "FreeRTOS.h"
#include "commander.h"
#include "crtp_commander.h"
#include "crtp_commander_high_level.h"
#include "param.h"

#include "debug.h"
#include "task.h"

#include "../obstacle_detection/obstacle_detection.h"
#include "../position/position.h"

static float update_time = 0;
static float default_z = 0;
static float distance_trigger = 0;

static float distance_trigger_z = 0;

static float walk_distance;

const float return_threshold = 0.5f;

bool isGoTo_finished() { return crtpCommanderHighLevelIsTrajectoryFinished(); }

static void reset()
{
    distance_trigger_z = 0;
    walk_distance = 0;
}

bool start_mission(float distance)
{
    if (!distance_trigger || !default_z || !update_time)
    {
        return false;
    }

    walk_distance = distance;
    distance_trigger_z =
        (default_z / 2) > (distance_trigger) ? (distance_trigger) : (default_z / 2);

    start_position(default_z);
    crtpCommanderInit();
    crtpCommanderHighLevelInit();

    crtpCommanderHighLevelTakeoff(default_z, update_time);

    return true;
}

void update_mission()
{
    if (!isGoTo_finished())
    {
        return;
    }

    struct Vec3 position;
    get_next_position(&position, walk_distance, 0);

    DEBUG_PRINT("GOTO: (%f, %f, %f)\n", (double)position.x, (double)position.y, (double)position.z);

    crtpCommanderHighLevelGoTo(position.x, position.y, position.z, 0, update_time, true);
}

// Returns true if the drone is in the process of landing, false if it is still moving towards the
// start
bool return_to_base()
{
    if (!isGoTo_finished())
    {
        return false;
    }

    if (get_distance_from_start() < return_threshold)
    {
        end_mission();
        return true;
    }
    else
    {
        struct Vec3 position;
        get_return_position(&position, walk_distance, 0);

        DEBUG_PRINT(
            "GOTO: (%f, %f, %f)\n", (double)position.x, (double)position.y, (double)position.z);

        crtpCommanderHighLevelGoTo(position.x, position.y, position.z, 0, update_time, true);
        return false;
    }
}

void end_mission()
{
    crtpCommanderHighLevelLand(0, update_time);
    reset();
}

void force_end_mission()
{
    crtpCommanderHighLevelStop();
    reset();
}

PARAM_GROUP_START(app)

PARAM_ADD(PARAM_FLOAT, updateTime, &update_time)
PARAM_ADD(PARAM_FLOAT, defaultZ, &default_z)
PARAM_ADD(PARAM_FLOAT, distanceTrigger, &distance_trigger)

PARAM_GROUP_STOP(app)
