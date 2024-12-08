add_executable(liblessdb-test
    test.cpp
)

target_link_libraries(liblessdb-test
    PRIVATE
    liblessdb-test-common
    liblessdb
)

target_compile_definitions(liblessdb-test
    PRIVATE
    TEST
)

add_test(
    NAME test_build
    COMMAND
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target liblessdb-test
)

set_tests_properties(test_build
    PROPERTIES
    FIXTURES_SETUP
    test_fixture
)

add_test(
    NAME test
    COMMAND $<TARGET_FILE:liblessdb-test>
)

set_tests_properties(test
    PROPERTIES
    FIXTURES_REQUIRED
    test_fixture
)