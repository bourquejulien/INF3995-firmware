#include "mission_control.h"

#include "commander.h"
#include "crtp_commander.h"
#include "crtp_commander_high_level.h"

#include "FreeRTOS.h"
#include "task.h"

#include <string.h>

static struct Vec3 next_moves[4] = {
    {0.5, 0.5, 0.0}, {-0.5, -0.5, 0.0}, {0.0, 0.0, -0.1}, {0.0, 0.0, 0.1}};

static int current_move = 0;
static const int move_count = sizeof(next_moves) / sizeof(next_moves[0]);

bool isGoTo_finished() { return crtpCommanderHighLevelIsTrajectoryFinished(); }

void start_mission(float z, float time)
{
    crtpCommanderInit();
    crtpCommanderHighLevelInit();

    crtpCommanderHighLevelTakeoff(z, time);
}

void update_mission()
{
    crtpCommanderHighLevelGoTo(
        next_moves[current_move].x, next_moves[current_move].y,
        next_moves[current_move].z, 0, 2, true);
    current_move = (current_move + 1) % move_count;
}

void end_mission(float time) { crtpCommanderHighLevelLand(0, time); }