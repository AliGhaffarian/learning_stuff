cmake_minimum_required(VERSION 4.0.3)
project(http_server)
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
add_executable(run_tests
	tests/run_tests.c
	tests/src/generic_test_context.c
	tests/src/mocked_functions.c
	tests/src/test_source_http.c
	tests/src/test_source_socket_utilities.c
	tests/src/test_source_utilities.c

	src/http.c
	src/socket_utilities.c
	src/utilities.c
)
target_include_directories(run_tests
	PUBLIC ./includes
	PUBLIC ./tests/includes
)
target_link_libraries(run_tests cmocka)
