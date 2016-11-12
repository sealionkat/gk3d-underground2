all: main

main: main 
	g++ project/main.cpp -o underground --std=c++11 -Wall -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lassimp

clean: 
	rm underground
