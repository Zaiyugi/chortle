#ifndef __CUDAGLRENDERER__
#define __CUDAGLRENDERER__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <random>
#include <memory>

#include <cmath>
#include <ctime>

#include "ogl_include.h"
#include "utils.h"

#include "math_functions.h"   // Anton's maths functions.

#include "DataObjectDefs.h"
#include "cackle/include/RenderData.h"

#ifdef __CUDACC__

#include <cuda.h>

#endif

#define _USE_MATH_DEFINES

namespace akasha
{

#ifndef __CUDACC__

struct cudaGraphicsResource;

#endif

struct CameraOptions
{
   float dx;
   float dy;
   float dz;

   float move_speed;

   bool look_enabled;
   bool zoom_enabled;
   bool pan_enabled;

   double current_elev;
   double current_azim;
   double delta_elev;
   double delta_azim;

   double start_mouse_x;
   double start_mouse_y;
   double last_mouse_x;
   double last_mouse_y;

   vec3 prev_mouse_pos;
};

class CudaGLRenderer
{

   public:

      static const int OGL_PRESS = 0;
      static const int OGL_RELEASE = 1;
      static const int OGL_MOUSE_BUTTON_NONE = 0;
      static const int OGL_MOUSE_BUTTON_LEFT = 1;
      static const int OGL_MOUSE_BUTTON_RIGHT = 2;
      static const int OGL_MOUSE_BUTTON_MIDDLE = 3;

      CudaGLRenderer() {}

      CudaGLRenderer(int w, int h) :
         w_width(w), w_height(h), fps(0), flag_fps_camera(false),
         full_tex_width(1920), full_tex_height(1080),
         default_vert_shader_file("./glsl/default.vert"),
         default_frag_shader_file("./glsl/default.frag"),
         camera(nullptr), uniforms(nullptr), renderbuffer_CUDA(nullptr)
      {
      }

      ~CudaGLRenderer()
      {
         uninit();
      }

      void draw();

      void init();
      void uninit();

      void launchKernel();

      void RotateX(vec3 &v, double degree);
      void RotateY(vec3 &v, double degree);
      void ArbitraryRotate(vec3 U, vec3 V, vec3 W, double degreeX, double degreeY, vec3& point, vec3 aim);

      void createVertexStorage();
      void loadShaderFiles();
      void loadGLSLRenderingProgram();
      void reloadRaymarchProgram();

      /* Callbacks */
      void ui_errorCallback(int error, const char* description);
      void ui_reshapeCallback(int w, int h);
      void ui_keyCallback(int key, int action, int mods);
      void ui_cursorPosCallback(double xpos, double ypos);
      void ui_mouseButtonCallback(int button, int action, int mods, int wx, int wy);
      void ui_scrollCallback(double xoffset, double yoffset);

      /* Uniforms */
      void setFractalIterations(int fi);
      void setRaymarchIterations(int ri);

      void setFogLevel(float fog);
      void setFogColor(float r, float g, float b);

      void setEpsilon(float eps);
      void setScale(float scale);
      void setMR2(float mr2);
      void setDither(float dither);
      void setFudgeFactor(float fudgefactor);

      void setGlowStrength(float glow);
      void setSpecularExponent(float spec);
      
   private:

      void initGL();

      std::unique_ptr<Camera> camera;
      std::unique_ptr<Uniforms> uniforms;
      CameraOptions camera_options;
      
      cudaGraphicsResource* renderbuffer_CUDA;

      int w_width;
      int w_height;
      double fps;

      bool flag_fps_camera;

      GLdouble mvMatrix[16];
      GLdouble projMatrix[16];
      GLint viewPort[4];

      GLuint v_position_vbo;
      GLuint v_normal_vbo;
      GLuint v_texcoord_vbo;

      GLuint rendering_program;
      GLuint renderbuffer;
      GLuint full_framebuffer;
      int full_tex_width;
      int full_tex_height;

      std::string default_vert_shader_file;
      std::string default_frag_shader_file;
      std::string default_vert_code;
      std::string default_frag_code;

};

}

#endif
