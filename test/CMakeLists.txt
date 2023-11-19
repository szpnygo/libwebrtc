FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(googletest)

include(CTest)
include(GoogleTest)

add_executable(websockets_test websockets_test.cpp)
target_link_libraries(websockets_test PRIVATE gtest_main)
target_link_libraries(websockets_test PUBLIC wsclient)
target_link_libraries(websockets_test PUBLIC nlohmann_json::nlohmann_json)
gtest_discover_tests(websockets_test)
