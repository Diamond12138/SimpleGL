#ifndef __SIMPLEGL_MODEL_MODEL_HPP__
#define __SIMPLEGL_MODEL_MODEL_HPP__
#include <vector>

namespace SimpleGL
{
	class Model
	{
		public:
			std::vector<float> vertices;
			std::vector<float> normals;
			std::vector<float> texCoords;

		public:
			Model(std::vector<float> ver = std::vector<float>(), std::vector<float> nor = std::vector<float>(), std::vector<float> tex = std::vector<float>()) : vertices(ver), normals(nor), texCoords(tex) {}
	};
}

#endif
