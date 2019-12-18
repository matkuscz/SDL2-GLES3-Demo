pokus:
	gcc Shader.cpp -c -lSDL2 -lGL
	gcc main.cpp -c
	gcc main.o Shader.o -lSDL2 -lGL -o pokus

.PHONY:
	pokus clean
	
clean:
	rm Shader.o main.o pokus
