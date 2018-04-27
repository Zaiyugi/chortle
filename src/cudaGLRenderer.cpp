#include "cudaGLRenderer.h"

namespace akasha
{

void CudaGLRenderer::RotateX(vec3 &v, double degree)
{
   double c = cos(degree * ONE_RAD_IN_DEG);
   double s = sin(degree * ONE_RAD_IN_DEG);
   double v1 = v.v[1] * c - v.v[2] * s;
   double v2 = v.v[1] * s + v.v[2] * c;
   v.v[1] = v1; v.v[2] = v2;
}

void CudaGLRenderer::RotateY(vec3 &v, double degree)
{
   double c = cos(degree * ONE_RAD_IN_DEG);
   double s = sin(degree * ONE_RAD_IN_DEG);
   double v0 = v.v[0] * c + v.v[2] * s;
   double v2 = -v.v[0] * s + v.v[2] * c;
   v.v[0] = v0; v.v[2] = v2;
}

void CudaGLRenderer::ArbitraryRotate(vec3 U, vec3 V, vec3 W, double degreeX, double degreeY, vec3& point, vec3 aim)
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
void CudaGLRenderer::createVertexStorage()
{
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

   size_t bufsize = sizeof(GLfloat) * 6 * 3;
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

   bufsize = sizeof(GLfloat) * 6 * 2;
   glGenBuffers(1, &v_texcoord_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, v_texcoord_vbo);
   glBufferData(GL_ARRAY_BUFFER, bufsize, vt, GL_STATIC_DRAW);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
   std::cerr << "GL error: " << glGetError() << std::endl;
}

void CudaGLRenderer::loadShaderFiles()
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
}

// GLSL Vertex/Fragment shaders
void CudaGLRenderer::loadGLSLRenderingProgram()
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

/* Callbacks */

void CudaGLRenderer::reshapeCallback(int w, int h)
{
   w_width = w;
   w_height = h;
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void CudaGLRenderer::errorCallback (int error, const char* description) {
   fputs (description, stderr);
   std::cerr << description << std::endl;
}

void CudaGLRenderer::keyCallback(int key, int action, int mods)
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

void CudaGLRenderer::cursorPosCallback(double xpos, double ypos)
{
   double px = xpos;
   double py = ypos;

   px = 2.0 * (px / static_cast<double>(w_width)) - 1.0;
   py = 2.0 * (py / static_cast<double>(w_height)) - 1.0;

   if( true )
   {
      double mouse_dx = (px - camera_options.last_mouse_x);
      double mouse_dy = (py - camera_options.last_mouse_y);

      double d = ( abs(mouse_dx) > abs(mouse_dy) ) ? mouse_dx : mouse_dy;

      if( camera_options.zoom_enabled )
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
      } else if( camera_options.look_enabled ) {

         camera_options.delta_azim = (px - camera_options.start_mouse_x) / 36.0;
         camera_options.delta_elev = (py - camera_options.start_mouse_y) / 36.0;

         double local_delta_azim = mouse_dx / 36.0;
         double local_delta_elev = mouse_dy / 36.0;

         vec3 winX(1, 0, 0);
         vec3 winY(0, 1, 0);

         RotateX(winX, camera_options.current_elev + camera_options.delta_elev);
         RotateY(winX, camera_options.current_azim + camera_options.delta_azim);
         winX.v[2] = -winX.v[2];

         RotateX(winY, camera_options.current_elev + camera_options.delta_elev);
         RotateY(winY, camera_options.current_azim + camera_options.delta_azim);
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

         camera_options.last_mouse_x = px;
         camera_options.last_mouse_y = py;
      } else if( camera_options.pan_enabled ) {

         px = xpos;
         py = static_cast<double>(w_height) - ypos;
         px = 2.0 * (px / static_cast<double>(w_width)) - 1.0;
         py = 2.0 * (py / static_cast<double>(w_height)) - 1.0;
         vec3 mouse_pos = camera.right * px + camera.up * py + camera.view * 1.0;

         vec3 dir = (mouse_pos - camera.prev_mouse_pos) * 1.0;
         camera.eye = camera.eye - dir;
         camera.aim = camera.aim - dir;

         camera_options.prev_mouse_pos = mouse_pos;
      }

   }
}

void CudaGLRenderer::mouseButtonCallback(int button, int action, int mods, int wx, int wy)
{
   if(action == OGL_PRESS)
   {
      double a(wx), b(wy);
      a = 2.0 * (a / static_cast<double>(w_width)) - 1.0;
      b = 2.0 * (b / static_cast<double>(w_height)) - 1.0;

      camera_options.start_mouse_x = camera_options.last_mouse_x = a;
      camera_options.start_mouse_y = camera_options.last_mouse_y = b;

      camera_options.look_enabled = (button == OGL_MOUSE_BUTTON_LEFT);
      camera_options.zoom_enabled = (button == OGL_MOUSE_BUTTON_RIGHT);
      if(button == OGL_MOUSE_BUTTON_MIDDLE)
      {
         camera_options.pan_enabled = true;

         a = wx, b = wy;
         b = static_cast<double>(w_height) - b;
         a = 2.0 * (a / static_cast<double>(w_width)) - 1.0;
         b = 2.0 * (b / static_cast<double>(w_height)) - 1.0;
         camera_options.prev_mouse_pos = camera.right * a + camera.up * b + camera.view * 1.0;
      }

   } else if( action == OGL_RELEASE && (camera_options.look_enabled || camera_options.zoom_enabled || camera_options.pan_enabled) ) {
      camera_options.look_enabled = false;
      camera_options.zoom_enabled = false;
      camera_options.pan_enabled = false;
      camera_options.current_elev += camera_options.delta_elev;
      camera_options.current_azim += camera_options.delta_azim;
      camera_options.delta_elev = camera_options.delta_azim = 0.0;
   }
}

void CudaGLRenderer::scrollCallback(double xoffset, double yoffset)
{
   camera.fov += 0.5 * ( (yoffset < 0.0) ? -1.0 : 1.0 );
}

/* UI */

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

void CudaGLRenderer::init()
{
   cudaGLSetGLDevice(0);

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

   camera.view = normalise( camera.aim - camera.eye );
   camera.up = vec3(0.0, 1.0, 0.0);

   camera.right = normalise( cross(camera.view, camera.up) );

   camera.fov = 65.0f;

   camera_options.dx = camera_options.dy = camera_options.dz = 0.0;
   camera_options.move_speed = 0.025;

   camera_options.look_enabled = false;
   camera_options.zoom_enabled = false;
   camera_options.pan_enabled = false;

   vec3 updatePos = camera.eye - camera.aim;
   vec3 axisOrigin(updatePos.v[0], 0, 0);
   double dist = length(axisOrigin - updatePos);
   vec3 tmp1(updatePos.v[0], 0, dist);

   vec3 tmp = normalise( updatePos );
   tmp1 = normalise( tmp1 );

   camera_options.current_elev = ONE_DEG_IN_RAD * acos( dot(tmp, tmp1) );

   axisOrigin = vec3(0, updatePos.v[1], 0);
   dist = length(axisOrigin - updatePos);

   tmp1 = vec3(0, updatePos.v[1], dist);
   tmp1 = normalise( tmp1 );

   camera_options.current_azim = ONE_DEG_IN_RAD * acos( dot(tmp1, tmp) );

   // Set default fractal parameters
   uniforms.fractal_iterations = 8;
   uniforms.raymarch_iterations = 1024;
   uniforms.fog_level = 0.01;
   uniforms.epsilon = 0.0001;

   uniforms.scale = 2.0;
   uniforms.mr2 = 0.1;
   uniforms.glow_strength = 0.5;

   // Begin loading VBOs, shaders

   createVertexStorage();

   loadShaderFiles();
   loadGLSLRenderingProgram();

   /* Init full/ipr texture and framebuffers */

   // Full size tex and framebuffer
   glGenTextures(1, &renderbuffer);
   glBindTexture(GL_TEXTURE_2D, renderbuffer);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, full_tex_width, full_tex_height, 0, GL_RGBA, GL_FLOAT, NULL);

   cudaGraphicsGLRegisterBuffer(&renderbuffer_CUDA, renderbuffer, cudaGraphicsMapFlagsWriteDiscard);

   // glGenFramebuffers(1, &full_framebuffer);
   // glBindFramebuffer(GL_FRAMEBUFFER, full_framebuffer);
   // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderbuffer, 0);

   // Set which buffers to draw to
   // GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
   // glDrawBuffers(1, draw_buffers);
}

void CudaGLRenderer::uninit()
{
   cudaGraphicsUnregisterResource(renderbuffer_CUDA);

   glDeleteBuffers(1, &v_position_vbo);
   glDeleteBuffers(1, &v_normal_vbo);
   glDeleteBuffers(1, &v_texcoord_vbo);

   glDeleteProgram(rendering_program);
}

void CudaGLRenderer::draw()
{
   // update timers
   // static double p_time = glfwGetTime ();
   // double u_time = glfwGetTime ();
   // double u_delta = u_time - p_time;
   // p_time = u_time;
   // _update_fps_counter (g_window);

   { // Raymarch
      float4* rbuf;
      cudaGraphicsMapResources(1, &renderbuffer_CUDA, 0);
      size_t num_bytes;
      cudaGraphicsResourceGetMappedPointer( (void**)&rbuf, &num_bytes, renderbuffer_CUDA );

      // Call kernel

      cudaGraphicsUnmapResources(1, &renderbuffer_CUDA, 0);
   }

   { // Draw
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glViewport(0, 0, w_width, w_height);

      glPointSize(2.0);

      glUseProgram(rendering_program);

      glBindTexture(GL_TEXTURE_2D, renderbuffer);

      // Begin rendering
      glBindBuffer(GL_ARRAY_BUFFER, v_position_vbo);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, v_normal_vbo);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(1);

      glBindBuffer(GL_ARRAY_BUFFER, v_texcoord_vbo);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(2);

      glDrawArrays(GL_TRIANGLES, 0, 6);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);
   }

}

}
