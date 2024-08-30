exe := build/glthrowsim
#.PRECIOUS: glthrowsim
src := src/

CXX := g++
CXXFLAGS := -g -Wall -lglfw3  #-lpthread  -ldl -lGL -lX11 -lXrandr -lXi

INC   := -I./include
LIB := -L./lib
SRCS := main.cpp glad.c
SRCS := $(patsubst %, $(src)%, $(SRCS))


INC_HPP := shader.hpp Simulation.hpp Types.hpp Camera.hpp Input.hpp WorldObject.hpp ui.hpp uiElement.hpp vertex.hpp model.hpp
INC_HPP := $(patsubst %, $(src)%, $(INC_HPP))
# OBJS := main.o


$(exe): $(SRCS) $(INC_HPP)
	$(CXX) $(LIB) $(INC) $(SRCS) -o $(exe) $(CXXFLAGS)
#	./$(exe)



.PHONY: run build

build : $(SRCS) $(INC_HPP)
	$(CXX) $(LIB) $(INC) $(SRCS) -o $(exe) $(CXXFLAGS)

run: $(exe)
	./$(exe)


# OLD VERSION
#
# main: main.cpp
# 	g++ -g -Wall -L./lib -I./include main.cpp glad.c -o main -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
# 	./main