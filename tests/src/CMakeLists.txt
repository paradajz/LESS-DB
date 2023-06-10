find_package(glog 0.6.0 REQUIRED)

add_library(lessdb-test-common)

target_sources(lessdb-test-common
    PRIVATE
    main.cpp
)

target_include_directories(lessdb-test-common
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../include
)

target_link_libraries(lessdb-test-common
    PUBLIC
    glog::glog
    gtest
)

add_subdirectory(test)