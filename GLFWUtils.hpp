#ifndef __SIMPLEGL_GLFWUTILS_HPP__
#define __SIMPLEGL_GLFWUTILS_HPP__
#include "SimpleGL.hpp"
#include <GLFW/glfw3.h>
#include <string>

namespace SimpleGL
{
	class GLFWUtils
	{
		public:
			static void processInput(GLFWwindow* window, Camera& camera, float deltaTime)
			{
				if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
					glfwSetWindowShouldClose(window, true);

				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
					camera.move(SimpleGL::Camera::MovementDirection::FORWARD, deltaTime);
				if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
					camera.move(SimpleGL::Camera::MovementDirection::BACKWARD, deltaTime);
				if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
					camera.move(SimpleGL::Camera::MovementDirection::LEFT, deltaTime);
				if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
					camera.move(SimpleGL::Camera::MovementDirection::RIGHT, deltaTime);
				if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
					camera.move(SimpleGL::Camera::MovementDirection::UP, deltaTime);
				if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
					camera.move(SimpleGL::Camera::MovementDirection::DOWN, deltaTime);

				if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
				{
					float speed = camera.getMovementSpeed();
					speed -= 0.01f;
					speed = std::max(speed, 0.1f);
					camera.setMovementSpeed(SimpleGL::Camera::MovementSpeed::CUSTOM, speed);
				}
				else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
				{
					float speed = camera.getMovementSpeed();
					speed += 0.01f;
					speed = std::min(speed, 5.0f);
					camera.setMovementSpeed(SimpleGL::Camera::MovementSpeed::CUSTOM, speed);
				}
			}

			static void processCursor(Camera& camera, glm::vec2& lastPos, glm::vec2 currentPos, bool& isFirst)
			{
				if (isFirst)
				{
					lastPos = currentPos;
					isFirst = false;
				}

				glm::vec2 delta = currentPos - lastPos;
				lastPos = currentPos;
				camera.rotate(-delta.x, delta.y);
			}

			static void processResize(Camera& camera, int width, int height)
			{
				camera.setAspect(float(width) / float(height));
				glViewport(0, 0, width, height);
			}

			static void processScroll(Camera& camera, float offset)
			{
				camera.zoom(offset);
			}

			template<typename T>
			static T* getUserData(GLFWwindow* window)
			{
				return (T*)glfwGetWindowUserPointer(window);
			}

			static GLFWwindow* initWindow_FullScreen(std::string title, int major = 3, int minor = 3)
			{
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* videomode = glfwGetVideoMode(monitor);
				GLFWwindow* window = glfwCreateWindow(videomode->width, videomode->height, title.data(), monitor, NULL);
				glfwMakeContextCurrent(window);

				return window;
			}

			static void disableCursor(GLFWwindow* window)
			{
				if (window != nullptr)
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}

			static void calcDeltaTime(float& lastTime, float& deltaTime)
			{
				float currentTime = glfwGetTime();
				deltaTime = currentTime - lastTime;
				lastTime = currentTime;
			}
			
			template<typename T>
			static T* newUserData(GLFWwindow* window)
			{
				T* data = new T;
				glfwSetWindowUserPointer(window, data);
				return data;
			}
	};
}

#endif
