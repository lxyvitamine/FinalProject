cmake_minimum_required(VERSION 3.10)
project(grpc_lab)

set(CMAKE_CXX_STANDARD 14)

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
include(FindProtobuf)
find_package(Protobuf REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Find gRPC installation
# /usr/local/grpc
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)

message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto files
get_filename_component(finalec2_proto "finalec2.proto" ABSOLUTE)
get_filename_component(finalec2_proto_path "${finalec2_proto}" PATH)

# Generated sources
set(finalec2_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/finalec2.pb.cc")
set(finalec2_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/finalec2.pb.h")
set(finalec2_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/finalec2.grpc.pb.cc")
set(finalec2_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/finalec2.grpc.pb.h")
set(candidate_hdrs "${CMAKE_CURRENT_BINARY_DIR}/candidate.h")
set(candidate_srcs "${CMAKE_CURRENT_BINARY_DIR}/candidate.cpp")
set(voter_hdrs "${CMAKE_CURRENT_BINARY_DIR}/voter.h")
set(voter_srcs "${CMAKE_CURRENT_BINARY_DIR}/voter.cpp")
set(helper_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helper.h")
set(helper_srcs "${CMAKE_CURRENT_BINARY_DIR}/helper.cpp")

add_custom_command(
        OUTPUT "${finalec2_proto_srcs}" "${finalec2_proto_hdrs}" "${finalec2_grpc_srcs}" "${finalec2_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${finalec2_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${finalec2_proto}"
        DEPENDS "${finalec2_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_executable(client ${finalec2_proto_srcs} ${finalec2_grpc_srcs} client.cpp)
target_link_libraries(client ${_PROTOBUF_LIBPROTOBUF} ${_GRPC_GRPCPP_UNSECURE})

add_executable(server ${finalec2_proto_srcs} ${finalec2_grpc_srcs} ${voter_srcs} ${candidate_srcs} ${helper_srcs}  server.cpp)
target_link_libraries(server ${_PROTOBUF_LIBPROTOBUF} ${_GRPC_GRPCPP_UNSECURE})