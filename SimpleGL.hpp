#ifndef __SIMPLEGL_MAIN_HPP__
#define __SIMPLEGL_MAIN_HPP__

#if defined(USE_GLEW)
#include <GL/glew.h>
#else
#include <glad/glad.c>
#endif

namespace SimpleGL
{
	bool initGL()
	{
		#if defined(__glad_h_)
			if(!gladLoadGL()) return false;
		#elif defined(__glew_h__)
			if(glewInit() != GLEW_OK) return false;
		#endif
		return true;
	}
}

#include "Program.hpp"
#include "Camera.hpp"
#include "Error.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "Model.hpp"
#include "GLFWUtils.hpp"
#include "Buffer.hpp"

#endif
