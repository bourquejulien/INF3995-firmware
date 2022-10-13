#pragma once

#include <stdbool.h>
#include "../position/position.h"

bool isGoTo_finished();
void start_mission(float z, float time);
void update_mission();
void end_mission(float time);
