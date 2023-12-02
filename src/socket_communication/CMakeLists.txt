set(SOURCE_FILES
        server_socket_handler.cpp server_socket_handler.h
        exceptions.h
        client_socket_handler.cpp client_socket_handler.h
        socket_utilities.cpp socket_utilities.h
        connection_manager.cpp connection_manager.h
        client_info_factory.h)

add_library(socket_communication ${SOURCE_FILES})
target_link_libraries(socket_communication PUBLIC messages types)
