#include <hal.h>

#include "DigitalOutput_handler.hpp"

DigitalOutputSub::DigitalOutputSub(uavcan::INode& node, uavcan::Logger& logger)
: m_logger{logger}, m_sub{node}
{
    m_sub.start([&](const uavcan::ReceivedDataStructure<cvra::io::DigitalOutput>& msg){
        callback(msg);
    });
}

void DigitalOutputSub::callback(const uavcan::ReceivedDataStructure<cvra::io::DigitalOutput>& msg)
{
    palWritePad(PORT_OUT0, PAD_OUT0, msg.pin[0]);
    palWritePad(PORT_OUT1, PAD_OUT1, msg.pin[1]);
    palWritePad(PORT_OUT2, PAD_OUT2, msg.pin[2]);
    palWritePad(PORT_OUT3, PAD_OUT3, msg.pin[3]);
    palWritePad(PORT_OUT4, PAD_OUT4, msg.pin[4]);

    m_logger.logInfo("DigitalOutputSub", "Received new outputs values: %d %d %d %d %d",
                 msg.pin[0], msg.pin[1], msg.pin[2], msg.pin[3], msg.pin[4]);
}
