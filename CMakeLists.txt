cmake_minimum_required(VERSION 3.15)
project(YAQ VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
# Find dependencies
find_package(GTest REQUIRED)

# target_include_directories(${PROJECT_NAME}
#     # PUBLIC
#         # ${GTest_INCLUDE_DIRS}
# )

# Add executable
add_executable(${PROJECT_NAME}_app
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}_app
    PRIVATE
        ${GTest_LIBRARIES}
)

# Add tests
enable_testing()
add_executable(${PROJECT_NAME}_tests
    tests/test_main.cpp
)

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        ${GTest_LIBRARIES}
)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests) 