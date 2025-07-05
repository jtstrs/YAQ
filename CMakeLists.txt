cmake_minimum_required(VERSION 3.15)
project(YAQ VERSION 0.1.0 LANGUAGES CXX)

set(APPLICATION_NAME ${PROJECT_NAME}_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
