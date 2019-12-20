#ifndef COMMON_H
#define COMMON_H



// #include "resources/chess.h"  //this is where we'll include the 3d chess logic - I'll have access to it from everywhere






#include <random>
#include <vector>
#include <stdio.h>
#include <iostream>

using std::cin;
using std::cerr;

using std::cout;
using std::flush;
using std::endl;

#include "shaders/Shader.h"

#define GL_GLEXT_PROTOTYPES



#define GLEW_STATIC
#include <GL/glew.h>


#include <SDL.h>
#include <SDL_opengl.h>


#include "../resources/LodePNG/lodepng.h"
// Good, simple png library

#include "../resources/perlin.h"
//perlin noise generation


#define GLM_FORCE_SWIZZLE
#define GLM_SWIZZLE_XYZW
#include "glm/glm.hpp" //general vector types
#include "glm/gtc/matrix_transform.hpp" // for glm::ortho
#include "glm/gtc/type_ptr.hpp" //to send matricies gpu-side
#include "glm/gtx/transform.hpp"


#endif
