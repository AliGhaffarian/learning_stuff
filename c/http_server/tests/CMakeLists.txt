add_executable(run_tests
	run_tests.c
	src/generic_test_context.c
	src/mocked_functions.c
	src/test_source_http.c
	src/test_source_socket_utilities.c
	src/test_source_utilities.c

	${CMAKE_SOURCE_DIR}/src/http.c
	${CMAKE_SOURCE_DIR}/src/socket_utilities.c
	${CMAKE_SOURCE_DIR}/src/utilities.c
)
target_include_directories(run_tests
	PUBLIC ${CMAKE_SOURCE_DIR}/includes
	PUBLIC includes
)
target_link_libraries(run_tests cmocka)
