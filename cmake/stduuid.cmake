FetchContent_Declare(
  stduuid
  GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
  GIT_TAG        v1.2.3
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(stduuid)

add_definitions(-DUUID_SYSTEM_GENERATOR=ON)

include_directories(${stduuid_SOURCE_DIR}/include)
include_directories(${stduuid_SOURCE_DIR}/)
