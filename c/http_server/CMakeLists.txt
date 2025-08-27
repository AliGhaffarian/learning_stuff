cmake_minimum_required(VERSION 4.0.3)
project(http_server)

if(${DEBUG})
	add_compile_options(-DDEBUG)
endif()

option(DEBUG "add debug logs" 0)

add_executable(http_server
	server.c
	src/errno2http_response.c
	src/http.c
	src/http_server.c
	src/socket_utilities.c
	src/utilities.c
)
target_include_directories(http_server 
	PUBLIC includes/
)
add_subdirectory(tests)
