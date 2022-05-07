#ifndef __SIMPLEGL_CORE_ERROR_HPP__
#define __SIMPLEGL_CORE_ERROR_HPP__
#include "gl.hpp"
#include <sstream>

namespace SimpleGL
{
	class Error
	{
		public:
			static std::string parse(GLenum error)
			{
				std::string result;
				switch (error)
				{
					case GL_NO_ERROR:
						result = "GL_NO_ERROR";
						break;
					case GL_INVALID_ENUM:
						result = "GL_INVALID_ENUM";
						break;
					case GL_INVALID_VALUE:
						result = "GL_INVALID_VALUE";
						break;
					case GL_INVALID_OPERATION:
						result = "GL_INVALID_OPERATION";
						break;
					case GL_STACK_OVERFLOW:
						result = "GL_STACK_OVERFLOW";
						break;
					case GL_STACK_UNDERFLOW:
						result = "GL_STACK_UNDERFLOW";
						break;
					case GL_OUT_OF_MEMORY:
						result = "GL_OUT_OF_MEMORY";
						break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:
						result = "GL_INVALID_FRAMEBUFFER_OPERATION";
						break;
					default:
						result = "Unknow";
						break;
				}
				return result;
			}

			//https://learnopengl.com/In-Practice/Debugging
			static std::string describe(GLenum error)
			{
				std::string result;
				switch (error)
				{
					case GL_NO_ERROR:
						result = "No user error reported since the last call to glGetError.";
						break;
					case GL_INVALID_ENUM:
						result = "Set when an enumeration parameter is not legal.";
						break;
					case GL_INVALID_VALUE:
						result = "Set when a value parameter is not legal.";
						break;
					case GL_INVALID_OPERATION:
						result = "Set when the state for a command is not legal for its given parameters.";
						break;
					case GL_STACK_OVERFLOW:
						result = "Set when a stack pushing operation causes a stack overflow.";
						break;
					case GL_STACK_UNDERFLOW:
						result = "Set when a stack popping operation occurs while the stack is at its lowest point.";
						break;
					case GL_OUT_OF_MEMORY:
						result = "Set when a memory allocation operation cannot allocate (enough) memory.";
						break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:
						result = "Set when reading or writing to a framebuffer that is not complete.";
						break;
					default:
						result = "Unknown enumeration.";
						break;
				}
				return result;
			}

		public:
			GLenum error;
			std::string value;
			std::string description;

		public:
			Error()
			{
				error = glGetError();
				value = parse(error);
				description = describe(error);
			}

			Error& operator()()
			{
				error = glGetError();
				value = parse(error);
				description = describe(error);
				return *this;
			}

			std::string toString()
			{
				std::stringstream sstr;
				sstr << "Error Number:" << error << std::endl
				     << "Error String:" << value << std::endl
				     << "Error Description:" << description << std::endl;
				return sstr.str();
			}
	};
}

#endif
