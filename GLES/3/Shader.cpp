// Shader.cpp

#include "Shader.h"

#include <cstdio>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL_opengles2.h>

#ifdef _MSC_VER
#pragma warning(disable:4996) // Allow us to use the portable fopen() function without warnings
#endif

/**
 * Gets the file's length in bytes
 * @param file - the file
 * @return size_t the file's length in bytes
 */
static size_t fileGetLength(FILE * file) {
	size_t length;
	size_t currPos = ftell(file);

	fseek(file, 0, SEEK_END);
	length = ftell(file);

	// Return file to its previous position
	fseek(file, currPos, SEEK_SET);

	return length;
}


/** Load and compile shader from file
 *
 */
static GLuint shaderLoad(const char * filename, GLenum shaderType) {
	FILE * file = fopen(filename, "r");
	if(!file) {
		SDL_Log("Can't open file: %s\n", filename);
		return 0;
	}

	size_t length = fileGetLength(file);

	// Alloc space for the file + '\0'
	GLchar * shaderSrc = (GLchar *)calloc(length + 1, 1);

	if(!shaderSrc) {
		SDL_Log("Out of memory when reading file: %s\n", filename);
		fclose(file);
		file = NULL;

		return 0;
	}

	fread(shaderSrc, 1, length, file);

	fclose(file);
	file = NULL;

	// Create shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, (const GLchar**)&shaderSrc, NULL);

	free(shaderSrc);
	shaderSrc = NULL;

	// Compile shader
	glCompileShader(shader);
	GLint compileSucceeded = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSucceeded);

	if(!compileSucceeded) {
		SDL_Log("Compilation of shader %s failed\n", filename);
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar * errLog = (GLchar*)malloc(logLength);
		
		if(errLog) {
			glGetShaderInfoLog(shader, logLength, &logLength, errLog);
			SDL_Log("%s\n", errLog);
			free(errLog);
		}
		else {
			SDL_Log("Couldn't get shader log; out of memory.\n");
		}

		glDeleteShader(shader);
		shader = 0;
	}

	return shader;
}

/**
 * Destroy a shader
 */
static void shaderDestroy(GLuint shaderID) {
	glDeleteShader(shaderID);
}

GLuint shaderProgLoad(const char * vertFilename, const char * fragFilename) {
	GLuint vertShader = shaderLoad(vertFilename, GL_VERTEX_SHADER);
	if(!vertShader) {
		SDL_Log("Couldn't load vertex shader: %s\n", vertFilename);
		return 0;
	}

	GLuint fragShader = shaderLoad(fragFilename, GL_FRAGMENT_SHADER);
	if(!fragShader) {
		SDL_Log("Couldn't load fragment shader: %s\n", fragFilename);
		shaderDestroy(vertShader);
		vertShader = 0;

		return 0;
	}

	GLuint shaderProg = glCreateProgram();
	if(shaderProg) {
		glAttachShader(shaderProg, vertShader);
		glAttachShader(shaderProg, fragShader);

		glLinkProgram(shaderProg);

		GLint linkingSucceeded = GL_FALSE;
		glGetProgramiv(shaderProg, GL_LINK_STATUS, &linkingSucceeded);

		if(!linkingSucceeded) {
			SDL_Log("Linking shader failed. Vert->%s Frag->%s\n", vertFilename, fragFilename);
			GLint logLength = 0;
			glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &logLength);
			GLchar * errLog = (GLchar*)malloc(logLength);
			if(errLog) {
				glGetProgramInfoLog(shaderProg, logLength, &logLength, errLog);
				SDL_Log("%s\n", errLog);
				free(errLog);
			}
			else {
				SDL_Log("Couldn't get shader link log, out of memory\n");
			}

			glDeleteProgram(shaderProg);
			shaderProg = 0;
		}
	}
	else {
		SDL_Log("Couldn't create shader program\n");
	}

	// We can now destroy shaders
	shaderDestroy(vertShader);
	shaderDestroy(fragShader);

	return shaderProg;
}

void shaderProgDestroy(GLuint shaderProg) {
	glDeleteProgram(shaderProg);
}
