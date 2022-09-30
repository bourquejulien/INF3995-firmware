#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "app.h"

#define DEBUG_MODULE "MAIN"
#include "debug.h"
#include "controller/controller.h"

void appMain() {
    struct CommandPacketRX CommandRX;

    while(1) {
        receive_command(&CommandRX);
    }
}
