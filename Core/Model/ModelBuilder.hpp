#ifndef __SIMPLEGL_CORE_MODEL_MODELBUILDER_HPP__
#define __SIMPLEGL_CORE_MODEL_MODELBUILDER_HPP__
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "Model.hpp"
#include "../gl.hpp"

namespace SimpleGL
{
	class ModelBuilder
	{
		public:
			static std::vector<float> calcNormals(std::vector<float> vertices)
			{
				std::vector<float> normals;
				for (int i = 0; i < vertices.size() / 9; i++)
				{
					glm::vec3 A = glm::vec3(vertices.at(i * 9 + 0), vertices.at(i * 9 + 1), vertices.at(i * 9 + 2));
					glm::vec3 B = glm::vec3(vertices.at(i * 9 + 3), vertices.at(i * 9 + 4), vertices.at(i * 9 + 5));
					glm::vec3 C = glm::vec3(vertices.at(i * 9 + 6), vertices.at(i * 9 + 7), vertices.at(i * 9 + 8));

					glm::vec3 AB = glm::normalize(B - A);
					glm::vec3 BC = glm::normalize(C - B);

					glm::vec3 N = glm::normalize(glm::cross(AB, BC));

					for (int j = 0; j < 3; j++)
					{
						normals.push_back(N.x);
						normals.push_back(N.y);
						normals.push_back(N.z);
					}
				}
				return normals;
			}

			static std::vector<glm::vec3> calcNormals(std::vector<glm::vec3> vertices)
			{
				std::vector<glm::vec3> normals;
				for (int i = 0; i < vertices.size() / 3; i++)
				{
					glm::vec3 A = vertices.at(i + 0);
					glm::vec3 B = vertices.at(i + 1);
					glm::vec3 C = vertices.at(i + 2);

					glm::vec3 AB = glm::normalize(B - A);
					glm::vec3 BC = glm::normalize(C - B);

					glm::vec3 N = glm::normalize(glm::cross(AB, BC));

					for (int j = 0; j < 3; j++)
					{
						normals.push_back(N);

					}
				}
				return normals;
			}

			static glm::vec3 calcNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C)
			{
				glm::vec3 AB = glm::normalize(B - A);
				glm::vec3 BC = glm::normalize(C - B);

				glm::vec3 N = glm::normalize(glm::cross(AB, BC));

				return N;
			}

			static std::vector<float> stretchTexCoords(std::vector<float> texCoords, glm::vec3 value)
			{
				std::vector<float> result;
				for (int i = 0; i < texCoords.size() / 3; i++)
				{
					result.push_back(texCoords.at(i * 3 + 0) * value.x);
					result.push_back(texCoords.at(i * 3 + 1) * value.y);
					result.push_back(texCoords.at(i * 3 + 2) * value.z);
				}
				return result;
			}

			static std::vector<float> toFloatArray(std::vector<glm::vec3> vertices)
			{
				std::vector<float> v;
				for (int i = 0; i < vertices.size(); i++)
				{
					glm::vec3 vertex = vertices.at(i);
					v.push_back(vertex.x);
					v.push_back(vertex.y);
					v.push_back(vertex.z);
				}
				return v;
			}

			static std::vector<glm::vec3> toVec3Array(std::vector<float> vertices)
			{
				std::vector<glm::vec3> v;
				for (int i = 0; i < vertices.size() / 3; i++)
				{
					float x = vertices.at(i + 0);
					float y = vertices.at(i + 1);
					float z = vertices.at(i + 2);
					v.push_back(glm::vec3(x, y, z));

				}
				return v;
			}

			static void push_back_triangle(std::vector<glm::vec3>& array, glm::vec3 A, glm::vec3 B, glm::vec3 C)
			{
				array.push_back(A);
				array.push_back(B);
				array.push_back(C);
			}

			static Model toModel(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> texCoords)
			{
				std::vector<float> v = ModelBuilder::toFloatArray(vertices);
				std::vector<float> n = ModelBuilder::toFloatArray(normals);
				std::vector<float> t = ModelBuilder::toFloatArray(texCoords);
				return Model(v, n, t);
			}

			static void push_back_line(std::vector<float>& array, glm::vec3 A, glm::vec3 B)
			{
				array.push_back(A.x);
				array.push_back(A.y);
				array.push_back(A.z);

				array.push_back(B.x);
				array.push_back(B.y);
				array.push_back(B.z);
			}

			static void push_back_line(std::vector<glm::vec3>& array, glm::vec3 A, glm::vec3 B)
			{
				array.push_back(A);
				array.push_back(B);
			}

		public:
			//Vertices and texture coordinates are OK, but there are some problems in normals. The reason is unclear.
			//rotate around Y axis.
			static Model rotate(std::vector<glm::vec2> source_data, float span = 180.0f, bool texturePerFace = true)
			{
				const float zero = 0.0f;
				const glm::vec3 t00 = glm::vec3(0.0f, 0.0f, 0.0f);
				const glm::vec3 t01 = glm::vec3(0.0f, 1.0f, 0.0f);
				const glm::vec3 t10 = glm::vec3(1.0f, 0.0f, 0.0f);
				const glm::vec3 t11 = glm::vec3(1.0f, 1.0f, 0.0f);
				float s_unit = 1.0f / 360.f * span;
				float t_unit = 1.0f / float(source_data.size());

				std::vector<glm::vec3> vertices;
				std::vector<glm::vec3> texCoords;
				for (float angle = 0.0f; angle <= 360.0f; angle += span)
				{
					float angle0 = glm::radians(angle + 0.0f);
					float angle1 = glm::radians(angle + span);

					for (int i = 0; i < source_data.size() - 1; i++)
					{
						glm::vec2 V0 = source_data.at(i + 0);
						glm::vec2 V1 = source_data.at(i + 1);

						glm::vec3 A = glm::vec3(glm::cos(angle1) * V0.x, V0.y, glm::sin(angle1) * V0.x);
						glm::vec3 B = glm::vec3(glm::cos(angle1) * V1.x, V1.y, glm::sin(angle1) * V1.x);
						glm::vec3 C = glm::vec3(glm::cos(angle0) * V1.x, V1.y, glm::sin(angle0) * V1.x);
						glm::vec3 D = glm::vec3(glm::cos(angle0) * V0.x, V0.y, glm::sin(angle0) * V0.x);

						push_back_triangle(vertices, A, B, C);
						push_back_triangle(vertices, A, C, D);

						if (texturePerFace)
						{
							push_back_triangle(texCoords, t00, t01, t11);
							push_back_triangle(texCoords, t00, t11, t10);
						}
						else
						{
							float s0 = 1.0f - angle / span * s_unit;
							float s1 = 1.0f - (angle + span) / span * s_unit;
							float t0 = float(i) * t_unit;
							float t1 = float(i + 1) * t_unit;

							texCoords.push_back(glm::vec3(s1, t0, zero));
							texCoords.push_back(glm::vec3(s1, t1, zero));
							texCoords.push_back(glm::vec3(s0, t1, zero));

							texCoords.push_back(glm::vec3(s1, t0, zero));
							texCoords.push_back(glm::vec3(s0, t1, zero));
							texCoords.push_back(glm::vec3(s0, t0, zero));
						}
					}
				}
				std::vector<glm::vec3> normals = calcNormals(vertices);

				return toModel(vertices, normals, texCoords);
			}

			static std::vector<float> generateWireframe(std::vector<float> vertices)
			{
				int size = vertices.size();

				std::vector<glm::vec3> lines;
				for (int i = 0; i < size / 9; i++)
				{
					glm::vec3 A = glm::vec3(vertices.at(i * 9 + 0), vertices.at(i * 9 + 1), vertices.at(i * 9 + 2));
					glm::vec3 B = glm::vec3(vertices.at(i * 9 + 3), vertices.at(i * 9 + 4), vertices.at(i * 9 + 5));
					glm::vec3 C = glm::vec3(vertices.at(i * 9 + 6), vertices.at(i * 9 + 7), vertices.at(i * 9 + 8));

					push_back_line(vertices, A, B);
					push_back_line(vertices, B, C);
					push_back_line(vertices, C, A);
				}

				return toFloatArray(lines);
			}

			static GLuint generateAxis(GLuint position_location = 0,GLuint color_location = 1)
			{
				GLfloat vertices[] =
				{
					-100, 0, 0,
					    100, 0, 0,
					    0, -100, 0,
					    0, 100, 0,
					    0, 0, -100,
					    0, 0, 100
				    };

				GLfloat colors[] =
				{
					0, 0, 0,
					1, 0, 0,
					0, 0, 0,
					0, 1, 0,
					0, 0, 0,
					0, 0, 1
				};

				GLuint VAO;
				GLuint VBO[2];

				glGenVertexArrays(1, &VAO);
				glGenBuffers(2, VBO);
				
				glBindVertexArray(VAO);
				
				glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(position_location);
				glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
				
				glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
				glEnableVertexAttribArray(color_location);
				glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
				
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glDeleteBuffers(2, VBO);
				
				return VAO;
			}
	};
}

#endif
