main: main.cpp
	g++ -Wall -L./lib -I./include main.cpp glad.c -o main -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
	./main