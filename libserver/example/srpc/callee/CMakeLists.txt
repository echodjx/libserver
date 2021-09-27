set(SRC_LIST userservice.cpp ../user.pb.cc)
add_executable(provider ${SRC_LIST})

target_link_libraries(provider protobuf srpc)