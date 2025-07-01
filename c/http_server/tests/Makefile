BUILD_DIR=build
LIB_DIR=libs
INCLUDE_DIR=includes
TO_BE_TESTED_ROOT_DIR=..
CFLAGS= -I$(INCLUDE_DIR) -I$(TO_BE_TESTED_ROOT_DIR)/includes

all: prep run_tests

clean:
	rm -rf $(BUILD_DIR)

prep:
	mkdir -p $(BUILD_DIR)

run_tests: $(BUILD_DIR)/run_tests.o $(BUILD_DIR)/test_source_utilities.o 
	gcc $(CFLAGS) $(BUILD_DIR)/run_tests.o $(BUILD_DIR)/test_source_utilities.o \
		$(TO_BE_TESTED_ROOT_DIR)/libs/utilities.c \
		-o $(BUILD_DIR)/run_tests \
		-lcmocka

$(BUILD_DIR)/run_tests.o: run_tests.c 
	gcc $(CFLAGS) -c run_tests.c \
		-o $(BUILD_DIR)/run_tests.o

$(BUILD_DIR)/test_source_utilities.o: $(LIB_DIR)/test_source_utilities.c $(INCLUDE_DIR)/test_source_utilities.h
	gcc $(CFLAGS) -c $(LIB_DIR)/test_source_utilities.c \
		-o $(BUILD_DIR)/test_source_utilities.o
