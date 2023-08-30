APP = EasyLocal
DEBUG =  -g -fstack-protector -Wall -Wfatal-errors 
RELEASE = -Ofast  -DNDEBUG -funroll-loops -march=native -frename-registers

# Uncomment if you want to make graph plots
#CFLAGS += -DVISUALIZE

# Uncomment if you want to display accepted moves
#CFLAGS += -DPRINTOBJECTIVE

# TODO TODO Gnu profiler
# Requires debug mode!
#DEBUG += -pg
#LDFLAGS += -pg


MODE = ${RELEASE}

# TODO TODO
# Only enable if you want to solve uncapacitated problem!!
# ODOT ODOT
#CFLAGS += -DUNCAPACITATED


# Path where easy local software is installed
EASYLOCAL = easylocal-3/

# Compilation flags for debug mode or release node
# HPC does not know the c++20 flag
#CFLAGS += ${MODE} -std=c++20 -I$(EASYLOCAL)/include
CFLAGS += ${MODE} -std=c++2a -I$(EASYLOCAL)/include

# Linking flags
LDFLAGS += -lboost_program_options -lpthread

CC = g++
LD = g++

# Use all threads to compile in parallel
MAKEFLAGS += -j 8

# Scan all subdirectories for cpp files, make build dir for *.o files
MODULES   := $(shell ls ./source/)
SRC_DIR   := $(addprefix source/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst source/%.cpp,build/%.o,$(SRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	@echo "[CC] $<"
	$(CC) $(CFLAGS) $(MODE) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs build/$(APP)

build/$(APP): $(OBJ) 
	@echo "[LD] $<"
	$(LD) $^ $(LDFLAGS) -o $(APP)

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)
	@rm $(APP)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
