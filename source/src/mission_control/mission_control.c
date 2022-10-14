#include "mission_control.h"
#include <string.h>

#include "commander.h"
#include "crtp_commander.h"
#include "crtp_commander_high_level.h"
#include "FreeRTOS.h"
#include "task.h"
#include "debug.h"

#include "../position/position.h"

static float update_time;
static float default_z;

bool isGoTo_finished() { return crtpCommanderHighLevelIsTrajectoryFinished(); }

void start_mission(float z, float time)
{
    update_time = time;
    default_z = z;

    crtpCommanderInit();
    crtpCommanderHighLevelInit();

    crtpCommanderHighLevelTakeoff(z, time);
}

void update_mission()
{
    if (!isGoTo_finished())
    {
        return;
    }

    struct Vec3 position;
    get_next_position(&position, 0.1, 0);

    DEBUG_PRINT("Position change %f, %f, %f\n", (double)position.x, (double)position.y, (double)position.z);

    crtpCommanderHighLevelGoTo(position.x, position.y, position.z, 0, update_time, true);
}

void end_mission(float time) { crtpCommanderHighLevelLand(0, time); }

void force_end_mission() { crtpCommanderHighLevelStop(); }
