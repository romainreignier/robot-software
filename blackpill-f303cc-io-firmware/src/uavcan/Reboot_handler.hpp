#ifndef REBOOT_HANDLER_HPP
#define REBOOT_HANDLER_HPP

#include <cvra/Reboot.hpp>
#include <uavcan/uavcan.hpp>

void Reboot_handler(const uavcan::ReceivedDataStructure<cvra::Reboot>& msg);

#endif
