#include "UAVCANRestart_handler.hpp"

#include <can-bootloader/boot_arg.h>

bool UAVCANRestartHandler::handleRestartRequest(uavcan::NodeID request_source)
{
    reboot_system(BOOT_ARG_START_APPLICATION);
    return true;
}