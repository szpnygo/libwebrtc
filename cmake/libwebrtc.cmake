# enable desktop device
add_definitions(-DRTC_DESKTOP_DEVICE)
set(LIBWEBRTC_DIR "")

if(WIN32)
    set(LIBWEBRTC_DIR "win_x64")
    set(LIBWEBRTC_NAME "libwebrtc.dll.lib")
endif()

if(APPLE)
    set(LIBWEBRTC_DIR "mac_x64")
    set(LIBWEBRTC_NAME "webrtc")
endif()

if(LINUX)
    set(LIBWEBRTC_DIR "linux_x64")
    set(LIBWEBRTC_NAME "webrtc")
endif()

if(LIBWEBRTC_DIR)
    find_library(
        LIBWEBRTC
        NAMES ${LIBWEBRTC_NAME}
        HINTS ${CMAKE_SOURCE_DIR}/libs/${LIBWEBRTC_DIR}
    )
endif()

set(LIBWEBRTC_DLL ${LIBWEBRTC})

if(WIN32)
    set(LIBWEBRTC_DLL ${CMAKE_SOURCE_DIR}/libs/${LIBWEBRTC_DIR}/libwebrtc.dll)
endif()
if(LINUX)
    set(LIBWEBRTC_DLL ${CMAKE_SOURCE_DIR}/libs/${LIBWEBRTC_DIR}/libwebrtc.so)
endif()

message(STATUS "Find libwebrtc: ${LIBWEBRTC}  ${LIBWEBRTC_DLL}")