APP_NAME = jacobi
MPI_APP_NAME = jacobi_mpi

LIBS = -lm -lgsl -lgslcblas
CFLAGS = -Wall -Werror -O3 -I src/jacobi -I src/jacobi_mpi
ifeq ($(shell uname), Darwin) 
	LIBS += -L/opt/homebrew/lib
	CFLAGS += -I/opt/homebrew/include
endif
	
DEPSFLAGS = -MMD
CC = gcc
MPICC = mpicc

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

APP_PATH = $(BIN_DIR)/$(APP_NAME)
APP_SOURCES = $(wildcard $(SRC_DIR)/$(APP_NAME)/*.c)
APP_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(APP_SOURCES))

MPI_APP_PATH = $(BIN_DIR)/$(MPI_APP_NAME)
MPI_APP_SOURCES = $(wildcard $(SRC_DIR)/$(MPI_APP_NAME)/*.c)
MPI_APP_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(MPI_APP_SOURCES))

DEPS = $(APP_OBJECTS:.o=.d) $(MPI_APP_OBJECTS:.o=.d)


.PHONY: all clean
all: make-dir $(APP_PATH) $(MPI_APP_PATH)
-include $(DEPS)

make-dir:
	@mkdir -p bin obj obj/src obj/src/jacobi obj/src/jacobi_mpi

$(APP_PATH): $(APP_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DEPSFLAGS) -c -o $@ $<

$(MPI_APP_PATH): $(MPI_APP_OBJECTS)
	$(MPICC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/$(SRC_DIR)/$(MPI_APP_NAME)/%.o: $(SRC_DIR)/$(MPI_APP_NAME)/%.c
	$(MPICC) $(CFLAGS) $(DEPSFLAGS) -c -o $@ $<

clean:
	$(RM) -rf $(BIN_DIR) $(OBJ_DIR)