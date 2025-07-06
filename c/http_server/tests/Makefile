BUILD_DIR=build
SRC_DIR=src
INCLUDE_DIR=includes
TO_BE_TESTED_ROOT_DIR=..
TO_BE_TESTED_SOURCES=$(wildcard $(TO_BE_TESTED_ROOT_DIR)/$(SRC_DIR)/*.c) 
TO_BE_TESTED_INCLUDES=$(wildcard $(TO_BE_TESTED_ROOT_DIR)/$(INCLUDE_DIR)/*.h) 
CFLAGS= -I$(INCLUDE_DIR) -I$(TO_BE_TESTED_ROOT_DIR)/includes -g
LDFLAGS= -Wl,--wrap=recv 
all: prep $(BUILD_DIR)/run_tests

clean:
	rm -rf $(BUILD_DIR)

prep:
	mkdir -p $(BUILD_DIR)


$(BUILD_DIR)/run_tests: $(BUILD_DIR)/run_tests.o \
	$(BUILD_DIR)/test_source_http.o $(BUILD_DIR)/test_source_utilities.o $(BUILD_DIR)/test_source_socket_utilities.o \
	$(BUILD_DIR)/generic_test_context.o $(BUILD_DIR)/mocked_functions.o \
	$(TO_BE_TESTED_SOURCES) \
	$(wildcard $(INCLUDE_DIR)/*.h) $(TO_BE_TESTED_INCLUDES)

	gcc $(CFLAGS) $(LDFLAGS) $^ \
		-o $@ \
		-lcmocka

$(BUILD_DIR)/mocked_functions.o: $(SRC_DIR)/mocked_functions.c $(INCLUDE_DIR)/mocked_functions.h
	gcc $(CFLAGS) -c $(SRC_DIR)/mocked_functions.c \
		-o $@

$(BUILD_DIR)/test_source_socket_utilities.o: $(SRC_DIR)/test_source_socket_utilities.c $(INCLUDE_DIR)/test_source_socket_utilities.h
	gcc $(CFLAGS) -c $(SRC_DIR)/test_source_socket_utilities.c \
		-o $@

$(BUILD_DIR)/generic_test_context.o: $(SRC_DIR)/generic_test_context.c $(INCLUDE_DIR)/generic_test_context.h
	gcc $(CFLAGS) -c $(SRC_DIR)/generic_test_context.c \
		-o $@

$(BUILD_DIR)/run_tests.o: run_tests.c 
	gcc $(CFLAGS) -c run_tests.c \
		-o $(BUILD_DIR)/run_tests.o

$(BUILD_DIR)/test_source_http.o: $(SRC_DIR)/test_source_http.c $(INCLUDE_DIR)/test_source_http.h
	gcc $(CFLAGS) -c $(SRC_DIR)/test_source_http.c \
		-o $(BUILD_DIR)/test_source_http.o

$(BUILD_DIR)/test_source_utilities.o: $(SRC_DIR)/test_source_utilities.c $(INCLUDE_DIR)/test_source_utilities.h
	gcc $(CFLAGS) -c $(SRC_DIR)/test_source_utilities.c \
		-o $(BUILD_DIR)/test_source_utilities.o
