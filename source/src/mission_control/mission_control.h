#pragma once

#include <stdbool.h>

struct Vec3 {
  float x;
  float y;
  float z;
} __attribute__((packed));

bool isGoTo_finished();
void start_mission(float z, float time);
void update_mission();
void end_mission(float time);
