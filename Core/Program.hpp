#ifndef __SIMPLEGL_CORE_SHADER_HPP__
#define __SIMPLEGL_CORE_SHADER_HPP__
#include <iostream>
#include <sstream>
#include <fstream>
#include "gl.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SimpleGL
{
	class Program
	{
		private:
			GLuint m_programID;
			std::stringstream m_error;

		public:
			static std::string check(GLuint ID, GLenum type)
			{
				GLint success;
				std::string result = std::string();

				if (type != GL_PROGRAM)
				{
					glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
					if (!success)
					{
						GLchar* infoLog = NULL;
						GLint length;
						glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
						infoLog = new char[length];
						glGetShaderInfoLog(ID, length, NULL, infoLog);
						result = std::string("Shader Error:\n") + std::string(infoLog);
						delete[] infoLog;
					}
				}
				else
				{
					glGetProgramiv(ID, GL_LINK_STATUS, &success);
					if (!success)
					{
						GLchar* infoLog = NULL;
						GLint length;
						glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);
						infoLog = new char[length];
						glGetProgramInfoLog(ID, length, NULL, infoLog);
						result = std::string("Program Error:\n") + std::string(infoLog);
						delete[] infoLog;
					}
				}

				return result;
			}
			static GLuint createShader(std::string source, GLenum type, std::stringstream* error_output = nullptr)
			{
				const GLchar* s = source.data();
				GLuint shader = glCreateShader(type);
				glShaderSource(shader, 1, &s, NULL);
				glCompileShader(shader);
				std::string error = check(shader, type);

				if (error_output != nullptr)
				{
					if (error.length() > 0)
					{
						*error_output << error << std::endl;
					}
				}

				if (error.length() > 0)
				{
					glDeleteShader(shader);
					return 0;
				}
				return shader;
			}
			static GLuint createProgram(std::string vSource, std::string fSource, std::string gSource, std::stringstream* error_output = nullptr)
			{
				GLuint vShader = createShader(vSource, GL_VERTEX_SHADER, error_output);
				GLuint fShader = createShader(fSource, GL_FRAGMENT_SHADER, error_output);
				GLuint gShader = 0;
				if (gSource.length() > 0)
				{
					gShader = createShader(gSource, GL_GEOMETRY_SHADER, error_output);
				}

				GLuint program = glCreateProgram();
				glAttachShader(program, vShader);
				glAttachShader(program, fShader);
				if (gShader != 0)
				{
					glAttachShader(program, gShader);
				}
				glLinkProgram(program);
				std::string error = check(program, GL_PROGRAM);

				glDeleteShader(vShader);
				glDeleteShader(fShader);
				if (gShader != 0)
				{
					glDeleteShader(gShader);
				}

				if (error_output != nullptr)
				{
					if (error.length() > 0)
					{
						*error_output << error << std::endl;
					}
				}

				if (error.length() > 0)
				{
					glDeleteProgram(program);
					return 0;
				}

				return program;
			}
			static std::string loadStringFromStream(std::istream& is)
			{
				std::stringstream result;
				std::string temp;
				while (getline(is, temp))
				{
					result << temp << std::endl;
				}
				return result.str();
			}

		public:
			Program()
			{
				m_programID = 0;
			}
			Program(std::string vSource, std::string fSource, std::string gSource = "")
			{
				m_programID = createProgram(vSource, fSource, gSource, &m_error);
			}
			~Program()
			{
				deleteProgram();
			}

		public:
			bool loadProgram(std::string vSource, std::string fSource, std::string gSource = "")
			{
				GLuint id = createProgram(vSource, fSource, gSource, &m_error);
				if (id == 0)
				{
					return false;
				}
				if (m_error.str().length() > 0)
				{
					return false;
				}

				m_programID = id;
				return true;
			}
			bool loadProgramFromStream(std::istream& vStream, std::istream& fStream)
			{
				std::string vSource = loadStringFromStream(vStream);
				std::string fSource = loadStringFromStream(fStream);
				return loadProgram(vSource, fSource);
			}
			bool loadProgramFromStream(std::istream& vStream, std::istream& fStream, std::istream& gStream)
			{
				std::string vSource = loadStringFromStream(vStream);
				std::string fSource = loadStringFromStream(fStream);
				std::string gSource = loadStringFromStream(gStream);
				return loadProgram(vSource, fSource, gSource);
			}
			bool loadProgramFromFile(std::string vPath, std::string fPath)
			{
				std::ifstream vStream;
				std::ifstream fStream;
				bool result;

				try
				{
					vStream.open(vPath);
					fStream.open(fPath);

					if (!vStream.is_open())
					{
						m_error << "Failed to open file:\"" << vPath << "\"" << std::endl;
						return false;
					}
					if (!fStream.is_open())
					{
						m_error << "Failed to open file:\"" << fPath << "\"" << std::endl;
						vStream.close();
						return false;
					}

					result = loadProgramFromStream(vStream, fStream);

					vStream.close();
					fStream.close();

				}
				catch (std::ifstream::failure& e)
				{
					result = false;
					m_error << "File Error:\n" << e.what() << std::endl;
				}

				return result;
			}
			bool loadProgramFromFile(std::string vPath, std::string fPath, std::string gPath)
			{
				std::ifstream vStream;
				std::ifstream fStream;
				std::ifstream gStream;
				bool result;

				try
				{
					vStream.open(vPath);
					fStream.open(fPath);
					gStream.open(gPath);

					if (!vStream.is_open())
					{
						m_error << "Failed to open file:\"" << vPath << "\"" << std::endl;
						return false;
					}
					if (!fStream.is_open())
					{
						m_error << "Failed to open file:\"" << fPath << "\"" << std::endl;
						vStream.close();
						return false;
					}
					if (!gStream.is_open())
					{
						m_error << "Failed to open file:\"" << gPath << "\"" << std::endl;
						vStream.close();
						fStream.close();
						return false;
					}

					result = loadProgramFromStream(vStream, fStream, gStream);

					vStream.close();
					fStream.close();
					gStream.close();

				}
				catch (std::ifstream::failure& e)
				{
					result = false;
					m_error << "File Error:\n" << e.what() << std::endl;
				}

				return result;
			}

			bool compile()
			{
				clearError();
				glLinkProgram(m_programID);
				std::string error = check(m_programID, GL_PROGRAM);
				if (error.length() > 0)
				{
					m_error << error << std::endl;
					return false;
				}
				return true;
			}

			Program& deleteProgram()
			{
				if (m_programID != 0)
				{
					glDeleteProgram(m_programID);
				}
				return *this;
			}

			Program& setProgramID(GLuint ID)
			{
				m_programID = ID;
				return *this;
			}
			GLuint getProgramID()
			{
				return m_programID;
			}

			Program& useProgram()
			{
				if (m_programID != 0)
				{
					glUseProgram(m_programID);
				}
				return *this;
			}

			std::string getError()
			{
				std::string error = m_error.str();
				if (error.length() == 0)
					error = "Program No Error";
				return error;
			}
			Program& clearError()
			{
				m_error.str("");
				return *this;
			}
			std::stringstream& writeError()
			{
				return m_error;
			}

		public:
			GLuint getUniformLocation(std::string name)
			{
				if (m_programID != 0)
				{
					return glGetUniformLocation(m_programID, name.data());
				}
				return -1;
			}

			Program& setUniform(std::string name, bool value)
			{
				if (m_programID != 0)
				{
					GLuint location = getUniformLocation(name);
					glUniform1i(location, value);
				}
				return *this;
			}
			Program& setUniform(std::string name, int value)
			{
				if (m_programID != 0)
				{
					GLuint location = getUniformLocation(name);
					glUniform1i(location, value);
				}
				return *this;
			}
			Program& setUniform(std::string name, float value)
			{
				if (m_programID != 0)
				{
					GLuint location = getUniformLocation(name);
					glUniform1f(location, value);
				}
				return *this;
			}

			Program& setUniform(std::string name, glm::vec2 value)
			{
				if (m_programID != 0)
				{
					GLuint location = getUniformLocation(name);
					glUniform2f(location, value.x, value.y);
				}
				return *this;
			}
			Program& setUniform(std::string name, glm::vec3 value)
			{
				if (m_programID != 0)
				{
					GLuint location = getUniformLocation(name);
					glUniform3f(location, value.x, value.y, value.z);
				}
				return *this;
			}
			Program& setUniform(std::string name, glm::vec4 value)
			{
				if (m_programID != 0)
				{
					GLuint location = getUniformLocation(name);
					glUniform4f(location, value.x, value.y, value.z, value.w);
				}
				return *this;
			}

			Program& setUniform(std::string name, glm::mat2 value, GLenum transpose = GL_FALSE)
			{
				if (m_programID != 0)
				{
					GLuint location = getUniformLocation(name);
					glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
				}
				return *this;
			}
			Program& setUniform(std::string name, glm::mat3 value, GLenum transpose = GL_FALSE)
			{
				if (m_programID != 0)
				{
					GLuint location = getUniformLocation(name);
					glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
				}
				return *this;
			}
			Program& setUniform(std::string name, glm::mat4 value, GLenum transpose = GL_FALSE)
			{
				if (m_programID != 0)
				{
					GLuint location = getUniformLocation(name);
					glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
				}
				return *this;
			}
	};

	//It just means existence,no idea to program it.
	//GL_COMPUTE_SHADER
#ifdef GL_COMPUTE_SHADER
	class ComputeProgram : public Program
	{
		public:
			static std::string check(GLuint program)
			{
				return std::string();
			}

		public:
			ComputeProgram() {}
			~ComputeProgram() {}

		public:
			bool loadComputeShader(std::string source)
			{
				return false;
			}
			bool loadComputeShaderFromStream(std::istream& is)
			{
				return false;
			}
			bool loadComputeShaderFromFile(std::string cs_file)
			{
				return false;
			}
	};
#endif
}

#endif
