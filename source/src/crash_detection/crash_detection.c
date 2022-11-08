#include "crash_detection.h"
#include "supervisor.h"

bool is_crashed() {
    return supervisorIsTumbled();
}