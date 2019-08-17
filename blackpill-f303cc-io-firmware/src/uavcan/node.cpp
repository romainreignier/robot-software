#include <ch.h>
#include <hal.h>

#include <uavcan/uavcan.hpp>
#include <uavcan_stm32/uavcan_stm32.hpp>

#include <error/error.h>
#include <version/version.h>

#include "DigitalInput_pub.hpp"
#include "DigitalOutput_handler.hpp"
#include "Reboot_handler.hpp"
#include "ServoPWM_handler.hpp"
#include "UAVCANRestart_handler.hpp"

#include "node.h"

#define UAVCAN_SPIN_FREQ 10 // [Hz]

namespace uavcan_node {
static const int RxQueueSize = 32;
static const uint32_t BitRate = 1000000;

const unsigned NodeMemoryPoolSize = 4096;
typedef uavcan::Node<NodeMemoryPoolSize> Node;

static uavcan::ISystemClock& getSystemClock()
{
    return uavcan_stm32::SystemClock::instance();
}

static uavcan::ICanDriver& getCanDriver()
{
    static uavcan_stm32::CanInitHelper<RxQueueSize> can;
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        int res = can.init(BitRate);
        if (res < 0) {
            chSysHalt("CAN driver");
        }
    }
    return can.driver;
}

static Node& getNode()
{
    static Node node(getCanDriver(), getSystemClock());
    return node;
}

void main(unsigned int id, const char* name)
{
    chRegSetThreadName("uavcan");

    Node& node = getNode();

    node.setNodeID(uavcan::NodeID(id));

    uavcan::protocol::SoftwareVersion sw_version;
    sw_version.major = 1;
    sw_version.optional_field_flags = sw_version.OPTIONAL_FIELD_FLAG_VCS_COMMIT;
    sw_version.vcs_commit = software_version_short;
    node.setSoftwareVersion(sw_version);

    uavcan::protocol::HardwareVersion hw_version;
    hw_version.major = 1;
    node.setHardwareVersion(hw_version);

    node.setName(name);

    UAVCANRestartHandler restarter;
    node.setRestartRequestHandler(&restarter);

    if (node.start() < 0) {
        chSysHalt("node start");
    }

    node.setModeOperational();
    node.setHealthOk();

    node.getLogger().setLevel(uavcan::protocol::debug::LogLevel::DEBUG);
    node.logInfo("main", "Hello world! My Node ID: %*",
                 static_cast<int>(node.getNodeID().get()));

    DigitalInputPub inputsPub{node};
    DigitalOutputSub outputsSub{node};

    uavcan::Subscriber<cvra::io::ServoPWM> servo_pwm_sub(node);
    if (servo_pwm_sub.start(ServoPWM_handler)) {
        LOG_ERROR("Cannot start servo PWM handler!");
    }

    uavcan::Subscriber<cvra::Reboot> reboot_sub(node);
    if (reboot_sub.start(Reboot_handler)) {
        LOG_ERROR("Cannot start reboot handler!");
    }

    while (true) {
        node.spin(uavcan::MonotonicDuration::fromMSec(1000 / UAVCAN_SPIN_FREQ));
        inputsPub.publish();
    }
}
} // namespace uavcan_node

void uavcan_start(unsigned int node_id, const char* node_name)
{
    uavcan_node::main(node_id, node_name);
}
