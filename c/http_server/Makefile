BUILD_DIR=build
LIB_DIR=libs
INCLUDE_DIR=includes

CFLAGS=-I includes

all: prep $(BUILD_DIR)/server

prep:
	mkdir -p $(BUILD_DIR)
clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/server: $(BUILD_DIR)/server.o $(BUILD_DIR)/http_server.o $(BUILD_DIR)/socket_utilities.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/http.o $(INCLUDE_DIR)/debug.h
	gcc $(CFLAGS) $(BUILD_DIR)/server.o $(BUILD_DIR)/http_server.o $(BUILD_DIR)/socket_utilities.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/http.o \
		-o $(BUILD_DIR)/server \
		-lpthread

$(BUILD_DIR)/server.o: server.c
	gcc $(CFLAGS) -c server.c \
		-o $(BUILD_DIR)/server.o

$(BUILD_DIR)/http_server.o: $(LIB_DIR)/http_server.c $(INCLUDE_DIR)/http_server.h
	gcc $(CFLAGS) -c $(LIB_DIR)/http_server.c \
		-o $(BUILD_DIR)/http_server.o \

$(BUILD_DIR)/http.o: $(LIB_DIR)/http.c $(INCLUDE_DIR)/http.h
	gcc $(CFLAGS) -c $(LIB_DIR)/http.c \
		-o $(BUILD_DIR)/http.o

$(BUILD_DIR)/socket_utilities.o: $(LIB_DIR)/socket_utilities.c $(INCLUDE_DIR)/socket_utilities.h
	gcc $(CFLAGS) -c $(LIB_DIR)/socket_utilities.c \
		-o $(BUILD_DIR)/socket_utilities.o

$(BUILD_DIR)/utilities.o: $(LIB_DIR)/utilities.c $(INCLUDE_DIR)/utilities.h
	gcc $(CFLAGS) -c $(LIB_DIR)/utilities.c \
		-o $(BUILD_DIR)/utilities.o
