#ifndef DATA_PACKET_HANDLER_HPP
#define DATA_PACKET_HANDLER_HPP

#include <uavcan/uavcan.hpp>

int data_packet_handler_init(uavcan::INode& node);

bool start_received(void);

void robot_notify_started(void);

void robot_notify_arrived(void);

#endif
