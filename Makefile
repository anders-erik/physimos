exe := build/glthrowsim
#.PRECIOUS: glthrowsim
SRC_DIR := src
BUILD_DIR := build

CXX := g++
# -MMD generates dependencies for user header files, excluding system headers.
# -MP prevents issues if header files are deleted by creating dummy rules.
CXXFLAGS := -g -Wall -lglfw3 -MMD -MP #-lpthread  -ldl -lGL -lX11 -lXrandr -lXi

INC   := -I./include
LIB := -L./lib
SRCS := glad.c main.cpp Input.cpp ui.cpp shader.cpp Simulation.cpp Camera.cpp Windowing.cpp WorldObject.cpp WorldRenderer.cpp Model.cpp bmp_loader.cpp Timing.cpp WorldScene.cpp obj_loader.cpp
# OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
# OBJS := $(patsubst %, $(BUILD_DIR)%, $(SRCS))
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.cpp.o,$(filter %.cpp,$(SRCS))) \
        $(patsubst %.c,$(BUILD_DIR)/%.c.o,$(filter %.c,$(SRCS)))
# SRCS := $(patsubst %, $(SRC_DIR)%, $(SRCS))
DEPS = $(OBJS:.o=.d) # dependency files


INC_HPP := shader.hpp Simulation.hpp Types.hpp Camera.hpp Input.hpp WorldObject.hpp ui.hpp uiElement.hpp Model.hpp bmp_loader.hpp Windowing.hpp WorldRenderer.hpp Timing.hpp WorldScene.hpp obj_loader.hpp PSO_util.hpp
INC_HPP := $(patsubst %, $(SRC_DIR)/%, $(INC_HPP))
# OBJS := main.o


# $(exe): $(SRCS) $(INC_HPP)
# 	$(CXX) $(LIB) $(INC) $(SRCS) $(CXXFLAGS) -o $(exe) 
#	./$(exe)

# Rules
all: $(exe)

# Linking step
$(exe): $(OBJS)
	$(CXX) $(OBJS) $(LIB) $(CXXFLAGS) -o $(exe)

# Compilation step
$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp # NO we need to recompile all cpp's that depend on the specific header that was changed !! #$(SRC_DIR)/%.hpp # I guess this works? #$(INC_HPP) # all cpp depend on all headers...
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# # BUILD OBJ_LOADER
# $(BUILD_DIR)/obj_loader.cpp.o: src/obj_loader.cpp src/obj_loader.hpp # NO we need to recompile all cpp's that depend on the specific header that was changed !! #$(SRC_DIR)/%.hpp # I guess this works? #$(INC_HPP) # all cpp depend on all headers...
# 	@mkdir -p $(BUILD_DIR)
# 	$(CXX) $(CXXFLAGS) $(INC) -c $(SRC_DIR)/obj_loader.cpp -o $(BUILD_DIR)/obj_loader.cpp.o
# 	$(OBJS) := $(OBJS) $(BUILD_DIR)/obj_loader.cpp.o

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@


-include $(DEPS)

.PHONY: run build

build : $(SRCS) $(INC_HPP)
	$(CXX) $(LIB) $(INC) $(SRCS) -o $(exe) $(CXXFLAGS)

run: $(exe)
	./$(exe)

clean:
	rm -rf $(BUILD_DIR)/*


# OLD VERSION
#
# main: main.cpp
# 	g++ -g -Wall -L./lib -I./include main.cpp glad.c -o main -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
# 	./main