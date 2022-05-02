#ifndef __SIMPLEGL_CAMERA_HPP__
#define __SIMPLEGL_CAMERA_HPP__
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

namespace SimpleGL
{
	class LookAt
	{
		public:
			const float PITCH_MAX = 89.9f;

			enum PitchDirection
			{
				POSITIVE_Y,
				ZERO,
				NEGATIVE_Y
			};
			enum YawDirection
			{
				POSITIVE_X,
				POSITIVE_Z,
				NEGATIVE_X,
				NEGATIVE_Z
			};
			enum MovementDirection
			{
				FORWARD,
				BACKWARD,
				LEFT,
				RIGHT,
				UP,
				DOWN
			};
			enum MovementSpeed
			{
				VERY_SLOW,
				SLOW,
				NORMAL,
				FAST,
				VERY_FAST,
				CUSTOM
			};

		private:
			glm::vec3 m_position;
			glm::vec3 m_front;
			glm::vec3 m_up;
			glm::vec3 m_right;
			float m_yaw;
			float m_pitch;
			glm::mat4 m_view_matrix;
			MovementSpeed m_movement_speed;
			float m_speed;
			float m_sensitivity;

		public:
			LookAt(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
			{
				m_position = position;
				m_front = front;
				m_up = up;
				calcAngle();
				update();
				m_movement_speed = MovementSpeed::NORMAL;
				m_speed = 1.0f;
				m_view_matrix = glm::mat4(1.0f);
				m_sensitivity = 0.05f;
			}
			LookAt(glm::vec3 position, float yaw, float pitch, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
			{
				m_position = position;
				m_yaw = yaw;
				m_pitch = pitch;
				m_up = up;
				update();
				m_movement_speed = MovementSpeed::NORMAL;
				m_speed = 1.0f;
				m_view_matrix = glm::mat4(1.0f);
				m_sensitivity = 0.05f;
			}

		public:
			LookAt& operator=(const LookAt& lookAt)
			{
				m_position = lookAt.m_position;
				m_front = lookAt.m_front;
				m_up = lookAt.m_up;
				m_right = lookAt.m_right;
				m_yaw = lookAt.m_yaw;
				m_pitch = lookAt.m_pitch;
				m_view_matrix = lookAt.m_view_matrix;
				m_movement_speed = lookAt.m_movement_speed;
				m_speed = lookAt.m_speed;
				m_sensitivity = lookAt.m_sensitivity;
				return *this;
			}

		public:
			LookAt& calcAngle()
			{
				m_front = glm::normalize(m_front);
				m_yaw = glm::degrees(glm::atan(m_front.z, m_front.x));
				m_pitch = glm::degrees(glm::asin(m_front.y));
				return *this;
			}

			LookAt& update()
			{
				if (m_pitch > PITCH_MAX)
					m_pitch =  PITCH_MAX;
				if (m_pitch < -PITCH_MAX)
					m_pitch = -PITCH_MAX;

				float h = glm::cos(glm::radians(m_pitch));
				float x = glm::cos(glm::radians(m_yaw)) * h;
				float y = glm::sin(glm::radians(m_pitch));
				float z = glm::sin(glm::radians(m_yaw)) * h;
				m_front = glm::normalize(glm::vec3(x, y, z));

				m_right = glm::normalize(glm::cross(m_front, m_up));
				m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
				return *this;
			}

		public:
			LookAt& setYawDirection(YawDirection direction)
			{
				if (direction == YawDirection::POSITIVE_X)
					m_yaw = 0.0f;
				else if (direction == YawDirection::POSITIVE_Z)
					m_yaw = 90.0f;
				else if (direction == YawDirection::NEGATIVE_X)
					m_yaw = 180.0f;
				else if (direction == YawDirection::NEGATIVE_Z)
					m_yaw = 270.0f;
				update();
				return *this;
			}
			LookAt& setYaw(float yaw)
			{
				m_yaw = yaw;
				update();
				return *this;
			}
			LookAt& yaw(float degree)
			{
				m_yaw += degree;
				update();
				return *this;
			}
			float getYaw()
			{
				return m_yaw;
			}

			LookAt& setPitchDirection(PitchDirection direction)
			{
				if (direction == PitchDirection::POSITIVE_Y)
					m_pitch = PITCH_MAX;
				else if (direction == PitchDirection::ZERO)
					m_pitch = 0.0f;
				else if (direction == PitchDirection::NEGATIVE_Y)
					m_pitch = -PITCH_MAX;
				update();
				return *this;
			}
			LookAt& setPitch(float pitch)
			{
				m_pitch = pitch;
				update();
				return *this;
			}
			LookAt& pitch(float degree)
			{
				m_pitch += degree;
				update();
				return *this;
			}
			float getPitch()
			{
				return m_pitch;
			}

			LookAt& setPosition(glm::vec3 position)
			{
				m_position = position;
				update();
				return *this;
			}
			LookAt& move(glm::vec3 distance, float deltaTime = 1.0f)
			{
				m_position += distance * deltaTime;
				update();
				return *this;
			}
			glm::vec3 getPosition()
			{
				return m_position;
			}

			LookAt& setFront(glm::vec3 front)
			{
				m_front = front;
				calcAngle();
				update();
				return *this;
			}
			glm::vec3 getFront()
			{
				return m_front;
			}

			LookAt& setUp(glm::vec3 up)
			{
				m_up = up;
				update();
				return *this;
			}
			glm::vec3 getUp()
			{
				return m_up;
			}

			LookAt& setMovementSpeed(MovementSpeed movement_speed, float speed = 1.0f)
			{
				m_movement_speed = movement_speed;
				if (m_movement_speed == MovementSpeed::VERY_SLOW)
					m_speed = 0.2f;
				else if (m_movement_speed == MovementSpeed::SLOW)
					m_speed = 0.5f;
				else if (m_movement_speed == MovementSpeed::NORMAL)
					m_speed = 1.0f;
				else if (m_movement_speed == MovementSpeed::FAST)
					m_speed = 2.0f;
				else if (m_movement_speed == MovementSpeed::VERY_FAST)
					m_speed = 5.0f;
				else if (m_movement_speed == MovementSpeed::CUSTOM)
					m_speed = speed;
				return *this;
			}
			float getMovementSpeed()
			{
				return m_speed;
			}

			LookAt& move(MovementDirection direction, float deltaTime = 1.0f)
			{
				if (direction == MovementDirection::FORWARD)
					m_position += m_front * m_speed * deltaTime;
				else if (direction == MovementDirection::BACKWARD)
					m_position += (-m_front) * m_speed * deltaTime;
				else if (direction == MovementDirection::LEFT)
					m_position += (-m_right) * m_speed * deltaTime;
				else if (direction == MovementDirection::RIGHT)
					m_position += m_right * m_speed * deltaTime;
				else if (direction == MovementDirection::UP)
					m_position += m_up * m_speed * deltaTime;
				else if (direction == MovementDirection::DOWN)
					m_position += (-m_up) * m_speed * deltaTime;
				update();
				return *this;
			}

			inline float clocksToSeconds(clock_t clocks)
			{
				return float(clocks) / float(CLOCKS_PER_SEC);
			}

			LookAt& rotate(float deltaX, float deltaY)
			{
				m_yaw += deltaX * m_sensitivity;
				m_pitch += deltaY * m_sensitivity;
				update();
				return *this;
			}

			LookAt& setRotateSpeed(float sensitivity)
			{
				m_sensitivity = sensitivity;
				return *this;
			}
			float getSensitivity()
			{
				return m_sensitivity;
			}

			LookAt& setViewMatrix(glm::mat4 matrix)
			{
				m_view_matrix = matrix;
				return *this;
			}
			glm::mat4 getViewMatrix()
			{
				return m_view_matrix;
			}
	};

	class Projection
	{
		public:
			const float FOVY_MIN = 0.1f;
			const float FOVY_MAX = 179.9f;

			enum ProjectionType
			{
				ORTHO,
				FRUSTUM,
				PERSPECTIVE
			};

		private:
			ProjectionType m_projection_type;
			float m_fovy;
			float m_aspect;
			float m_left;
			float m_right;
			float m_bottom;
			float m_top;
			float m_near;
			float m_far;
			glm::mat4 m_projection_matrix;

		public:
			Projection(float near_plane = 0.1f, float far_plane = 100.0f)
			{
				m_near = near_plane;
				m_far = far_plane;
				m_projection_type = Projection::PERSPECTIVE;
				m_projection_matrix = glm::mat4(1.0f);
			}
			Projection(float fovy, float aspect, float near_plane = 0.1f, float far_plane = 100.0f)
			{
				m_fovy = fovy;
				m_aspect = aspect;
				m_near = near_plane;
				m_far = far_plane;
				m_projection_type = Projection::PERSPECTIVE;
				m_projection_matrix = glm::mat4(1.0f);
			}
			Projection(float l, float r, float b, float t,  float n = 0.1f, float f = 100.0f)
			{
				m_left = l;
				m_right = r;
				m_bottom = b;
				m_top = t;
				m_near = n;
				m_far = f;
				m_projection_type = Projection::PERSPECTIVE;
				m_projection_matrix = glm::mat4(1.0f);
			}

		public:
			Projection& operator=(const Projection& projection)
			{
				m_projection_type = projection.m_projection_type;
				m_fovy = projection.m_fovy;
				m_aspect = projection.m_aspect;
				m_left = projection.m_left;
				m_right = projection.m_right;
				m_bottom = projection.m_bottom;
				m_top = projection.m_top;
				m_near = projection.m_near;
				m_far = projection.m_far;
				m_projection_matrix = projection.m_projection_matrix;
				return *this;
			}

		public:
			Projection& setProjectionType(ProjectionType type)
			{
				m_projection_type = type;
				return *this;
			}
			ProjectionType getProjectionType()
			{
				return m_projection_type;
			}

			Projection& setFovy(float fovy)
			{
				m_fovy = fovy;
				update();
				return *this;
			}
			Projection& zoom(float degree)
			{
				m_fovy += degree;
				update();
				return *this;
			}
			float getFovy()
			{
				return m_fovy;
			}

			Projection& setAspect(float aspect)
			{
				m_aspect = aspect;
				update();
				return *this;
			}
			float getAspect()
			{
				return m_aspect;
			}

			Projection& set(float l, float r, float b, float t, float n = 0.1f, float f = 100.0f)
			{
				m_left = l;
				m_right = r;
				m_bottom = b;
				m_top = t;
				m_near = n;
				m_far = f;
				update();
				return *this;
			}
			Projection& set(float fovy, float aspect, float near_plane = 0.1f, float far_plane = 100.0f)
			{
				m_fovy = fovy;
				m_aspect = aspect;
				m_near = near_plane;
				m_far = far_plane;
				update();
				return *this;
			}

			Projection& update()
			{
				if (m_fovy < FOVY_MIN)
					m_fovy = FOVY_MIN;
				else if (m_fovy > FOVY_MAX)
					m_fovy = FOVY_MAX;

				if (m_projection_type == ProjectionType::ORTHO)
					m_projection_matrix = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
				else if (m_projection_type == ProjectionType::FRUSTUM)
					m_projection_matrix = glm::frustum(m_left, m_right, m_bottom, m_top, m_near, m_far);
				else if (m_projection_type == ProjectionType::PERSPECTIVE)
					m_projection_matrix = glm::perspective(m_fovy, m_aspect, m_near, m_far);
				return *this;
			}

			Projection& setProjectionMatrix(glm::mat4 matrix)
			{
				m_projection_matrix = matrix;
				return *this;
			}
			glm::mat4 getProjectionMatrix()
			{
				return m_projection_matrix;
			}
	};

	class Camera : public LookAt, public Projection
	{
		public:
			Camera() : LookAt(), Projection() {}
			Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : LookAt(position, front, up), Projection() {}
			Camera(glm::vec3 position, float yaw, float pitch, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : LookAt(position, yaw, pitch, up), Projection() {}
	};
}

#endif
