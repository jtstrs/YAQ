# Source subdirectory CMakeLists.txt

# Find Boost dependency
find_package(Boost REQUIRED format)

# Source files for the library
set(src_files
    ini_config_reader.cpp
    logger.cpp
    protocol.cpp
)

# Create library target
add_library(yaq_lib STATIC
    ${src_files}
)

target_include_directories(yaq_lib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
)

# Main application executable
add_executable(${APPLICATION_NAME}
    main.cpp
)

target_link_libraries(${APPLICATION_NAME}
    PRIVATE
        yaq_lib
        ${GTest_LIBRARIES}
        $<$<PLATFORM_ID:Linux,Darwin>:pthread>
)

target_include_directories(${APPLICATION_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
)

add_subdirectory(yaq_client)