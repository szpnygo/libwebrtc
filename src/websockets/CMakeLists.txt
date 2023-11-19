add_library(wsclient STATIC client.cpp)
target_include_directories(wsclient PUBLIC ${CMAKE_SOURCE_DIR}/include/websockets)
target_link_libraries(wsclient PUBLIC ixwebsocket)