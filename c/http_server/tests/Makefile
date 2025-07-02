BUILD_DIR=build
SRC_DIR=src
INCLUDE_DIR=includes
TO_BE_TESTED_ROOT_DIR=..
CFLAGS= -I$(INCLUDE_DIR) -I$(TO_BE_TESTED_ROOT_DIR)/includes

all: prep run_tests

clean:
	rm -rf $(BUILD_DIR)

prep:
	mkdir -p $(BUILD_DIR)


run_tests: $(BUILD_DIR)/*.o $(TO_BE_TESTED_ROOT_DIR)/$(SRC_DIR)/*.c 
	gcc $(CFLAGS) $? \
		-o $(BUILD_DIR)/run_tests \
		-lcmocka

$(BUILD_DIR)/run_tests.o: run_tests.c 
	gcc $(CFLAGS) -c run_tests.c \
		-o $(BUILD_DIR)/run_tests.o

$(BUILD_DIR)/test_source_http.o: $(SRC_DIR)/test_source_http.c
	gcc $(CFLAGS) -c $(SRC_DIR)/test_source_http.c \
		-o $(BUILD_DIR)/test_source_http.o

$(BUILD_DIR)/test_source_utilities.o: $(SRC_DIR)/test_source_utilities.c $(INCLUDE_DIR)/test_source_utilities.h
	gcc $(CFLAGS) -c $(SRC_DIR)/test_source_utilities.c \
		-o $(BUILD_DIR)/test_source_utilities.o
