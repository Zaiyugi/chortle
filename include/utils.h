/* Utilities
 */

#ifndef __UTILITES_H__
#define __UTILITES_H__

#include <iostream>
#include <fstream>
#include <vector>

#include "ogl_include.h"

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line);

// Logging
void printShaderInfoLog(GLuint obj);
void printProgramInfoLog(GLuint obj);

// GLSL Shader utils
GLuint loadGLSLShader(std::string file, GLenum shader_type);
void releaseGLSLShader(GLuint &program_id, GLuint &shader_id);
GLint getUniformLocation(GLuint &program_id, const std::string& _uniformName);

#endif
