#include "mission_control.h"

#include "commander.h"
#include "crtp_commander.h"
#include "crtp_commander_high_level.h"

#include "FreeRTOS.h"
#include "task.h"

#include <string.h>
// #include <math.h>
// #include "usec_time.h"

// static setpoint_t setpoint;

// static void setVelocitySetpoint(setpoint_t *setpoint, float vx, float vy, float z, float yawrate)
// {
//   setpoint->mode.z = modeAbs;
//   setpoint->position.z = z;
//   setpoint->mode.yaw = modeVelocity;
//   setpoint->attitudeRate.yaw = yawrate;
//   setpoint->mode.x = modeVelocity;
//   setpoint->mode.y = modeVelocity;
//   setpoint->velocity.x = vx;
//   setpoint->velocity.y = vy;

//   setpoint->velocity_body = true;
// }

static struct Vec3 next_moves[4] = {{0.5, 0.5, 0.0}, {-0.5, -0.5, 0.0}, {0.0, 0.0, -0.1}, {0.0, 0.0, 0.1}};
static int current_move = 0;

bool isGoTo_finished()
{
  return crtpCommanderHighLevelIsTrajectoryFinished();
}

void start_mission(float z, float time)
{
    // float timeNow = usecTimestamp() / 1e6;
    // float cmdAngWDeg = cmdAngWRad * 180.0f / (float)M_PI;
    
    // Turn velocity commands into setpoints and send it to the commander
    crtpCommanderInit();
    crtpCommanderHighLevelInit();
    // setVelocitySetpoint(&setpoint, vx, vy, vz, 0);
    // commanderSetSetpoint(&setpoint, 3);

    crtpCommanderHighLevelTakeoff(z, time);
}

void update_mission()
{
    // float cmdAngWDeg = cmdAngWRad * 180.0f / (float)M_PI;
    
    // Turn velocity commands into setpoints and send it to the commander
    crtpCommanderHighLevelGoTo(next_moves[current_move].x, next_moves[current_move].y, next_moves[current_move].z, 0, 2, true);
    current_move = (current_move + 1) % (sizeof(next_moves)/sizeoff(next_moves[0]));
}

void end_mission(float time)
{
    // setpoint_t setpoint;
    // // float cmdAngWDeg = cmdAngWRad * 180.0f / (float)M_PI;

    // memset(&setpoint, 0, sizeof(setpoint_t));
    // commanderSetSetpoint(&setpoint, 3);
    crtpCommanderHighLevelLand(0, time);
    // crtpCommanderHighLevelStop();
    
    // Turn velocity commands into setpoints and send it to the commander
    // setVelocitySetpoint(&setpoint, 0, 0, 0, 0);
    // commanderSetSetpoint(&setpoint, 3);
}