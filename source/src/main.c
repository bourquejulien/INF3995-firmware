#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "app.h"

#define DEBUG_MODULE "MAIN"
#include "debug.h"
#include "status/status.h"
#include "controller/controller.h"


void appMain() {
    struct CommandPacketRX CommandRX;
    enum State State = Idle;

    while(1) {
        if (receive_command(&CommandRX) == 0)
        {
            CommandRX.command_id = -1;
        }

        handle_state(&CommandRX, &State);
    }
}
