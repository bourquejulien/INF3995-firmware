#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "param.h"

#include "app.h"
#include "cfassert.h"
#include "debug.h"

#include "controller/controller.h"
#include "position/position.h"
#include "status/status.h"

#define DEBUG_MODULE "MAIN"

// TODO Add as a config or a fraction of the random walk step
#define TRIGGER_DISTANCE 0.4

void appMain()
{
    struct CommandPacketRX CommandRX;
    enum State State = Idle;

    paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
    paramVarId_t idMultiranger = paramGetVarId("deck", "bcMultiranger");

    if (!paramGetUint(idPositioningDeck) || !paramGetUint(idMultiranger))
    {
        DEBUG_PRINT("Failed to initialise INF3995 module, check the decks!\n");
        ASSERT_FAILED();
    }

    init_position(TRIGGER_DISTANCE);

    DEBUG_PRINT("INF3995 module initialized\n");

    while (1)
    {
        if (receive_command(&CommandRX) == 0)
        {
            CommandRX.command_id = -1;
        }

        handle_state(&CommandRX, &State);
    }
}
