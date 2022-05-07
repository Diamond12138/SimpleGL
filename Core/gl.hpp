#ifndef __SIMPLEGL_CORE_GL_HPP__
#define __SIMPLEGL_CORE_GL_HPP__

#if defined(SIMPLEGL_USE_GLEW)
	#include <GL/glew.h>
#else
	#include <glad/glad.c>
#endif

namespace SimpleGL
{
	//It is the same as gladGLversionStruct, but considering that someone will use GLEW, it is defined.
	struct SGL_GLVersion
	{
		unsigned int major;
		unsigned int minor;
	};
	
	bool initGL()
	{
		#if defined(__glad_h_)
			if(!gladLoadGL()) return false;
		#elif defined(__glew_h__)
			if(glewInit() != GLEW_OK) return false;
		#endif
		return true;
	}
	
	const SGL_GLVersion getGLVersion()
	{
		const unsigned char* version_str = glGetString(GL_VERSION);
		SGL_GLVersion version;
		version.major = int(version_str[0]) - int('0');
		version.minor = int(version_str[2]) - int('0');
		return version;
	}
}

#endif
