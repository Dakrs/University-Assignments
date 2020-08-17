
CFLAGS = -O3

ifeq ($(CUDA),yes)
	CXX = nvcc
	NAME = GPU_AA
	CFLAGS += -DD_GPU
else
ifeq ($(INTEL),yes)
	CXX = icpc
	CFLAGS += -std=c++11 -mavx -g -qopenmp -qopt-report5 -no-vec
else
	CXX = g++
endif
ifeq ($(KNL),yes)
	NAME = CPU_AA
	CFLAGS += -qop-report-phase=vec -march=knl
else
	NAME = CPU_AA
	CFLAGS += -DD_CPU -fopenmp -lm -lpthread -lpapi -L/share/apps/papi/5.5.0/lib -I/share/apps/papi/5.5.0/include
endif
endif

INCLUDES = -I include

SRC := src
OBJ := obj

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

ifeq ($(CUDA),yes)

OBJECTS := $(filter-out $(OBJ)/mypapi.o,$(OBJECTS))
SOURCES_GPU := $(wildcard $(SRC)/*.cu)
OBJECTS += $(patsubst $(SRC)/%.cu, $(OBJ)/%.o, $(SOURCES_GPU))

$(OBJ)/%.o: $(SRC)/%.cu
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

endif

ifeq ($(KNL),yes)

OBJECTS := $(filter-out $(OBJ)/mypapi.o,$(OBJECTS))

endif

all: checkdirs BUILD

BUILD: $(OBJECTS)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJECTS)
	@echo  Compiled without errors

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

checkdirs:
	@mkdir -p $(OBJ)

clean:
	$(RM) -r $(OBJ)
	$(RM) CPU_AA GPU_AA
