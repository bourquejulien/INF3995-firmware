#pragma once

typedef enum
{
    StatusBlue = 5,
    StatusGreen = 3,
    StatusRed = 4
} StatusColor;

void enable_status();
void disable_status();
void set_color(StatusColor color);
void identify_drone();
