// Shader.h

#ifndef __SHADER_H__
#define __SHADER_H__

#include <GLES3/gl3.h>

/** Loads a vertex and fragment shaders from disk and compiles and links them into shader program
 *
 * @param vertFilename filename of vertex shader
 * @param fragFilename filename of fragment shader
 *
 * @return GLuint - shader program's ID, or 0 if failed
 */
GLuint shaderProgLoad(const char * vertFilename, const char * fragFilename);

/**
 * Destroys a shader program
 */
void shaderProgDestroy(GLuint shaderProg);

#endif
