add_subdirectory(test-runner)
add_subdirectory(chibios-syscalls)
add_subdirectory(pid)
add_subdirectory(filter)
add_subdirectory(cmp)
add_subdirectory(cmp_mem_access)
add_subdirectory(parameter)
add_subdirectory(crc)
add_subdirectory(parameter_flash_storage)
add_subdirectory(timestamp)
add_subdirectory(msgbus)
add_subdirectory(trace)

add_library(Eigen INTERFACE)
target_include_directories(Eigen INTERFACE eigen/)

# TODO(antoinealb): I am not completely sure if this is the best way to
# implement the import of the UAVCAN library into our build system. It appears
# UAVCAN has some level of support for CMake but I have yet to investigate
# this.

# Build UAVCAN list
set(DSDLC_INPUTS uavcan/dsdl/uavcan ../uavcan_data_types/cvra)
set(DSDLC_OUTPUT ${CMAKE_BINARY_DIR}/dsdlc_generated)

set(DSDLC_INPUT_FILES "")
set(DSDLC_OUTPUT_HEADERS )
foreach(DSDLC_INPUT ${DSDLC_INPUTS})
    file(GLOB_RECURSE DSDLC_NEW_INPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR} "${DSDLC_INPUT}/*.uavcan")
    get_filename_component(DSDL_NAMESPACE ${DSDLC_INPUT} NAME_WE)
    message(STATUS "UAVCAN namespace: ${DSDL_NAMESPACE}")
    foreach(NEW_FILE ${DSDLC_NEW_INPUT_FILES})
        get_filename_component(DSDLC_INPUT ${DSDLC_INPUT} ABSOLUTE)
        file(RELATIVE_PATH NEW_FILE ${DSDLC_INPUT} ${NEW_FILE})
        message(STATUS ${NEW_FILE})
        string(REGEX REPLACE "\\.uavcan$" "" NEW_FILE ${NEW_FILE})
        message(STATUS ${NEW_FILE})
        string(REGEX REPLACE "[0-9]+\\." "" NEW_FILE ${NEW_FILE})
        message(STATUS ${NEW_FILE})
        set(NEW_FILE "${DSDLC_OUTPUT}/${DSDL_NAMESPACE}/${NEW_FILE}.hpp")
        message(STATUS ${NEW_FILE})
        list(APPEND DSDLC_OUTPUT_HEADERS ${NEW_FILE})
    endforeach()
    set(DSDLC_INPUT_FILES ${DSDLC_INPUT_FILES} ${DSDLC_NEW_INPUT_FILES})
endforeach()



add_custom_command(COMMAND ${PYTHON} uavcan/libuavcan/dsdl_compiler/libuavcan_dsdlc ${DSDLC_INPUTS} -O${DSDLC_OUTPUT}
                   OUTPUT ${DSDLC_OUTPUT_HEADERS}
                   DEPENDS ${DSDLC_INPUT_FILES}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   COMMENT "Running dsdl compiler")

add_library(dsdlc INTERFACE)
target_sources(dsdlc INTERFACE ${DSDLC_OUTPUT_HEADERS})

add_library(uavcan
    uavcan/libuavcan/src/driver/uc_can.cpp
    uavcan/libuavcan/src/marshal/uc_bit_array_copy.cpp
    uavcan/libuavcan/src/marshal/uc_bit_stream.cpp
    uavcan/libuavcan/src/marshal/uc_float_spec.cpp
    uavcan/libuavcan/src/marshal/uc_scalar_codec.cpp
    uavcan/libuavcan/src/node/uc_generic_publisher.cpp
    uavcan/libuavcan/src/node/uc_generic_subscriber.cpp
    uavcan/libuavcan/src/node/uc_global_data_type_registry.cpp
    uavcan/libuavcan/src/node/uc_scheduler.cpp
    uavcan/libuavcan/src/node/uc_service_client.cpp
    uavcan/libuavcan/src/node/uc_timer.cpp
    uavcan/libuavcan/src/protocol/uc_dynamic_node_id_client.cpp
    uavcan/libuavcan/src/protocol/uc_node_status_provider.cpp
    uavcan/libuavcan/src/transport/uc_can_acceptance_filter_configurator.cpp
    uavcan/libuavcan/src/transport/uc_can_io.cpp
    uavcan/libuavcan/src/transport/uc_crc.cpp
    uavcan/libuavcan/src/transport/uc_dispatcher.cpp
    uavcan/libuavcan/src/transport/uc_frame.cpp
    uavcan/libuavcan/src/transport/uc_outgoing_transfer_registry.cpp
    uavcan/libuavcan/src/transport/uc_transfer.cpp
    uavcan/libuavcan/src/transport/uc_transfer_buffer.cpp
    uavcan/libuavcan/src/transport/uc_transfer_listener.cpp
    uavcan/libuavcan/src/transport/uc_transfer_receiver.cpp
    uavcan/libuavcan/src/transport/uc_transfer_sender.cpp
    uavcan/libuavcan/src/uc_data_type.cpp
    uavcan/libuavcan/src/uc_dynamic_memory.cpp
    uavcan/libuavcan/src/uc_error.cpp
)

target_include_directories(uavcan PUBLIC uavcan/libuavcan/include)
target_compile_definitions(uavcan PUBLIC "UAVCAN_CPP_VERSION=UAVCAN_CPP11")
target_include_directories(uavcan PUBLIC ${DSDLC_OUTPUT})
target_link_libraries(uavcan dsdlc)
