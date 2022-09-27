#include <string.h>
#include "app_channel.h"
#include "debug.h"
#include "controller.h"
#include "../status/status.h"
#define DEBUG_MODULE "CONTROLLER"

void receive_command(struct CommandPacketRX *RX)
{
    if (appchannelReceiveDataPacket(RX, sizeof(*RX), APPCHANNEL_WAIT_FOREVER))
    {
        switch(RX->command_id)
        {
            case 0:
                identify_drone();
                break;
        }
    }
} 
