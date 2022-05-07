#ifndef __SIMPLEGL_MAIN_HPP__
#define __SIMPLEGL_MAIN_HPP__

#include "Core/gl.hpp"

#include "Core/Program.hpp"
#include "Core/Camera.hpp"
#include "Core/Error.hpp"
#include "Core/Sprite.hpp"
#include "Core/Texture.hpp"
#include "Core/Model.hpp"

#ifdef SIMPLEGL_EXTRA
	#include "Extra/Macro.hpp"
	
	#ifdef SIMPLEGL_USE_GLFW
		#include "Extra/GLFWUtils.hpp"
	#endif
#endif

#endif
