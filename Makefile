exe := build/glthrowsim
#.PRECIOUS: glthrowsim
SRC_DIR := src
BUILD_DIR := build

CXX := g++
CXXFLAGS := -g -Wall -lglfw3  #-lpthread  -ldl -lGL -lX11 -lXrandr -lXi

INC   := -I./include
LIB := -L./lib
SRCS := glad.c main.cpp Input.cpp ui.cpp shader.cpp Simulation.cpp Camera.cpp Windowing.cpp WorldObject.cpp WorldRenderer.cpp Model.cpp bmp_loader.cpp Timing.cpp WorldScene.cpp
# OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
# OBJS := $(patsubst %, $(BUILD_DIR)%, $(SRCS))
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.cpp.o,$(filter %.cpp,$(SRCS))) \
        $(patsubst %.c,$(BUILD_DIR)/%.c.o,$(filter %.c,$(SRCS)))
# SRCS := $(patsubst %, $(SRC_DIR)%, $(SRCS))


INC_HPP := shader.hpp Simulation.hpp Types.hpp Camera.hpp Input.hpp WorldObject.hpp ui.hpp uiElement.hpp Model.hpp bmp_loader.hpp Windowing.hpp WorldRenderer.hpp Timing.hpp WorldScene.hpp
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
$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp $(INC_HPP) # NO we need to recompile all cpp's that depend on the specific header that was changed !! #$(SRC_DIR)/%.hpp # I guess this works? #$(INC_HPP) # all cpp depend on all headers...
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@


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