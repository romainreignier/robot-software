#include "data_packet_handler.hpp"
#include <uavcan/uavcan.hpp>
#include <cvra/uwb_beacon/DataPacket.hpp>

#define UWB_MAC_START_SIGNAL 42

using DataPacket = cvra::uwb_beacon::DataPacket;
static uavcan::LazyConstructor<uavcan::Publisher<DataPacket>> data_pub;

static bool _start_received = false;
static uint16_t robot_mac = 0xdead;

bool start_received()
{
    return _start_received;
}

static void data_packet_cb(const uavcan::ReceivedDataStructure<DataPacket>& msg)
{
    if (msg.dst_addr == UWB_MAC_START_SIGNAL) {
        _start_received = true;
        robot_mac = msg.src_addr;
    }
}

int data_packet_handler_init(uavcan::INode& node)
{
    static uavcan::Subscriber<DataPacket> subscriber(node);

    data_pub.construct<uavcan::INode&>(node);

    return subscriber.start(data_packet_cb);
}

static void robot_send(const char* msg)
{
    if (!_start_received) {
        return;
    }
    DataPacket packet;
    packet.data = msg;
    packet.src_addr = UWB_MAC_START_SIGNAL;
    packet.dst_addr = robot_mac;
    data_pub->broadcast(packet);
}

void robot_notify_started()
{
    robot_send("started");
}

void robot_notify_arrived()
{
    robot_send("arrived");
}
