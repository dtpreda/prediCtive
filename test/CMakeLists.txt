# Add tests

enable_testing()

add_executable(
        configuration_test
        configuration_test.cpp
)

add_executable(
        parser_test
        parser_test.cpp
)

target_link_libraries(
        configuration_test
        GTest::gtest_main
)

target_link_libraries(
        parser_test
        GTest::gtest_main
        testables
)

include(GoogleTest)
include(CTest)

gtest_discover_tests(configuration_test parser_test)

add_test(NAME configuration_test COMMAND configuration_test)
add_test(NAME parser_test COMMAND parser_test)