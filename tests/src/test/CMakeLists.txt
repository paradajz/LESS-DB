add_executable(lessdb-test
    test.cpp
)

target_link_libraries(lessdb-test
    PRIVATE
    lessdb-test-common
    lessdb-lib
)

target_compile_definitions(lessdb-test
    PRIVATE
    TEST
)

add_test(
    NAME test_build
    COMMAND
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target lessdb-test
)

set_tests_properties(test_build
    PROPERTIES
    FIXTURES_SETUP
    test_fixture
)

add_test(
    NAME test
    COMMAND $<TARGET_FILE:lessdb-test>
)

set_tests_properties(test
    PROPERTIES
    FIXTURES_REQUIRED
    test_fixture
)