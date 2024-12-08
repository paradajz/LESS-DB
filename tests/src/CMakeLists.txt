find_package(glog 0.6.0 REQUIRED)

add_library(liblessdb-test-common)

target_sources(liblessdb-test-common
    PRIVATE
    main.cpp
)

target_include_directories(liblessdb-test-common
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../include
)

target_link_libraries(liblessdb-test-common
    PUBLIC
    glog::glog
    gtest
)

add_subdirectory(test)