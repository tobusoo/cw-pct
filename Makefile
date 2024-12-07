APP_NAME = jacobi
# MPI_APP_NAME=jacobi_mpi

LIBS = -lm
CFLAGS = -Wall -Werror -I src/jacobi
DEPSFLAGS = -MMD
CC = gcc

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

APP_PATH = $(BIN_DIR)/$(APP_NAME)
APP_SOURCES = $(wildcard $(SRC_DIR)/$(APP_NAME)/*.c)
APP_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(APP_SOURCES))

DEPS = $(APP_OBJECTS:.o=.d)


.PHONY: all clean
all: make-dir $(APP_PATH)
-include $(DEPS)

make-dir:
	@mkdir -p bin obj obj/src obj/src/jacobi 

$(APP_PATH): $(APP_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DEPSFLAGS) -c -o $@ $< $(LIBS)

clean:
	$(RM) -rf $(BIN_DIR) $(OBJ_DIR)