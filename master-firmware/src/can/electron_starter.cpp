#include "electron_starter.hpp"

#include <cvra/uwb_beacon/DataPacket.hpp>
#include "config.h"

using DataPacket = cvra::uwb_beacon::DataPacket;

static uavcan::LazyConstructor<uavcan::Publisher<DataPacket>> data_pub;

static bool electron_started = false;
static bool electron_start_confirmed = false;
static bool electron_arrived = false;

static void data_packet_cb(const uavcan::ReceivedDataStructure<DataPacket>& msg)
{
    if (msg.src_addr == config_get_integer("/master/electron_uwb_mac")) {
        if (!electron_start_confirmed && msg.data == "started") {
            electron_start_confirmed = true;
        }
        if (!electron_arrived && msg.data == "arrived") {
            electron_arrived = true;
        }
    }
}

int electron_starter_init(uavcan::INode& node)
{
    data_pub.construct<uavcan::INode&>(node);
    static uavcan::Timer periodic_timer(node);

    periodic_timer.setCallback(
        [&](const uavcan::TimerEvent& event) {
            (void)event;

            if (!electron_started) {
                return;
            }

            auto tx_packet = DataPacket();
            tx_packet.dst_addr = config_get_integer("/master/electron_uwb_mac");

            data_pub->broadcast(tx_packet);
        });

    /* Send a message to the electron every second */
    periodic_timer.startPeriodic(uavcan::MonotonicDuration::fromMSec(1000));

    static uavcan::Subscriber<DataPacket> subscriber(node);

    return subscriber.start(data_packet_cb);
}

void electron_starter_start(void)
{
    electron_started = true;
}

bool electron_is_arrived()
{
    return electron_arrived;
}