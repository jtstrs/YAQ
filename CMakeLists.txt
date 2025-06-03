cmake_minimum_required(VERSION 3.15)
project(YAQ VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
# Find dependencies
find_package(GTest REQUIRED)

message(STATUS "GTest_FOUND: ${GTest_FOUND}")
message(STATUS "GTest_INCLUDE_DIRS: ${GTest_INCLUDE_DIRS}")
message(STATUS "GTest_LIBRARIES: ${GTest_LIBRARIES}")

# Add main library
add_library(${PROJECT_NAME}
    src/lib.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${GTest_INCLUDE_DIRS}
)

# Add executable
add_executable(${PROJECT_NAME}_app
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}_app
    PRIVATE
        ${PROJECT_NAME}
        ${GTest_LIBRARIES}
)

# Add tests
enable_testing()
add_executable(${PROJECT_NAME}_tests
    tests/test_main.cpp
)

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        ${PROJECT_NAME}
        ${GTest_LIBRARIES}
)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests) 