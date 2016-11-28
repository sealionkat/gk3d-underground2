all: main

main:
	g++ project/main.cpp project/Perlin.cpp -o underground --std=c++11 -Wall -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lassimp -lSOIL -lnoise

clean: 
	rm underground
