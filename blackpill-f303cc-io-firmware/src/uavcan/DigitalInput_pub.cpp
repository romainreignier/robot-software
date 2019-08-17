#include <ch.h>
#include <hal.h>
#include "DigitalInput_pub.hpp"


void read_inputs(bool* result)
{
    result[0] = palReadPad(PORT_IN0, PAD_IN0);
    result[1] = palReadPad(PORT_IN1, PAD_IN1);
    result[2] = palReadPad(PORT_IN2, PAD_IN2);
    result[3] = palReadPad(PORT_IN3, PAD_IN3);
    result[4] = palReadPad(PORT_IN4, PAD_IN4);
    result[5] = palReadPad(PORT_IN5, PAD_IN5);
    result[6] = palReadPad(PORT_IN6, PAD_IN6);
    result[7] = palReadPad(PORT_IN7, PAD_IN7);
}

DigitalInputPub::DigitalInputPub(uavcan::INode& node)
: m_pub{node}
{
    m_pub.init();
}

void DigitalInputPub::publish()
{
    bool values[8];

    read_inputs(values);

    cvra::io::DigitalInput msg = cvra::io::DigitalInput();

    for (int i = 0; i < 8; i++) {
        msg.pin[i] = values[i];
    }

    m_pub.broadcast(msg);
}
