/* Utilities
 */

#include "utils.h"

int printOglError(char *file, int line)
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %x\n", file, line, glErr);
        retCode = 1;
    }
    return retCode;
}

// Utility

void printShaderInfoLog(GLuint obj)
{
   GLint status = 0;
   glGetShaderiv(obj, GL_COMPILE_STATUS, &status);

   if(status == GL_FALSE)
   {
      GLint maxLength = 0;
      glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &maxLength);

      //The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(obj, maxLength, &maxLength, &infoLog[0]);

      for(int i = 0; i < maxLength; ++i)
         std::cout << infoLog[i];
      std::cout << std::endl;
   }
}

void printProgramInfoLog(GLuint obj)
{
   GLint status = 0;
   glGetProgramiv(obj, GL_LINK_STATUS, &status);
   if(status == GL_FALSE)
   {
      GLint maxLength = 0;
      glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &maxLength);

      //The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(obj, maxLength, &maxLength, &infoLog[0]);

      for(int i = 0; i < maxLength; ++i)
         std::cout << infoLog[i];
      std::cout << std::endl;
   }
}

// GLSL Shader loader
GLuint loadGLSLShader(std::string file, GLenum shader_type)
{
   GLuint shader_id = glCreateShader(shader_type);

   std::ifstream infile(file.c_str());
   std::string shader_code;
   std::getline(infile, shader_code, '\0');
   infile.close();

   // Compile, link and debug shader
   // std::cout << shader_code.c_str() << std::endl;
   const char* c_str = shader_code.c_str();
   glShaderSource(shader_id, 1, &c_str, NULL);
   glCompileShader(shader_id);
   printShaderInfoLog(shader_id);

return shader_id;
}

void releaseGLSLShader(GLuint &program_id, GLuint &shader_id)
{
   glDetachShader(program_id, shader_id);
   glDeleteShader(shader_id);
}

GLint getUniformLocation(GLuint &program_id, const std::string& _uniformName)
{
    GLint loc = glGetUniformLocation(program_id, _uniformName.c_str());
    if(loc == -1){
        // std::cerr << "Uniform " << _uniformName << " not found" << std::endl;
    }
    return loc;
}
