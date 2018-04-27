#ifndef __OGLRENDERER_H__
#define __OGLRENDERER_H__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <random>

#include <cmath>
#include <ctime>

#include "ogl_include.h"
#include "utils.h"

#include "math_functions.h"   // Anton's maths functions.

#define _USE_MATH_DEFINES

namespace akasha
{

struct Camera
{
   vec3 eye;
   vec3 aim;

   vec3 view;
   vec3 up;
   vec3 right;
   float fov;

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

struct Uniforms
{
   int fractal_iterations;
   int raymarch_iterations;
   float flimit_x;
   float flimit_y;
   float flimit_z;
   float julia_C_x;
   float julia_C_y;
   float julia_C_z;
   float julia_offset_x;
   float julia_offset_y;
   float julia_offset_z;
   float de_offset;
   float fog_level;
   float epsilon;
   float scale;
   float min_radius_2;
   float fixed_radius_2;
   float glow_strength;
};

class OGLRenderer
{

   public:

      static const int OGL_PRESS = 0;
      static const int OGL_RELEASE = 1;
      static const int OGL_MOUSE_BUTTON_NONE = 0;
      static const int OGL_MOUSE_BUTTON_LEFT = 1;
      static const int OGL_MOUSE_BUTTON_RIGHT = 2;
      static const int OGL_MOUSE_BUTTON_MIDDLE = 3;

      OGLRenderer() {}

      OGLRenderer(int w, int h) :
         w_width(w), w_height(h), fps(0),
         flag_use_IPR(true), flag_render_next_sample(false), flag_fps_camera(false),
         point_count(0), aa_sample_count(0), max_aa_samples(4),
         full_tex_width(1920), full_tex_height(1080),
         ipr_tex_width(1920), ipr_tex_height(1080),
         default_vert_shader_file("./glsl/default.vert"),
         default_frag_shader_file("./glsl/default.frag"),
         raymarch_shader_file("./glsl/raymarch.frag")
      {
      }

      ~OGLRenderer()
      {
         uninit();
      }

      void draw();

      void init();
      void uninit();

      void RotateX(vec3 &v, double degree);
      void RotateY(vec3 &v, double degree);
      void ArbitraryRotate(vec3 U, vec3 V, vec3 W, double degreeX, double degreeY, vec3& point, vec3 aim);

      void createVertexStorage();
      void loadShaderFiles();
      void loadGLSLRenderingProgram();
      void reloadRaymarchProgram();

      /* Callbacks */
      void errorCallback (int error, const char* description);
      void reshapeCallback(int w, int h);
      void keyCallback(int key, int action, int mods);
      void cursorPosCallback(double xpos, double ypos);
      void mouseButtonCallback(int button, int action, int mods, int wx, int wy);
      void scrollCallback(double xoffset, double yoffset);

      void setRaymarchIterations(int n);
      void setFractalIterations(int n);
      void setFLimitX(double flx);
      void setFLimitY(double fly);
      void setFLimitZ(double flz);

      void setFractalScale(double s);
      void setMR2(double r);
      void setFR2(double r);

      void setJuliaCx(double jcx);
      void setJuliaCy(double jcy);
      void setJuliaCz(double jcz);
      void setJuliaOffsetX(double jofx);
      void setJuliaOffsetY(double jofy);
      void setJuliaOffsetZ(double jofz);

      void setDEOffset(double deof);

      void setGlowStrength(double s);
      void setEpsilon(double s);

      void getConfiguration();

   private:

      Camera camera;
      Uniforms uniforms;

      int w_width;
      int w_height;
      double fps;

      bool flag_use_IPR;
      bool flag_render_next_sample;
      bool flag_fps_camera;

      int point_count;
      int aa_sample_count;
      int max_aa_samples;

      GLdouble mvMatrix[16];
      GLdouble projMatrix[16];
      GLint viewPort[4];

      GLuint v_position_vbo;
      GLuint v_normal_vbo;
      GLuint v_texcoord_vbo;

      GLuint rendering_program;
      GLuint raymarch_program;

      GLuint full_tex_id;
      GLuint full_framebuffer;
      int full_tex_width;
      int full_tex_height;

      GLuint ipr_tex_id;
      GLuint ipr_framebuffer;
      int ipr_tex_width;
      int ipr_tex_height;

      std::string default_vert_shader_file;
      std::string default_frag_shader_file;
      std::string raymarch_shader_file;
      std::string default_vert_code;
      std::string default_frag_code;
      std::string raymarch_frag_code;

};

}

#endif
