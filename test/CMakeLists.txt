
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories( ../include)

add_executable(
  TestServer
  test.cpp
  ../src/log.cpp
  ../src/buffer.cpp
)


target_link_libraries(
    TestServer
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestServer)