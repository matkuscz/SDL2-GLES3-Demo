/*
 * Basic Open GL ES 3 Project
 * Based on amazing tutorial by 
 * https://keasigmadelta.com/store/wp-content/uploads/2017/03/GLES3-and-SDL2-Tutorials.pdf
 *
 */


#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>

#include "Shader.h"

/**
 * Encapsulates data for single vertex
 * Must match the vertex shader's input
 */
typedef struct Vertex_s {
	float position[2];
} Vertex;

const unsigned int DISP_WIDTH = 640;
const unsigned int DISP_HEIGHT = 480;


GLuint vboCreate(const Vertex * vertices, GLuint numVertices);
void vboFree(GLuint vbo);

int main(int argc, const char * argv[]) {
	SDL_Window * window = NULL;
	SDL_GLContext context = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL could not init: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Log("SDL Init OK");

	atexit(SDL_Quit);

	// Request OpenGL ES 3.0
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Double buff
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Log("GL attribs set [OK]");


	// Window
	window = SDL_CreateWindow("GLES3+SDL2 Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DISP_WIDTH, DISP_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	SDL_Log("Window created [OK]");

	if(!window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create the main window.", NULL);
		return EXIT_FAILURE;
	}

	context = SDL_GL_CreateContext(window);

	SDL_Log("GL context created [OK]");

	if(!context) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create an OpenGL context.", NULL);
		return EXIT_FAILURE;
	}



	// OpenGL fun starts here !!!!!
	
	// Clear screen to black
	glClearColor(0.3f, 0.1f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_Log("Color ready...");

	// Update window
	SDL_GL_SwapWindow(window);
	SDL_Log("SDL GL Window swap [OK]");

	// Load shader
	GLuint shaderProg = shaderProgLoad("Simple2D.vert", "Simple2D.frag");

	if(!shaderProg) {
		return EXIT_FAILURE;
	}

	SDL_Log("Shader load [OK]");

	// Any draw calls from now will use this shader
	glUseProgram(shaderProg);


	SDL_Log("Shader init [OK]");

	// Create triangle
	const Vertex vertices[] = {
		{ 0.0f, -0.9f },
		{ 0.9f, 0.9f },
		{ -0.9f, 0.9f }
	};

	GLsizei vertSize = sizeof(vertices[0]);
	GLsizei numVertices = sizeof(vertices) / vertSize;
	GLuint triangleVBO = vboCreate(vertices, numVertices);

	if(!triangleVBO) {
		return EXIT_FAILURE;
	}


	SDL_Log("Create VBO's [OK]");

	// Setup for rendering the triangle ( activate the VBO )
	GLuint positionIdx = 0; // Position is vertex attribute with index 0
	// We know that positionIdx is 0 because the vertex shader Simple2D.vert has only one vertex input !!!!!!!!!!!
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glVertexAttribPointer(positionIdx, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
	glEnableVertexAttribArray(positionIdx);

	SDL_Log("GL Buffer setup [OK]");
	/*
	 * glBindBuffer() is called, which tells OpenGL that subsequent calls to glVertexAttribPointer()should use triangleVBO. Thus, the glVertexAttribPointer() call sets up the position (positionIdx) to be read from triangleVBO starting at offset 0 (the last parameter: (const GLvoid*)0), with a stride between vertices of sizeof(Vertex) bytes. Finally, the position attribute is enabled with glEnableVertexAttribArray()
	 */

	// Finally, we can draw !!!!!
	SDL_GL_SwapWindow(window);
	SDL_Log("Final window swap [OK]");





	// User input handling
	bool quit = false;

	while(!quit) {
		SDL_Event event;

		if(SDL_WaitEvent(&event) != 0) {
			if(event.type == SDL_QUIT) {
				quit = true;
			}
		}
	}



	// Cleanup
	//vboFree(triangleVBO);
	//triangleVBO = 0;
	//shaderProgDestroy(shaderProg);
	//shaderProg = 0;

	return EXIT_SUCCESS;
}

/** Create Vertex Buffer Object (VBO) containing given vertices
 *
 * @param vertices - pointer to array of vertices
 * @param numVertices - number of vertices in that array
 */
GLuint vboCreate(const Vertex * vertices, GLuint numVertices) {
	GLuint vbo;
	int nBuffers = 1;
	glGenBuffers(nBuffers, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Copy vertex data in and deactivate
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Check for problems
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		glDeleteBuffers(nBuffers, &vbo);
		SDL_Log("Creating VBO failed, code: %u\n", err);
		vbo = 0;
	}

	return vbo;
}

/** Free VBO's
 * @param vbo - VBO's name
 */
void vboFree(GLuint vbo) {
	glDeleteBuffers(1, &vbo);
}


