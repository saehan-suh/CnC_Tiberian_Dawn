add_executable(cnc_td_tests
    test_facing.cpp
)

target_link_libraries(cnc_td_tests PRIVATE
    cnc_td_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cnc_td_tests)