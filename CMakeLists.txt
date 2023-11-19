cmake_minimum_required(VERSION 3.26)
project(libwebrtc)
set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

# install deps
include(cmake/libwebrtc.cmake)
include(cmake/ixwebsocket.cmake)
include(cmake/json.cmake)
include(cmake/stduuid.cmake)

# libwebrtc
add_subdirectory(${CMAKE_SOURCE_DIR}/libwebrtc)

# add custom library
add_subdirectory(${CMAKE_SOURCE_DIR}/src/websockets)

# third party libs
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/olivec)

function(add_copy_command target)
    add_custom_command(
        TARGET ${target} PRE_BUILD # 或者POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBWEBRTC} ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBWEBRTC_DLL} ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Copying modified libwebrtc..."
    )
endfunction()

file(COPY ${LIBWEBRTC} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${LIBWEBRTC} DESTINATION ${CMAKE_SOURCE_DIR})
file(COPY ${LIBWEBRTC_DLL} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${LIBWEBRTC_DLL} DESTINATION ${CMAKE_SOURCE_DIR})

# smoke
add_executable(libwebrtc_smoke main.cpp)
target_link_libraries(libwebrtc_smoke PUBLIC ${LIBWEBRTC})
target_include_directories(libwebrtc_smoke PUBLIC ${CMAKE_SOURCE_DIR}/libwebrtc/include)
add_copy_command(libwebrtc_smoke)

# add test
add_subdirectory(test)

# example
add_subdirectory(example)