//These macros designed for lazy people like me. :)

#ifndef __SIMPLEGL_EXTRA_MACRO_HPP__
#define __SIMPLEGL_EXTRA_MACRO_HPP__
#include <iostream>
#include <string>
#include "../SimpleGL.hpp"

#define LoadGL()\
		if(!SimpleGL::initGL())\
		{\
			std::cout << "Failed to load GL." << std::endl;\
			return -1;\
		}

#define LoadProgramFromFile(program,vfilename,ffilename)\
		if(!program.loadProgramFromFile(std::string(vfilename),std::string(ffilename)))\
		{\
			std::cout << "Failed to load program." << std::endl;\
			std::cout << program.getError() << std::endl;\
			return -1;\
		}

#define LoadProgramFromFile2(program,vfilename,ffilename,gfilename)\
		if(!program.loadProgramFromFile(std::string(vfilename),std::string(ffilename),std::string(gfilename)))\
		{\
			std::cout << "Failed to load program." << std::endl;\
			std::cout << program.getError() << std::endl;\
			return -1;\
		}

#define OutputError()\
		{\
			std::cout << SimpleGL::Error().toString() << std::endl;\
		}
		
#define DrawArrays(VAO,type,start,count)\
		glBindVertexArray(VAO);\
		glDrawArrays(type,start,count);\
		glBindVertexArray(0);\

#ifdef SIMPLEGL_USE_GLFW
	#define InitWindow_FullScreen(title,major,minor)\
		window = SimpleGL::GLFWUtils::initWindow_FullScreen(std::string(title),(unsigned int)major,(unsigned int)minor);\
		if (window == nullptr)\
		{\
			std::cout << "Failed to create window." << std::endl;\
			return -1;\
		}
		
	#define CALLBACK_FRAMEBUFFER_SIZE\
		void simplegl_framebuffer_size(GLFWwindow* window, int w, int h)\
		{\
			SimpleGL::GLFWUtils::processResize(camera, w, h);\
		}
		
	#define CALLBACK_CURSOR_POS\
		void simplegl_cursor_pos(GLFWwindow* window, double x, double y)\
		{\
			glm::vec2 currentPos = glm::vec2(x, y);\
			SimpleGL::GLFWUtils::processCursor(camera, lastPos, currentPos, isFirst);\
		}
		
	#define CALLBACK_SCROLL\
		void simplegl_scroll(GLFWwindow* window, double x, double y)\
		{\
			SimpleGL::GLFWUtils::processScroll(camera, y);\
		}
		
	#define CALLBACK_PROCESS_INPUT\
		void simplegl_process_input(GLFWwindow* window)\
		{\
			SimpleGL::GLFWUtils::processInput(window, camera, deltaTime);\
		}
#endif

#endif
