set(src_files
    client.cpp
    client_main.cpp
)


add_executable(${APPLICATION_NAME}_client ${src_files})

target_link_libraries(${APPLICATION_NAME}_client
    PRIVATE
        yaq_lib
        ${Boost_LIBRARIES}
)