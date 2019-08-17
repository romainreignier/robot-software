#ifndef DIGITALINPUT_PUB_HPP
#define DIGITALINPUT_PUB_HPP

#include <uavcan/uavcan.hpp>

#include <cvra/io/DigitalInput.hpp>

class DigitalInputPub
{
public:
    DigitalInputPub(uavcan::INode& node);
    void publish();

private:
    uavcan::Publisher<cvra::io::DigitalInput> m_pub;
};

#endif
