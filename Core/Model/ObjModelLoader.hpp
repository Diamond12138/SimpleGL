#ifndef __SIMPLEGL_CORE_MODEL_OBJMODELLOADER_HPP__
#define __SIMPLEGL_CORE_MODEL_OBJMODELLOADER_HPP__
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include "Model.hpp"
#include "ModelBuilder.hpp"

namespace SimpleGL
{
	class ObjModelLoader
	{
		public:
			typedef std::pair<std::string, Model> ResultType;
			typedef std::vector<std::pair<std::string, Model>> ResultArrayType;
		
		public:
			std::vector<std::pair<std::string, Model>> results;
			bool success;
			std::stringstream error_log;

		public:
			ObjModelLoader() : success(false) {}
			ObjModelLoader(std::string filename)
			{
				loadFromFile(filename);
			}

		public:
			bool loadFromStream(std::istream& is)
			{
				bool result = true;
				std::string groupname = "<default>";

				std::vector<glm::vec3> vertices0;
				std::vector<glm::vec3> normals0;
				std::vector<glm::vec3> texCoords0;

				std::vector<glm::vec3> vertices1;
				std::vector<glm::vec3> normals1;
				std::vector<glm::vec3> texCoords1;

				try
				{
					std::string temp;
					while (getline(is, temp))
					{
						std::vector<std::string> items = split(temp, "[ ]");
						int length = items.size();
						if (items.at(0).compare("v") == 0)
						{
							float x = std::stof(items.at(1));
							float y = std::stof(items.at(2));
							float z = std::stof(items.at(3));
							vertices0.push_back(glm::vec3(x, y, z));
						}
						else if (items.at(0).compare("vn") == 0)
						{
							float x = std::stof(items.at(1));
							float y = std::stof(items.at(2));
							float z = std::stof(items.at(3));
							normals0.push_back(glm::vec3(x, y, z));
						}
						else if (items.at(0).compare("vt") == 0)
						{
							float x = std::stof(items.at(1));
							float y = std::stof(items.at(2));
							float z = 0.0f;
							if (length > 3)
								z = std::stof(items.at(3));
							texCoords0.push_back(glm::vec3(x, y, z));
						}
						else if (items.at(0).compare("f") == 0)
						{
							std::vector<std::vector<int>> indices;
							for (int i = 1; i < length; i++)
							{
								std::vector<std::string> values0 = split(items.at(i), "[/]");
								std::vector<int> values1;
								for (int j = 0; j < values0.size(); j++)
								{
									values1.push_back(std::stoi(values0.at(j)) - 1);
								}
								indices.push_back(values1);
							}

							if (vertices0.size() != 0)
							{
								for (int i = 2; i < indices.size(); i++)
								{
									vertices1.push_back(vertices0.at(indices.at(0).at(0)));
									vertices1.push_back(vertices0.at(indices.at(i - 1).at(0)));
									vertices1.push_back(vertices0.at(indices.at(i).at(0)));
								}
							}

							if (normals0.size() != 0)
							{
								for (int i = 2; i < indices.size(); i++)
								{
									normals1.push_back(normals0.at(indices.at(0).at(2)));
									normals1.push_back(normals0.at(indices.at(i - 1).at(2)));
									normals1.push_back(normals0.at(indices.at(i).at(2)));
								}
							}

							if (texCoords0.size() != 0)
							{
								for (int i = 2; i < indices.size(); i++)
								{
									texCoords1.push_back(texCoords0.at(indices.at(0).at(1)));
									texCoords1.push_back(texCoords0.at(indices.at(i - 1).at(1)));
									texCoords1.push_back(texCoords0.at(indices.at(i).at(1)));
								}
							}
						}
						else if (items.at(0).compare("g") == 0)
						{
							push_result(groupname, vertices1, normals1, texCoords1);
							groupname = items.at(1);
							vertices1 = std::vector<glm::vec3>();
							normals1 = std::vector<glm::vec3>();
							texCoords1 = std::vector<glm::vec3>();
						}
					}
				}
				catch (std::out_of_range& e)
				{
					error_log << e.what() << std::endl;
					result = false;
				}
				push_result(groupname, vertices1, normals1, texCoords1);
				return result;
			}

			bool loadFromFile(std::string filename)
			{
				std::ifstream fin;
				try
				{
					fin.open(filename);
					if (!fin.is_open())
					{
						error_log << "Failed to open file." << std::endl;
						success = false;
						return false;
					}
					success = loadFromStream(fin);
					fin.close();
				}
				catch (std::ifstream::failure& e)
				{
					error_log << e.what() << std::endl;
					success = false;
					return false;
				}
				return true;
			}

			void push_result(std::string groupname, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> texCoords)
			{
				if (vertices.size() != 0)
				{
					std::vector<float> ver = ModelBuilder::toFloatArray(vertices);
					std::vector<float> nor;
					if (normals.size() == 0)
					{
						nor = ModelBuilder::calcNormals(ver);
					}
					else
					{
						nor = ModelBuilder::toFloatArray(normals);
					}
					std::vector<float> tex = ModelBuilder::toFloatArray(texCoords);
					results.push_back(std::pair<std::string, Model>(groupname, Model(ver, nor, tex)));
				}
			}

		public:
			static std::vector<std::string> split(std::string data, std::string reg)
			{
				std::vector<std::string> res;
				std::regex regex("(.*)" + reg + "(.*)");
				std::smatch m;
				std::string last;
				for (; std::regex_search(data, m, regex); data = m.str(1))
				{
					last = m.str(1);
					res.push_back(m.str(2));
				}
				res.push_back(last);

				std::vector<std::string> result;
				for (int i = res.size() - 1; i >= 0; i--)
				{
					result.push_back(res.at(i));
				}

				return result;
			}
	};
}

#endif
