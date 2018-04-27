## Chortle: Real-time 3D fractal visualizer 
### Author: Zachary Shore
### Project: Digital Production Arts Thesis project

### Description

Chortle is a real-time visualizer for exploring 3D fractals. It uses GLSL to render 3D fractals like the Mandelbox or Pseudo-Kleinian Menger with sphere-tracing to get real-time performance. Scenes can also be exported from Chortle as a Python snippet that can be used in Cackle. Information on Cackle can be found here: [Cackle](https://github.com/Zaiyugi/cackle)

### Building
*  make -- make all required libraries
*  make genswig -- makes the Python bindings; requires libraries to be built

### Running Chortle
* ./python/Chortle.py
This will run the Chortle interface. The path to swig/\_chortle.co must be in the PYTHONPATH envvar and the path to lib/libCackle.a must in the LD\_LIBRARY\_PATH envvar for Chortle to launch.

### Directories
*  _glsl/_
   Contains GLSL shaders used for rendering 
*  _include/_  
   Contains class header files
*  _lib/_  
   Contains static library files: libCackle.a
*  _src/_  
   Contains some source files
*  _obj/_  
   Intermediate object file storage
*  _python/_  
   Render and utility scripts
*  _swig/_  
   Swig interface and Swig-generated Python bindings

Created: 2017-04-01 
Edited: 2018-04-27
 
