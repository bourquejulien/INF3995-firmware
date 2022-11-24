#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "param.h"

#include "app.h"
#include "cfassert.h"
#include "debug.h"
#include "usec_time.h"

#include "controller/controller.h"
#include "position/position.h"
#include "status/status.h"
#include "synchronization/synchronization.h"

#define DEBUG_MODULE "MAIN"

static float telemetric_update = 500;

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

    initUsecTimer();
    init_position();
    init_synchronization();

    uint64_t  last_clock = usecTimestamp();

    DEBUG_PRINT("INF3995 module initialized\n");

    while (1)
    {
        if (receive_command(&CommandRX) == 0)
        {
            CommandRX.command_id = -1;
        }

        handle_state(&CommandRX, &State);
        synchronize_drones();

        uint64_t time_since_update_ms = ((usecTimestamp() - last_clock) / 1000);

        if(time_since_update_ms > telemetric_update)
        {
            last_clock = usecTimestamp();
            update_status(&State);
        }
    }
}

PARAM_GROUP_START(app)
PARAM_ADD(PARAM_FLOAT, telemetric_update, &telemetric_update)
PARAM_GROUP_STOP(app)
