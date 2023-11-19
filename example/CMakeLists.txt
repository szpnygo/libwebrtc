add_executable(example_app main.cpp)
target_link_libraries(example_app PRIVATE ${LIBWEBRTC})
target_link_libraries(example_app PRIVATE wsclient)
target_link_libraries(example_app PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(example_app PRIVATE ${olivec})
target_include_directories(example_app PUBLIC ${CMAKE_SOURCE_DIR}/libwebrtc/include)

add_copy_command(example_app)
