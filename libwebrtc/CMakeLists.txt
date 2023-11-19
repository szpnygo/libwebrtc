include_directories(${CMAKE_SOURCE_DIR}/libwebrtc/include)
include_directories(${CMAKE_SOURCE_DIR}/webrtc/src)
include_directories(${CMAKE_SOURCE_DIR}/webrtc/src/api)

file(GLOB SOURCES
    "src/*.cc"
    "src/*.h"
    "src/*.cpp"
    "src/internal/*.h"
    "src/internal/*.cc"
    "src/base/*.h"
    "src/base/*.cc"
)

# webrtc third party
set(WTP ${CMAKE_SOURCE_DIR}/webrtc/src/third_party)

add_library(testlibwebrtc ${SOURCES})
target_include_directories(testlibwebrtc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(testlibwebrtc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(testlibwebrtc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/win/include)
target_include_directories(testlibwebrtc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/base)
target_include_directories(testlibwebrtc PUBLIC ${CMAKE_SOURCE_DIR}/webrtc/src)
target_include_directories(testlibwebrtc PUBLIC ${CMAKE_SOURCE_DIR}/webrtc/src/api)
target_include_directories(testlibwebrtc PUBLIC ${WTP}/abseil-cpp)
target_include_directories(testlibwebrtc PUBLIC ${WTP}/libyuv/include)
target_include_directories(testlibwebrtc PUBLIC ${WTP}/libjpeg_turbo)

add_compile_definitions(testlibwebrtc PRIVATE -DRTC_DESKTOP_DEVICE)

if(WIN32)
    add_compile_definitions(testlibwebrtc PRIVATE -DNOMINMAX)
    add_compile_definitions(testlibwebrtc PRIVATE -DWEBRTC_WIN)
    add_compile_definitions(testlibwebrtc PRIVATE -DWIN32_LEAN_AND_MEAN)
endif()