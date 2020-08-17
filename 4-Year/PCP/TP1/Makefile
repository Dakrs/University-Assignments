CC = gcc
CFLAGS = -O3 -std=c99 -fopenmp -lm -lpthread -lpapi -L/share/apps/papi/5.4.1/lib -I/share/apps/papi/5.4.1/include

INCLUDES = -I include

SRC := src
OBJ := obj

NAME = sparse

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJECTS)
	@echo  Compiled without errors

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -r $(OBJ)
	$(RM) $(NAME)

$(shell   mkdir -p $(OBJ))