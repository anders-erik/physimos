exe := glthrowsim
#.PRECIOUS: glthrowsim

CXX := g++
CXXFLAGS := -g -Wall -lglfw3  #-lpthread  -ldl -lGL -lX11 -lXrandr -lXi

INC   := -I./include
LIB := -L./lib
SRCS := main.cpp glad.c
# OBJS := main.o


$(exe): $(SRCS)
	$(CXX) $(LIB) $(INC) $(SRCS) -o $(exe) $(CXXFLAGS)
#	./$(exe)



.PHONY: run
run: $(exe)
	./$(exe)


# OLD VERSION
#
# main: main.cpp
# 	g++ -g -Wall -L./lib -I./include main.cpp glad.c -o main -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
# 	./main