# Add tests

enable_testing()

add_executable(
        configuration_test
        configuration_test.cpp
)
target_link_libraries(
        configuration_test
        GTest::gtest_main
)

include(GoogleTest)
include(CTest)

gtest_discover_tests(configuration_test)

add_test(NAME configuration_test COMMAND configuration_test)