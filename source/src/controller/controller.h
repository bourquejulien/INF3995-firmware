#pragma once

struct CommandPacketRX
{
    int command_id;
    char command_param_name[8];
    float command_param_value;
} __attribute__((packed));

enum State
{
    Idle,
    Identify,
    Takeoff,
    Exploration,
    Landing
};

int receive_command(struct CommandPacketRX* RX);
enum State handle_command(struct CommandPacketRX* RX);
void handle_state(struct CommandPacketRX* RX, enum State* state);
