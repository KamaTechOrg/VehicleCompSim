cmake_minimum_required(VERSION 3.8)

include(cmake/get_grpc.cmake)

# Proto file
get_filename_component(hsm_proto "./hsm.proto" ABSOLUTE)
get_filename_component(hsm_proto_path "${hsm_proto}" PATH)

# Generated sources
set(hsm_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/hsm.pb.cc")
set(hsm_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/hsm.pb.h")
set(hsm_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/hsm.grpc.pb.cc")
set(hsm_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/hsm.grpc.pb.h")
add_custom_command(
      OUTPUT "${hsm_proto_srcs}" "${hsm_proto_hdrs}" "${hsm_grpc_srcs}" "${hsm_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hsm_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hsm_proto}"
      DEPENDS "${hsm_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# hsm_grpc_proto
add_library(hsm_grpc_proto
  ${hsm_grpc_srcs}
  ${hsm_grpc_hdrs}
  ${hsm_proto_srcs}
  ${hsm_proto_hdrs})
target_link_libraries(hsm_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

foreach(_target 
  hsm_grpc_server hsm_grpc_client)
    add_library(${_target} "${_target}.cpp")
    target_link_libraries(${_target}
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    hsm_grpc_proto
    HSM
  )
endforeach()

# server main
add_executable(hsm_grpc_server_main hsm_grpc_server_main.cpp)
target_link_libraries(hsm_grpc_server_main hsm_grpc_server
    absl::flags
    absl::flags_parse
)

# test
add_executable(hsm_grpc_client_test RPC_test.cpp)
target_link_libraries(
  hsm_grpc_client_test hsm_grpc_client hsm_grpc_server HSM GTest::gtest_main
)
gtest_discover_tests(hsm_grpc_client_test)
