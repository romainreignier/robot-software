#pragma once

#include <uavcan/uavcan.hpp>

class UAVCANRestartHandler : public uavcan::IRestartRequestHandler
{
public:
    virtual ~UAVCANRestartHandler() { }

    /**
     * This method shall do either:
     *  - restart the local node immediately;
     *  - initiate the restart procedure to complete it asynchronously;
     *  - reject the restart request and return false.
     *
     * If the restart requets was accepted, this method shall either return true or don't return at all.
     */
    bool handleRestartRequest(uavcan::NodeID request_source) override;
};
