#ifndef TRACE_POINTS_H
#define TRACE_POINTS_H
#include <trace/trace.h>

#define TRACE_POINTS                       \
    C(TRACE_POINT_UWB_IRQ)                 \
    C(TRACE_POINT_UWB_SEND_ADVERTISEMENT)  \
    C(TRACE_POINT_UWB_TX_DONE)             \
    C(TRACE_POINT_DATA_PACKET_READY)       \
    C(TRACE_POINT_DATA_PACKET_RECEIVED)    \
    C(TRACE_POINT_DATA_PACKET_SENT_TO_CAN) \
    C(TRACE_POINT_RX_ADVERTISEMENT)        \
    C(TRACE_POINT_RX_REPLY)                \
    C(TRACE_POINT_RX_FINALIZATION)         \
    C(TRACE_POINT_RX_INITIATE)             \
    C(TRACE_POINT_RX_USER_DATA)            \
    C(TRACE_POINT_RX_ERROR)                \
    C(TRACE_POINT_RX_TIMEOUT)              \
    C(TRACE_POINT_REJECTED_PACKET)         \
    C(TRACE_POINT_REJECTED_PAN_ID)         \
    C(TRACE_POINT_UWB_RX)

/* List of all trace points in numerical format. */
#undef C
#define C(x) x,
enum {
    TRACE_POINTS
};

#endif /* TRACE_POINTS_H */
