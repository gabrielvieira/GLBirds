####################################################################
# make commands:
#	* make						- non-verbose, release build (-O3)
#	* make debug				- non-verbose, debug build (-g)
#	* make clean				- removes .o and .d files
#	* make cleaner				- removes .o and .d files, and exe file
#	* make verbose=true			- verbose mode (prints all compile information), release build
#	* make debug verbose=true	- verbose mode, debug build
####################################################################


####################################################################
#### Modify any of these settings to suit your specific project ####
####################################################################

# Name of executable to be created
BIN := GLBirds

# Path to put the executable relative to makefile
BIN_PATH := ..

# Compiler to be used for compiling object files
CC := gcc

# Linker to be used for linking object files
LD := gcc

# Extension of source files used in the project
SRC_EXT := c

# Path of source folder relative to makefile
SRC_PATH := ../src

# Path of folder to place object files, relative to makefile
OBJ_PATH := .build

# Additional include paths
INCLUDES := -I../../libs/include # Add this if you haven't installed SOIL/glew

# Additional lib paths
LIBS := -L../../libs/lib # Add this if you haven't installed SOIL/glew

# General compile flags
$ CFLAGS := -Wall -Wfatal-errors -Wimplicit-function-declaration -Wextra -Werror -std=c99 -D_LINUX
CFLAGS := -std=c11 -D_LINUX

# General linker flags
LDFLAGS := -lm -lGL -lGLU -lglut -lGLEW -lSOIL

# Debug specific compile flags
DBG_CFLAGS := -g

# Release specific compile flags
RLS_CFLAGS := -O3





###############################################################
#### You shouldn't need to modify anything below this line ####
###############################################################

# Remove command for cleaning
RM := rm -f

# Shell used in this makefile
SHELL := /bin/bash

# Verbose option, to output compile and link commands
export verbose = false
export CMD_PREFIX = @
ifeq ($(verbose),true)
	CMD_PREFIX = 
endif

# Source files
SOURCES := $(shell find $(SRC_PATH)/ -name '*.$(SRC_EXT)')

# Object files, mimicking src sub-folder structure, without added object path
OBJECTS := $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=%.o)

# Temporary folders created for the object files, mimicking structure of source files
DIRS := $(addprefix $(OBJ_PATH)/, $(filter-out ./, $(dir $(OBJECTS))))

# Add object file path, now that temporary folder info has been created
OBJECTS := $(OBJECTS:%.o=$(OBJ_PATH)/%.o)

# Dependency files
DEPS := $(OBJECTS:.o=.d)

# Release build
.PHONY: release
release: CFLAGS += $(RLS_CFLAGS)
release: all

# Debug build
.PHONY: debug
debug: CFLAGS += $(DBG_CFLAGS)
debug: all

# Standard compilation command
.PHONY: all
all: dirs $(BIN_PATH)/$(BIN)

# Link command for the exe, will compile object files first if necessary
$(BIN_PATH)/$(BIN) : $(OBJECTS)
	@echo linking $(BIN_PATH)/$(BIN)
	$(CMD_PREFIX)$(LD) -o $(BIN_PATH)/$(BIN) $(OBJECTS) $(LIBS) $(LDFLAGS)

# Removes all build files
.PHONY: clean
clean:
	@echo "Removing dependencies"
	$(CMD_PREFIX)$(RM) $(DEPS)
	@echo "Removing object files"
	$(CMD_PREFIX)$(RM) $(OBJECTS)
ifneq ($(strip $(DIRS)),)
	@echo "Removing folders"
	$(CMD_PREFIX)rm -rf $(DIRS)
endif

# Removes all build files and binary
.PHONY: cleaner
cleaner: clean
	@echo "Removing executable"
	$(CMD_PREFIX)$(RM) $(BIN_PATH)/$(BIN)

# All folders that need to be created for the object files
.PHONY: dirs
dirs:
	@echo "Creating folders"
ifneq ($(strip $(DIRS)),)
	@mkdir -p $(DIRS)
endif

# Lists all source files
.PHONY: echosrc
echosrc:
	@echo "Listing source files"
	@echo $(SOURCES)

# Lists all object files
.PHONY: echoobj
echoobj:
	@echo "Listing object files"
	@echo $(OBJECTS)

# Lists all dependencies, can only be done after the dependencies have been generated (ie compiled)
.PHONY: echodeps
echodeps:
	@echo "Listing dependencies"
	@cat $(DEPS)

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
$(OBJECTS): $(OBJ_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CMD_PREFIX)$(CC) $(INCLUDES) $(CFLAGS) -MP -MMD -c $< -o $@
