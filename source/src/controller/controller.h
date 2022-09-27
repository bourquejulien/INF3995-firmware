#pragma once

struct CommandPacketRX {
  int command_id;
} __attribute__((packed));

void receive_command(struct CommandPacketRX *RX);
