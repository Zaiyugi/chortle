#include "OGLRenderer.h"

namespace akasha
{

void OGLRenderer::RotateX(vec3 &v, double degree)
{
   double c = cos(degree * ONE_RAD_IN_DEG);
   double s = sin(degree * ONE_RAD_IN_DEG);
   double v1 = v.v[1] * c - v.v[2] * s;
   double v2 = v.v[1] * s + v.v[2] * c;
   v.v[1] = v1; v.v[2] = v2;
}

void OGLRenderer::RotateY(vec3 &v, double degree)
{
   double c = cos(degree * ONE_RAD_IN_DEG);
   double s = sin(degree * ONE_RAD_IN_DEG);
   double v0 = v.v[0] * c + v.v[2] * s;
   double v2 = -v.v[0] * s + v.v[2] * c;
   v.v[0] = v0; v.v[2] = v2;
}

void OGLRenderer::ArbitraryRotate(vec3 U, vec3 V, vec3 W, double degreeX, double degreeY, vec3& point, vec3 aim)
{
   double cx = cos(degreeX * ONE_RAD_IN_DEG);
   double sx = sin(degreeX * ONE_RAD_IN_DEG);
   double cy = cos(degreeY * ONE_RAD_IN_DEG);
   double sy = sin(degreeY * ONE_RAD_IN_DEG);

   mat4 trans;
   trans.m[0] = 1.0; trans.m[4] = 0.0; trans.m[8]  = 0.0; trans.m[12] = -aim.v[0];
   trans.m[1] = 0.0; trans.m[5] = 1.0; trans.m[9]  = 0.0; trans.m[13] = -aim.v[1];
   trans.m[2] = 0.0; trans.m[6] = 0.0; trans.m[10] = 1.0; trans.m[14] = -aim.v[2];
   trans.m[3] = 0.0; trans.m[7] = 0.0; trans.m[11] = 0.0; trans.m[15] = 1.0;

   mat4 mat;
   mat.m[0] = U.v[0]; mat.m[4] = U.v[1]; mat.m[8]  = U.v[2]; mat.m[12] = 0.0;
   mat.m[1] = V.v[0]; mat.m[5] = V.v[1]; mat.m[9]  = V.v[2]; mat.m[13] = 0.0;
   mat.m[2] = W.v[0]; mat.m[6] = W.v[1]; mat.m[10] = W.v[2]; mat.m[14] = 0.0;
   mat.m[3] = 0.0;    mat.m[7] = 0.0;    mat.m[11] = 0.0;    mat.m[15] = 1.0;

   mat4 rot;
   vec4 pos(point, 1.0f);
   pos = trans*pos;
   pos = mat*pos;

   rot.m[0] = 1.0; rot.m[4] = 0.0; rot.m[8] = 0.0;  rot.m[12] = 0.0;
   rot.m[1] = 0.0; rot.m[5] = cx;  rot.m[9] = sx;   rot.m[13] = 0.0;
   rot.m[2] = 0.0; rot.m[6] = -sx; rot.m[10] = cx;  rot.m[14] = 0.0;
   rot.m[3] = 0.0; rot.m[7] = 0.0; rot.m[11] = 0.0; rot.m[15] = 1.0;

   pos = rot*pos;

   rot.m[0] = cy;  rot.m[4] = 0.0; rot.m[8] = sy;   rot.m[12] = 0.0;
   rot.m[1] = 0.0; rot.m[5] = 1.0; rot.m[9] = 0.0;  rot.m[13] = 0.0;
   rot.m[2] = -sy; rot.m[6] = 0.0; rot.m[10] = cy;  rot.m[14] = 0.0;
   rot.m[3] = 0.0; rot.m[7] = 0.0; rot.m[11] = 0.0; rot.m[15] = 1.0;

   pos = rot*pos;
   pos = inverse(mat)*pos;
   pos = inverse(trans)*pos;
   point = vec3(pos);
}

// Create GL_ARRAY_BUFFER for vertex and color storage
void OGLRenderer::createVertexStorage()
{
   point_count = 6;
   GLfloat vp[18] = {
       1.0,  1.0, 0.0, // v
      -1.0,  1.0, 0.0, // v
       1.0, -1.0, 0.0, // v

       1.0, -1.0, 0.0, // v
      -1.0,  1.0, 0.0, // v
      -1.0, -1.0, 0.0  // v
   };

   GLfloat vn[18] = {
      0.0, 0.0, 1.0, // vn
      0.0, 0.0, 1.0, // vn
      0.0, 0.0, 1.0, // vn

      0.0, 0.0, 1.0, // vn
      0.0, 0.0, 1.0, // vn
      0.0, 0.0, 1.0  // vn
   };

   GLfloat vt[12] = {
      1.0, 1.0, // vt
      0.0, 1.0, // vt
      1.0, 0.0, // vt

      1.0, 0.0, // vt
      0.0, 1.0, // vt
      0.0, 0.0  // vt
   };

   size_t bufsize = sizeof(GLfloat) * point_count * 3;
   glGenBuffers(1, &v_position_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, v_position_vbo);
   glBufferData(GL_ARRAY_BUFFER, bufsize, vp, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   std::cerr << "GL error: " << glGetError() << std::endl;

   glGenBuffers(1, &v_normal_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, v_normal_vbo);
   glBufferData(GL_ARRAY_BUFFER, bufsize, vn, GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
   std::cerr << "GL error: " << glGetError() << std::endl;

   bufsize = sizeof(GLfloat) * point_count * 2;
   glGenBuffers(1, &v_texcoord_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, v_texcoord_vbo);
   glBufferData(GL_ARRAY_BUFFER, bufsize, vt, GL_STATIC_DRAW);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
   std::cerr << "GL error: " << glGetError() << std::endl;
}

void OGLRenderer::loadShaderFiles()
{
   {
      std::ifstream infile(default_vert_shader_file.c_str());
      std::getline(infile, default_vert_code, '\0');
      infile.close();
   }

   {
      std::ifstream infile(default_frag_shader_file.c_str());
      std::getline(infile, default_frag_code, '\0');
      infile.close();
   }

   {
      std::ifstream infile(raymarch_shader_file.c_str());
      std::getline(infile, raymarch_frag_code, '\0');
      infile.close();
   }
}

// GLSL Vertex/Fragment shaders
void OGLRenderer::loadGLSLRenderingProgram()
{
   rendering_program = glCreateProgram();

   // --- Vertex Shader ---
   // Load vertex shader
   GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
   {
      const char* c_str = default_vert_code.c_str();
      glShaderSource(vs_id, 1, &c_str, NULL);
      glCompileShader(vs_id);
      printShaderInfoLog(vs_id);
   }

   // --- Fragment Shader ---
   // Load fragment shader
   GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);
   {
      const char* c_str = default_frag_code.c_str();
      glShaderSource(fs_id, 1, &c_str, NULL);
      glCompileShader(fs_id);
      printShaderInfoLog(fs_id);
   }

   // Attach shaders
   glAttachShader(rendering_program, vs_id);
   glAttachShader(rendering_program, fs_id);

   // Bind vertex shader IO locations
   glBindAttribLocation(rendering_program, 0, "v_position");
   glBindAttribLocation(rendering_program, 1, "v_normal");
   glBindAttribLocation(rendering_program, 2, "v_uv");

   glLinkProgram(rendering_program);
   std::cout << "-----------------------------------" << std::endl;
   std::cout << "Rendering Program log: " << std::endl;
   printProgramInfoLog(rendering_program);
   std::cout << "-----------------------------------" << std::endl;

   releaseGLSLShader(rendering_program, vs_id);
   releaseGLSLShader(rendering_program, fs_id);
}

void OGLRenderer::reloadRaymarchProgram()
{
   if( glIsProgram(raymarch_program) )
      glDeleteProgram(raymarch_program);

   raymarch_program = glCreateProgram();

   // --- Vertex Shader ---
   // Load vertex shader
   GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
   {
      const char* c_str = default_vert_code.c_str();
      glShaderSource(vs_id, 1, &c_str, NULL);
      glCompileShader(vs_id);
      printShaderInfoLog(vs_id);
   }

   // Prepend necessary data
   std::string raymarch_header;
   raymarch_header += "#version 430 \n";
   raymarch_header += "layout(location = 0) out vec4 fragment_color; \n";


   raymarch_header += "#define MAX_ITERATIONS " + std::to_string(uniforms.raymarch_iterations) + " \n";
   raymarch_header += "#define FRACTAL_ITERS " + std::to_string(uniforms.fractal_iterations) + " \n";
   raymarch_header += "#define MAX_AA_SAMPLES 1 \n";
   raymarch_header += "#define NORMAL_AO_SAMPLES 5 \n";
   raymarch_header += "#define SCALE (" + std::to_string(uniforms.scale) + "f) \n";
   raymarch_header += "#define MR2 (" + std::to_string(uniforms.min_radius_2) + ") \n";
   raymarch_header += "#define FR2 (" + std::to_string(uniforms.fixed_radius_2) + ") \n";
   raymarch_header += "#define FLIMIT vec3(" + std::to_string(uniforms.flimit_x) + ", " + std::to_string(uniforms.flimit_y) + ", " + std::to_string(uniforms.flimit_z) + ") \n";
   raymarch_header += "#define JULIA_C vec3(" + std::to_string(uniforms.julia_C_x) + ", " + std::to_string(uniforms.julia_C_y) + ", " + std::to_string(uniforms.julia_C_z) + ") \n";
   raymarch_header += "#define JULIA_OFFSET vec3(" + std::to_string(uniforms.julia_offset_x) + ", " + std::to_string(uniforms.julia_offset_y) + ", " + std::to_string(uniforms.julia_offset_z) + ") \n";
   raymarch_header += "#define DE_OFFSET (" + std::to_string(uniforms.de_offset) + "f) \n";
   raymarch_header += "#define EPSILON (" + std::to_string(uniforms.epsilon) + "f) \n";
   raymarch_header += "#define FOG_LEVEL (" + std::to_string(uniforms.fog_level) + "f) \n";
   raymarch_header += "#define GLOW_STRENGTH (" + std::to_string(uniforms.glow_strength) + "f) \n";

   std::string shader_code = raymarch_header + raymarch_frag_code;

   // std::cout << shader_code.c_str() << std::endl;

   // Compile, link and debug shader
   GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);
   {
      const char* c_str = shader_code.c_str();
      glShaderSource(fs_id, 1, &c_str, NULL);
      glCompileShader(fs_id);
      printShaderInfoLog(fs_id);
   }

   // Attach shaders
   glAttachShader(raymarch_program, vs_id);
   glAttachShader(raymarch_program, fs_id);

   // Bind vertex shader IO locations
   glBindAttribLocation(raymarch_program, 0, "v_position");
   glBindAttribLocation(raymarch_program, 1, "v_normal");
   glBindAttribLocation(raymarch_program, 2, "v_uv");

   glLinkProgram(raymarch_program);
   // std::cout << "-----------------------------------" << std::endl;
   // std::cout << "Rendering Program log: " << std::endl;
   // printProgramInfoLog(raymarch_program);
   // std::cout << "-----------------------------------" << std::endl;

   releaseGLSLShader(raymarch_program, vs_id);
   releaseGLSLShader(raymarch_program, fs_id);
}

/* Callbacks */

void OGLRenderer::reshapeCallback(int w, int h)
{
   w_width = w;
   w_height = h;
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void OGLRenderer::errorCallback (int error, const char* description) {
   fputs (description, stderr);
   std::cerr << description << std::endl;
}

void OGLRenderer::keyCallback(int key, int action, int mods)
{
   /*
   double move_speed = camera.move_speed * 1.0 / fps;//time_since_last_frame;
   switch(key)
   {
      case GLFW_KEY_ESCAPE:
         if( action == GLFW_PRESS )
            glfwSetWindowShouldClose(window, 1);
         break;

      case GLFW_KEY_R:
         if( action == GLFW_PRESS )
         {
            std::cout << "Reloading Raymarching shader..." << std::endl;
            // flag_use_IPR = false;
            // flag_render_next_sample = true;

            // uniforms.fractal_iterations = 12;
            // uniforms.raymarch_iterations = 512;
            // uniforms.fog_level = 0.1;
            // uniforms.epsilon = 0.1e-2;
            reloadRaymarchProgram();
            std::cout << "Reload DONE" << std::endl;
         }
         break;

      case GLFW_KEY_I:
         if( action == GLFW_PRESS )
         {
            flag_use_IPR = true;

            uniforms.fractal_iterations = 12;
            uniforms.raymarch_iterations = 256;
            uniforms.fog_level = 0.1;
            uniforms.epsilon = 0.1e-2;
            reloadRaymarchProgram();
         }
         break;

      case GLFW_KEY_O:
         if( action == GLFW_PRESS )
            flag_fps_camera = !flag_fps_camera;
         break;

      case GLFW_KEY_P:
         if( action == GLFW_PRESS )
         {
            std::cout << "Eye: "; print(camera.eye);
            std::cout << "Aim: "; print(camera.aim);
         }
         break;

      // Movement
      case GLFW_KEY_W:
         if( action == GLFW_PRESS )
            camera.aim += camera.view * move_speed;
            camera.eye += camera.view * move_speed;
         break;

      case GLFW_KEY_S:
         if( action == GLFW_PRESS )
            camera.aim -= camera.view * move_speed;
            camera.eye -= camera.view * move_speed;
         break;

      case GLFW_KEY_A:
         if( action == GLFW_PRESS )
            camera.aim -= camera.right * move_speed;
            camera.eye -= camera.right * move_speed;
         break;

      case GLFW_KEY_D:
         if( action == GLFW_PRESS )
            camera.aim += camera.right * move_speed;
            camera.eye += camera.right * move_speed;
         break;

      case GLFW_KEY_SPACE:
         if( action == GLFW_PRESS )
            camera.aim += vec3(0.0, 1.0, 0.0) * move_speed;
            camera.eye += vec3(0.0, 1.0, 0.0) * move_speed;
         break;

      case GLFW_KEY_X:
         if( action == GLFW_PRESS )
            camera.aim -= vec3(0.0, 1.0, 0.0) * move_speed;
            camera.eye -= vec3(0.0, 1.0, 0.0) * move_speed;
         break;

      default:
         ;
   }
   */
}

void OGLRenderer::cursorPosCallback(double xpos, double ypos)
{
   double px = xpos;
   double py = ypos;

   px = 2.0 * (px / static_cast<double>(w_width)) - 1.0;
   py = 2.0 * (py / static_cast<double>(w_height)) - 1.0;

   if( flag_use_IPR )
   {
      double mouse_dx = (px - camera.last_mouse_x);
      double mouse_dy = (py - camera.last_mouse_y);

      double d = ( abs(mouse_dx) > abs(mouse_dy) ) ? mouse_dx : mouse_dy;

      if( camera.zoom_enabled )
      {
         double z = d / 250.0;
         vec3 dir = camera.view;

         if( length(dir) < 0.1 && z > 0 )
         {
            z *= 10.0;
            camera.aim = camera.aim + dir * z;
         }

         camera.eye = camera.eye + dir * z;
         // std::cout << camera.eye.v[0] << " " << camera.eye.v[1] << " " << camera.eye.v[2] << std::endl;
      } else if( camera.look_enabled ) {

         camera.delta_azim = (px - camera.start_mouse_x) / 36.0;
         camera.delta_elev = (py - camera.start_mouse_y) / 36.0;

         double local_delta_azim = mouse_dx / 36.0;
         double local_delta_elev = mouse_dy / 36.0;

         vec3 winX(1, 0, 0);
         vec3 winY(0, 1, 0);

         RotateX(winX, camera.current_elev + camera.delta_elev);
         RotateY(winX, camera.current_azim + camera.delta_azim);
         winX.v[2] = -winX.v[2];

         RotateX(winY, camera.current_elev + camera.delta_elev);
         RotateY(winY, camera.current_azim + camera.delta_azim);
         winY.v[2] = -winY.v[2];

         winX = normalise( winX );
         winY = normalise( winY );
         vec3 winZ = normalise( cross(winX, winY) );

         if( flag_fps_camera )
         {
            camera.up = winY;
            camera.view = winZ * -1.0;
            camera.right = winX;
         } else {

            ArbitraryRotate( winX, winY, winZ, local_delta_elev, 0, camera.eye, camera.aim );

            ArbitraryRotate( vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1), 0, -local_delta_azim, camera.eye, camera.aim );

            camera.up = winY;
            camera.view = normalise( camera.aim - camera.eye );
            camera.right = normalise( cross(camera.view, camera.up) );
         }

         camera.last_mouse_x = px;
         camera.last_mouse_y = py;
      } else if( camera.pan_enabled ) {

         px = xpos;
         py = static_cast<double>(w_height) - ypos;
         px = 2.0 * (px / static_cast<double>(w_width)) - 1.0;
         py = 2.0 * (py / static_cast<double>(w_height)) - 1.0;
         vec3 mouse_pos = camera.right * px + camera.up * py + camera.view * 1.0;

         vec3 dir = (mouse_pos - camera.prev_mouse_pos) * 1.0;
         camera.eye = camera.eye - dir;
         camera.aim = camera.aim - dir;

         camera.prev_mouse_pos = mouse_pos;
      }

   }
}

void OGLRenderer::mouseButtonCallback(int button, int action, int mods, int wx, int wy)
{
   if(action == OGL_PRESS)
   {
      double a(wx), b(wy);
      a = 2.0 * (a / static_cast<double>(w_width)) - 1.0;
      b = 2.0 * (b / static_cast<double>(w_height)) - 1.0;

      camera.start_mouse_x = camera.last_mouse_x = a;
      camera.start_mouse_y = camera.last_mouse_y = b;

      camera.look_enabled = (button == OGL_MOUSE_BUTTON_LEFT);
      camera.zoom_enabled = (button == OGL_MOUSE_BUTTON_RIGHT);
      if(button == OGL_MOUSE_BUTTON_MIDDLE)
      {
         camera.pan_enabled = true;

         a = wx, b = wy;
         b = static_cast<double>(w_height) - b;
         a = 2.0 * (a / static_cast<double>(w_width)) - 1.0;
         b = 2.0 * (b / static_cast<double>(w_height)) - 1.0;
         camera.prev_mouse_pos = camera.right * a + camera.up * b + camera.view * 1.0;
      }

   } else if( action == OGL_RELEASE && (camera.look_enabled || camera.zoom_enabled || camera.pan_enabled) ) {
      camera.look_enabled = false;
      camera.zoom_enabled = false;
      camera.pan_enabled = false;
      camera.current_elev += camera.delta_elev;
      camera.current_azim += camera.delta_azim;
      camera.delta_elev = camera.delta_azim = 0.0;
   }
}

void OGLRenderer::scrollCallback(double xoffset, double yoffset)
{
   camera.fov += 0.5 * ( (yoffset < 0.0) ? -1.0 : 1.0 );
}

/* UI */

void OGLRenderer::setRaymarchIterations(int n)
{
   uniforms.raymarch_iterations = n;
   reloadRaymarchProgram();
}

void OGLRenderer::setFractalIterations(int n)
{
   uniforms.fractal_iterations = n;
   reloadRaymarchProgram();
}

void OGLRenderer::setFLimitX(double flx)
{
   uniforms.flimit_x = flx;
   reloadRaymarchProgram();
}

void OGLRenderer::setFLimitY(double fly)
{
   uniforms.flimit_y = fly;
   reloadRaymarchProgram();
}

void OGLRenderer::setFLimitZ(double flz)
{
   uniforms.flimit_z = flz;
   reloadRaymarchProgram();
}

void OGLRenderer::setJuliaCx(double jcx)
{
   uniforms.julia_C_x = jcx;
   reloadRaymarchProgram();
}

void OGLRenderer::setJuliaCy(double jcy)
{
   uniforms.julia_C_y = jcy;
   reloadRaymarchProgram();
}

void OGLRenderer::setJuliaCz(double jcz)
{
   uniforms.julia_C_z = jcz;
   reloadRaymarchProgram();
}

void OGLRenderer::setJuliaOffsetX(double jofx)
{
   uniforms.julia_offset_x = jofx;
   reloadRaymarchProgram();
}

void OGLRenderer::setJuliaOffsetY(double jofy)
{
   uniforms.julia_offset_y = jofy;
   reloadRaymarchProgram();
}

void OGLRenderer::setJuliaOffsetZ(double jofz)
{
   uniforms.julia_offset_z = jofz;
   reloadRaymarchProgram();
}

void OGLRenderer::setDEOffset(double deof)
{
   uniforms.de_offset = deof;
   reloadRaymarchProgram();
}

void OGLRenderer::setFractalScale(double s)
{
   uniforms.scale = s;
   reloadRaymarchProgram();
}

void OGLRenderer::setMR2(double r)
{
   uniforms.min_radius_2 = r;
   reloadRaymarchProgram();
}

void OGLRenderer::setFR2(double r)
{
   uniforms.fixed_radius_2 = r;
   reloadRaymarchProgram();
}

void OGLRenderer::setGlowStrength(double s)
{
   uniforms.glow_strength = s;
   reloadRaymarchProgram();
}

void OGLRenderer::setEpsilon(double s)
{
   uniforms.epsilon = s;
   reloadRaymarchProgram();
}

void OGLRenderer::getConfiguration()
{
   std::cout << "rend.setFractalIterations("
      << uniforms.fractal_iterations << ")"
      << std::endl;

   std::cout << "rend.setRaymarchIterations("
      << uniforms.raymarch_iterations << ")"
      << std::endl;

   std::cout << "rend.setFLimit("
      << uniforms.flimit_x << ", "
      << uniforms.flimit_y << ", "
      << uniforms.flimit_z << ")"
      << std::endl;
      
   std::cout << "rend.setJuliaC("
      << uniforms.julia_C_x << ", "
      << uniforms.julia_C_y << ", "
      << uniforms.julia_C_z << ")"
      << std::endl;
      
   std::cout << "rend.setJuliaOffset("
      << uniforms.julia_offset_x << ", "
      << uniforms.julia_offset_y << ", "
      << uniforms.julia_offset_z << ")"
      << std::endl;
      
   std::cout << "rend.setEpsilon("
      << uniforms.epsilon << ")"
      << std::endl;
      
   std::cout << "rend.setDEOffset("
      << uniforms.de_offset << ")"
      << std::endl;
      
   std::cout << "rend.setScale("
      << uniforms.scale << ")"
      << std::endl;
      
   std::cout << "rend.setMR2("
      << uniforms.min_radius_2 << ")"
      << std::endl;
      
   std::cout << "rend.setFR2("
      << uniforms.fixed_radius_2 << ")"
      << std::endl;
      
   std::cout << "rend.setCameraEye("
      << camera.eye.v[0] << ", "
      << camera.eye.v[1] << ", "
      << camera.eye.v[2] << ")"
      << std::endl;

   std::cout << "rend.setCameraAim("
      << camera.aim.v[0] << ", "
      << camera.aim.v[1] << ", "
      << camera.aim.v[2] << ")"
      << std::endl;

   std::cout << "rend.setCameraView("
      << camera.view.v[0] << ", "
      << camera.view.v[1] << ", "
      << camera.view.v[2] << ")"
      << std::endl;

   std::cout << "rend.setCameraUp("
      << camera.up.v[0] << ", "
      << camera.up.v[1] << ", "
      << camera.up.v[2] << ")"
      << std::endl;

   std::cout << "rend.setCameraRight("
      << camera.right.v[0] << ", "
      << camera.right.v[1] << ", "
      << camera.right.v[2] << ")"
      << std::endl;

   std::cout << "rend.setCameraFOV(" << camera.fov << ")" << std::endl;
}

/*
void _update_fps_counter (GLFWwindow* window) {
   static double previous_seconds = glfwGetTime ();
   static int frame_count;
   double current_seconds = glfwGetTime ();
   double elapsed_seconds = current_seconds - previous_seconds;
   if (elapsed_seconds > 0.25) {
      previous_seconds = current_seconds;
      fps = (double)frame_count / elapsed_seconds;
      char tmp[128];
      sprintf (tmp, "Akasha (fps: %.2f)", fps);
      glfwSetWindowTitle (window, tmp);
      frame_count = 0;
   }
   frame_count++;
}
*/

void OGLRenderer::init()
{
   // Have to have a current context to init renderer
   glClearColor(0.1, 0.1, 0.1, 1.0); // background color
   glClearDepth(1.0f); // 0 is near, 1 is far

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);

   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glFrontFace(GL_CCW);

   glEnable(GL_TEXTURE_2D);

   GLenum err = glewInit();
   if( GLEW_OK != err )
   {
      std::cerr << "Oops, it broke: " << glewGetString(err) << std::endl;
      exit(4);
   }

   // Init camera
   camera.eye = vec3(-0.0, 0.0, 1.1);
   camera.aim = vec3(-0.0, 0.0, 0.1);
   // camera.eye = vec3(-0.43, 0.58, 1.81);
   // camera.aim = vec3(-0.56, 0.52, 0.58);

   camera.view = normalise( camera.aim - camera.eye );
   camera.up = vec3(0.0, 1.0, 0.0);

   camera.right = normalise( cross(camera.view, camera.up) );

   camera.fov = 90.0f;

   camera.dx = camera.dy = camera.dz = 0.0;
   camera.move_speed = 0.025;

   camera.look_enabled = false;
   camera.zoom_enabled = false;
   camera.pan_enabled = false;

   vec3 updatePos = camera.eye - camera.aim;
   vec3 axisOrigin(updatePos.v[0], 0, 0);
   double dist = length(axisOrigin - updatePos);
   vec3 tmp1(updatePos.v[0], 0, dist);

   vec3 tmp = normalise( updatePos );
   tmp1 = normalise( tmp1 );

   camera.current_elev = ONE_DEG_IN_RAD * acos( dot(tmp, tmp1) );

   axisOrigin = vec3(0, updatePos.v[1], 0);
   dist = length(axisOrigin - updatePos);

   tmp1 = vec3(0, updatePos.v[1], dist);
   tmp1 = normalise( tmp1 );

   camera.current_azim = ONE_DEG_IN_RAD * acos( dot(tmp1, tmp) );

   // Set default fractal parameters
   uniforms.fractal_iterations = 8;
   uniforms.raymarch_iterations = 1024;
   uniforms.fog_level = 0.01;
   uniforms.epsilon = 0.0001;
   // uniforms.epsilon = pow(10.0, -2.35396);

   uniforms.scale = 2.0;
   uniforms.min_radius_2 = 0.1;
   uniforms.glow_strength = 0.5;

   // Begin loading VBOs, shaders

   createVertexStorage();

   loadShaderFiles();
   loadGLSLRenderingProgram();
   reloadRaymarchProgram();

   /* Init full/ipr texture and framebuffers */

   // Full size tex and framebuffer
   glGenTextures(1, &full_tex_id);
   glBindTexture(GL_TEXTURE_2D, full_tex_id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, full_tex_width, full_tex_height, 0, GL_RGBA, GL_FLOAT, NULL);

   glGenFramebuffers(1, &full_framebuffer);
   glBindFramebuffer(GL_FRAMEBUFFER, full_framebuffer);
   glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, full_tex_id, 0);

   // IPR tex and framebuffer
   glGenTextures(1, &ipr_tex_id);
   glBindTexture(GL_TEXTURE_2D, ipr_tex_id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ipr_tex_width, ipr_tex_height, 0, GL_RGBA, GL_FLOAT, NULL);

   glGenFramebuffers(1, &ipr_framebuffer);
   glBindFramebuffer(GL_FRAMEBUFFER, ipr_framebuffer);
   glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ipr_tex_id, 0);

   // Set which buffers to draw to
   GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
   glDrawBuffers(1, draw_buffers);
}

void OGLRenderer::uninit()
{
   glDeleteBuffers(1, &v_position_vbo);
   glDeleteBuffers(1, &v_normal_vbo);
   glDeleteBuffers(1, &v_texcoord_vbo);

   glDeleteProgram(rendering_program);
   glDeleteProgram(raymarch_program);
}

void OGLRenderer::draw()
{
   // update timers
   // static double p_time = glfwGetTime ();
   // double u_time = glfwGetTime ();
   // double u_delta = u_time - p_time;
   // p_time = u_time;
   // _update_fps_counter (g_window);

   { // Raymarch
      // Begin

      glBindFramebuffer(GL_FRAMEBUFFER, full_framebuffer);

      int w = full_tex_width;
      int h = full_tex_height;

      // w = ipr_tex_width;
      // h = ipr_tex_height;

      // Use IPR mode if we've received input
      if( flag_use_IPR )
      {
         // std::cout << "Use IPR" << std::endl;
         // Use 1 sample
         aa_sample_count = 0;

         glBindFramebuffer(GL_FRAMEBUFFER, ipr_framebuffer);

         w = ipr_tex_width;
         h = ipr_tex_height;
      }

      // Render only if in IPR mode or there are samples left to use
      if( flag_use_IPR || (aa_sample_count < max_aa_samples && flag_render_next_sample) )
      {
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glViewport(0, 0, w, h);

         glUseProgram(raymarch_program);
         // glUniform1f(getUniformLocation(raymarch_program, "u_time"), u_time);
         glUniform2f(getUniformLocation(raymarch_program, "u_resolution"), w, h);

         glUniform3fv( getUniformLocation(raymarch_program, "u_cam_eye"), 1, camera.eye.v );
         glUniform3fv( getUniformLocation(raymarch_program, "u_cam_view"), 1, camera.view.v );
         glUniform3fv( getUniformLocation(raymarch_program, "u_cam_up"), 1, camera.up.v );
         glUniform3fv( getUniformLocation(raymarch_program, "u_cam_right"), 1, camera.right.v );
         glUniform1f( getUniformLocation(raymarch_program, "u_cam_fov"), camera.fov );

         if( !flag_use_IPR )
            std::cout << "Raymarch with " << aa_sample_count + 1 << " samples" << std::endl;

         glUniform1i(getUniformLocation(raymarch_program, "u_fractal_iters"), uniforms.fractal_iterations);
         glUniform1i(getUniformLocation(raymarch_program, "u_raymarch_iters"), uniforms.raymarch_iterations);

         glUniform1f( getUniformLocation(raymarch_program, "u_fog_level"), uniforms.fog_level );
         glUniform1f( getUniformLocation(raymarch_program, "u_epsilon"), uniforms.epsilon );

         glBindBuffer(GL_ARRAY_BUFFER, v_position_vbo);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
         glEnableVertexAttribArray(0);

         glBindBuffer(GL_ARRAY_BUFFER, v_normal_vbo);
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
         glEnableVertexAttribArray(1);

         glBindBuffer(GL_ARRAY_BUFFER, v_texcoord_vbo);
         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
         glEnableVertexAttribArray(2);

         glDrawArrays(GL_TRIANGLES, 0, point_count);

         glDisableVertexAttribArray(0);
         glDisableVertexAttribArray(1);
         glDisableVertexAttribArray(2);
         // flag_use_IPR = false;

      }

      if( !flag_use_IPR && flag_render_next_sample )
         ++aa_sample_count;

      if( flag_render_next_sample )
         flag_render_next_sample = false;

      // std::cout << "Sample Count: " << aa_sample_count << std::endl;

      //End
   }

   { // Draw
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glViewport(0, 0, w_width, w_height);

      glPointSize(2.0);

      glUseProgram(rendering_program);

      glBindTexture(GL_TEXTURE_2D, full_tex_id);

      if( flag_use_IPR )
      {
         glBindTexture(GL_TEXTURE_2D, ipr_tex_id);
      }

      // Begin rendering
      glBindBuffer(GL_ARRAY_BUFFER, v_position_vbo);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, v_normal_vbo);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(1);

      glBindBuffer(GL_ARRAY_BUFFER, v_texcoord_vbo);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(2);

      glDrawArrays(GL_TRIANGLES, 0, point_count);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);
   }

}

}
