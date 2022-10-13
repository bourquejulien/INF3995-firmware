#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "param.h"

#include "app.h"
#include "cfassert.h"

#include "controller/controller.h"
#include "debug.h"
#include "status/status.h"
#include "position/position.h"

#define DEBUG_MODULE "MAIN"

#define TRIGGER_DISTANCE 0.3

void appMain()
{
    struct CommandPacketRX CommandRX;
    enum State State = Idle;

    paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
    paramVarId_t idMultiranger = paramGetVarId("deck", "bcMultiranger");

    if (!paramGetUint(idPositioningDeck) || !paramGetUint(idMultiranger))
    {
        ASSERT_FAILED();
    }

    init_position(TRIGGER_DISTANCE);

    while (1)
    {
        if (receive_command(&CommandRX) == 0)
        {
            CommandRX.command_id = -1;
        }

        handle_state(&CommandRX, &State);
    }
}
