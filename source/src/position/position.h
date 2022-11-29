#pragma once

#include <stdbool.h>

struct Vec3
{
    float x;
    float y;
    float z;
} __attribute__((packed));

void init_position();
void start_position(float z_trigger);
void get_current_position(struct Vec3* position);
float get_distance_from_start();
bool get_next_position(struct Vec3* position, float distance, float zdistance);
bool get_return_position(struct Vec3* position, float distance, float zdistance);