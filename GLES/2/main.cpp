#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>

const unsigned int DISP_WIDTH = 640;
const unsigned int DISP_HEIGHT = 480;

int main(int argc, const char * argv[]) {
//	SDL_main(argc, argv);
//}

//int SDL_main(int argc, const char * argv[]) {
	SDL_Window * window = NULL;
	SDL_GLContext context = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL could not init: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	atexit(SDL_Quit);

	// Request OpenGL ES 3.0
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Double buff
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);



	// Window
	window = SDL_CreateWindow("GLES3+SDL2 Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DISP_WIDTH, DISP_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if(!window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create the main window.", NULL);
		return EXIT_FAILURE;
	}

	context = SDL_GL_CreateContext(window);

	if(!context) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create an OpenGL context.", NULL);
		return EXIT_FAILURE;
	}



	// OpenGL fun starts here !!!!!
	
	// Clear screen to black
	glClearColor(0.3f, 0.1f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Update window
	SDL_GL_SwapWindow(window);



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

	return EXIT_SUCCESS;
}

