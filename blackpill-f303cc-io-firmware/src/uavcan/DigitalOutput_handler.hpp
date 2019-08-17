#ifndef DIGITALOUTPUT_HANDLER_HPP
#define DIGITALOUTPUT_HANDLER_HPP

#include <uavcan/uavcan.hpp>

#include <cvra/io/DigitalOutput.hpp>

class DigitalOutputSub
{
public:
    DigitalOutputSub(uavcan::INode& node, uavcan::Logger& logger);
    void callback(const uavcan::ReceivedDataStructure<cvra::io::DigitalOutput>& msg);

private:
    uavcan::Logger& m_logger;
    uavcan::Subscriber<cvra::io::DigitalOutput> m_sub;
};

#endif
