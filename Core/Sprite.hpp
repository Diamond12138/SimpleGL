#ifndef __SIMPLEGL_CORE_SPRITE_HPP__
#define __SIMPLEGL_CORE_SPRITE_HPP__
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SimpleGL
{
	class Sprite
	{
		private:
			glm::vec3 m_position;
			glm::vec3 m_rotate;
			glm::vec3 m_scale;
			glm::mat4 m_model_matrix;

		public:
			Sprite(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
			{
				m_position = position;
				m_rotate = rotate;
				m_scale = scale;
				m_model_matrix = glm::mat4(1.0f);
			}

		public:
			Sprite& update()
			{
				m_model_matrix = glm::mat4(1.0f);
				m_model_matrix = glm::translate(m_model_matrix, m_position);
				m_model_matrix = glm::rotate(m_model_matrix, m_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
				m_model_matrix = glm::rotate(m_model_matrix, m_rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
				m_model_matrix = glm::rotate(m_model_matrix, m_rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
				m_model_matrix = glm::scale(m_model_matrix, m_scale);
				return *this;
			}

			Sprite& setModelMatrix(glm::mat4 matrix)
			{
				m_model_matrix = matrix;
				return *this;
			}
			glm::mat4 getModelMatrix()
			{
				return m_model_matrix;
			}

			Sprite& setPosition(glm::vec3 position)
			{
				m_position = position;
				update();
				return *this;
			}
			Sprite& translate(glm::vec3 distance)
			{
				m_position += distance;
				update();
				return *this;
			}
			glm::vec3 getPosition()
			{
				return m_position;
			}

			Sprite& setRotate(glm::vec3 degree)
			{
				m_rotate = degree;
				update();
				return *this;
			}
			Sprite& rotate(glm::vec3 degree)
			{
				m_rotate += degree;
				update();
				return *this;
			}
			glm::vec3 getRotate()
			{
				return m_rotate;
			}

			Sprite& setScale(glm::vec3 size)
			{
				m_scale = size;
				update();
				return *this;
			}
			Sprite& scale(glm::vec3 size)
			{
				m_scale += size;
				update();
				return *this;
			}
			glm::vec3 getScale()
			{
				return m_scale;
			}
	};
}

#endif
