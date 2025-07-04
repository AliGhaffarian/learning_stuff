BUILD_DIR=build
DEBUG_DIR=debug_build
SRC_DIR=src
INCLUDE_DIR=includes
TEST_DIR=tests
CFLAGS=-I includes

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0 -DDEBUG
	BUILD_DIR=$(DEBUG_DIR)
endif

all: prep $(BUILD_DIR)/server

prep:
	mkdir -p $(BUILD_DIR)
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DEBUG_DIR)
	cd $(TEST_DIR) && \
		make clean

compile_commands:
	bear -- make DEBUG=1
	bear --append -- make test

test:
	cd $(TEST_DIR) && \
		make && \
		cd $(BUILD_DIR) && \
		./run_tests

$(BUILD_DIR)/server: $(BUILD_DIR)/server.o $(BUILD_DIR)/http_server.o $(BUILD_DIR)/http.o $(BUILD_DIR)/socket_utilities.o $(BUILD_DIR)/utilities.o \
	$(INCLUDE_DIR)/*.h

	gcc $(CFLAGS) $^  \
		-o $@ \
		-lpthread

$(BUILD_DIR)/server.o: server.c
	gcc $(CFLAGS) -c server.c \
		-o $(BUILD_DIR)/server.o

$(BUILD_DIR)/http_server.o: $(SRC_DIR)/http_server.c $(INCLUDE_DIR)/http_server.h
	gcc $(CFLAGS) -c $(SRC_DIR)/http_server.c \
		-o $(BUILD_DIR)/http_server.o \

$(BUILD_DIR)/http.o: $(SRC_DIR)/http.c $(INCLUDE_DIR)/http.h
	gcc $(CFLAGS) -c $(SRC_DIR)/http.c \
		-o $(BUILD_DIR)/http.o

$(BUILD_DIR)/socket_utilities.o: $(SRC_DIR)/socket_utilities.c $(INCLUDE_DIR)/socket_utilities.h
	gcc $(CFLAGS) -c $(SRC_DIR)/socket_utilities.c \
		-o $(BUILD_DIR)/socket_utilities.o

$(BUILD_DIR)/utilities.o: $(SRC_DIR)/utilities.c $(INCLUDE_DIR)/utilities.h
	gcc $(CFLAGS) -c $(SRC_DIR)/utilities.c \
		-o $(BUILD_DIR)/utilities.o
