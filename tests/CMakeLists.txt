# Tests subdirectory CMakeLists.txt

# Find GTest dependency
find_package(GTest REQUIRED)

# Test source files
set(test_files
    init_config_reader_test.cpp
    yaq_test.cpp
    logger_test.cpp
    connection_test.cpp
    network_test.cpp
    protocol_test.cpp
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
        yaq_lib
)
target_include_directories(${TEST_APP_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

# Enable code coverage
if(CMAKE_COMPILER_IS_GNUCXX)
    message("Collect coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    
    # Add custom target for coverage report
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory coverage
        COMMAND find . -name "*.gcda" -delete
        COMMAND ${CMAKE_COMMAND} -E chdir coverage ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=coverage.profraw $<TARGET_FILE:${PROJECT_NAME}_tests>
        COMMAND lcov --directory . --capture --output-file coverage/coverage.info --ignore-errors mismatch
        COMMAND lcov --remove coverage/coverage.info '*tests/*' --output-file coverage/coverage.filtered.info
        COMMAND genhtml coverage/coverage.filtered.info --output-directory coverage/html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS ${PROJECT_NAME}_tests
        COMMENT "Generating code coverage report..."
    )
endif()

add_test(NAME ${TEST_APP_NAME} COMMAND ${PROJECT_NAME}_tests)
