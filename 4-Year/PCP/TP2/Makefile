CC = mpicxx
CFLAGS = -O3 -g

INCLUDES = -I include

SRC := src
OBJ := obj

NAME = COO_STC CRS_DYN CRS_STC

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

ALL_MAINS=obj/crs_dynamic.o obj/crs_stattic.o obj/coo_static_main.o

OBJECTS_WITHOUT_MAINS := $(filter-out $(ALL_MAINS),$(OBJECTS))


CRS_DYN_TARGET=crs_dynamic
CRS_STC_TARGET=crs_stattic

COO_STC_TARGET=coo_static_main

all: CRS_DYN CRS_STC COO_STC

CRS_DYN: $(OBJECTS_WITHOUT_MAINS) 
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/$(CRS_DYN_TARGET).cpp -o $(OBJ)/$(CRS_DYN_TARGET).o
	$(CC) $(CFLAGS) $(INCLUDES) -o CRS_DYN $(OBJECTS_WITHOUT_MAINS) $(OBJ)/$(CRS_DYN_TARGET).o
	@echo  CRS DYN Compiled without errors

CRS_STC: $(OBJECTS_WITHOUT_MAINS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/$(CRS_STC_TARGET).cpp -o $(OBJ)/$(CRS_STC_TARGET).o
	$(CC) $(CFLAGS) $(INCLUDES) -o CRS_STC $(OBJECTS_WITHOUT_MAINS) $(OBJ)/$(CRS_STC_TARGET).o
	@echo  CRS STC Compiled without errors

COO_STC: $(OBJECTS_WITHOUT_MAINS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/$(COO_STC_TARGET).cpp -o $(OBJ)/$(COO_STC_TARGET).o
	$(CC) $(CFLAGS) $(INCLUDES) -o COO_STC $(OBJECTS_WITHOUT_MAINS) $(OBJ)/$(COO_STC_TARGET).o
	@echo  COO STC Compiled without errors

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -r $(OBJ)
	$(RM) $(NAME)

$(shell   mkdir -p $(OBJ))
