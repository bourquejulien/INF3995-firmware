#pragma once


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
void get_next_position(struct Vec3* position, float distance, float zdistance);
void get_return_position(struct Vec3* position, float distance, float zdistance);