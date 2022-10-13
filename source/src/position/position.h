#pragma once

#include <stdbool.h>

struct Vec3
{
    float x;
    float y;
    float z;
} __attribute__((packed));

struct Vec3 get_current_position();
struct Vec3 get_next_position();
bool override_position();
