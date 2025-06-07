cmake_minimum_required(VERSION 3.15)
project(YAQ VERSION 0.1.0 LANGUAGES CXX)

set(APPLICATION_NAME ${PROJECT_NAME}_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
# Find dependencies
find_package(GTest REQUIRED)
find_package(Boost REQUIRED format)

set(src_files
    src/ini_config_reader.cpp
    src/yaq.cpp
)

add_executable(${APPLICATION_NAME}
    src/main.cpp
    ${src_files}
)

target_link_libraries(${APPLICATION_NAME}
    PRIVATE
        ${GTest_LIBRARIES}
)

target_include_directories(${APPLICATION_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

set(test_files
    tests/init_config_reader_test.cpp
    tests/yaq_test.cpp
)

# Add tests
enable_testing()

set(TEST_APP_NAME ${PROJECT_NAME}_tests)
add_executable(${TEST_APP_NAME}
    ${test_files}
    ${src_files}
)

target_link_libraries(${TEST_APP_NAME}
    PRIVATE
        ${GTest_LIBRARIES}
)
target_include_directories(${TEST_APP_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

add_test(NAME ${TEST_APP_NAME} COMMAND ${PROJECT_NAME}_tests) 