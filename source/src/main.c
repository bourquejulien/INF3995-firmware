#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "param.h"

#include "app.h"
#include "cfassert.h"

#include "debug.h"
#include "status/status.h"
#include "controller/controller.h"

#define DEBUG_MODULE "MAIN"

void appMain() {
    struct CommandPacketRX CommandRX;
    enum State State = Idle;

    paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
    // TODO Add multiranger
    // paramVarId_t idMultiranger = paramGetVarId("deck", "bcMultiranger");

    if (!paramGetUint(idPositioningDeck) /* || !paramGetUint(idMultiranger) */)
    {
        ASSERT_FAILED();
    }

    while(1) {
        if (receive_command(&CommandRX) == 0)
        {
            CommandRX.command_id = -1;
        }

        handle_state(&CommandRX, &State);
    }
}
