#pragma once

typedef enum
{
    StatusBlue = 0,
    StatusGreen,
    StatusRed
} StatusColor;

void enable_status();
void disable_status();
void set_color(StatusColor color);
void identify_drone();
