#ifndef __SIMPLEGL_BUFFER_HPP__
#define __SIMPLEGL_BUFFER_HPP__
#include "gl.hpp"

namespace SimpleGL
{
	class BufferObject
	{
		private:
			GLuint m_bufferObjectID;
			GLenum m_type;

		public:
			static void bindBuffer0(GLenum type)
			{
				glBindBuffer(type, 0);
			}

		public:
			BufferObject(GLenum type)
			{
				m_type = type;
				genBuffer();
				bindBuffer();
				bindBuffer0();
			}

			~BufferObject()
			{
				deleteBuffer();
			}

		public:
			BufferObject& setBufferID(GLuint id)
			{
				deleteBuffer();
				m_bufferObjectID = id;
				return *this;
			}

			GLuint getBufferID()
			{
				return m_bufferObjectID;
			}

			const GLuint getBufferType()
			{
				return m_type;
			}

		public:
			bool genBuffer()
			{
				deleteBuffer();
				glGenBuffers(1, &m_bufferObjectID);
				return (m_bufferObjectID != 0);
			}

			BufferObject& bindBuffer()
			{
				if (m_bufferObjectID != 0)
				{
					glBindBuffer(m_type, m_bufferObjectID);
				}
				return *this;
			}
			BufferObject& bindBuffer0()
			{
				bindBuffer0(m_type);
				return *this;
			}

			BufferObject& deleteBuffer()
			{
				if (m_bufferObjectID != 0)
				{
					glDeleteBuffers(1, &m_bufferObjectID);
					m_bufferObjectID = 0;
				}
				return *this;
			}
	};

	class VertexBufferObject : public BufferObject
	{
		public:
			VertexBufferObject() : BufferObject(GL_ARRAY_BUFFER) {}
	};

	class ElementBufferObject : public BufferObject
	{
		public:
			ElementBufferObject() : BufferObject(GL_ELEMENT_ARRAY_BUFFER) {}
	};

	class VertexArrayObject
	{
		private:
			GLuint m_VertexArrayObjectID;

		public:
			static void bindVertexArray0()
			{
				glBindVertexArray(0);
			}

		public:
			VertexArrayObject()
			{
				genVertexArray();
				bindVertexArray();
				bindVertexArray0();
			}
			~VertexArrayObject()
			{
				deleteVertexArray();
			}

		public:
			VertexArrayObject& setVertexArrayID(GLuint id)
			{
				deleteVertexArray();
				m_VertexArrayObjectID = id;
				return *this;
			}
			GLuint getVertexArrayID()
			{
				return m_VertexArrayObjectID;
			}

		public:
			bool genVertexArray()
			{
				deleteVertexArray();
				glGenVertexArrays(1, &m_VertexArrayObjectID);
				return (m_VertexArrayObjectID != 0);
			}

			VertexArrayObject& bindVertexArray()
			{
				if (m_VertexArrayObjectID != 0)
				{
					glBindVertexArray(m_VertexArrayObjectID);
				}
				return *this;
			}

			VertexArrayObject& deleteVertexArray()
			{
				if (m_VertexArrayObjectID != 0)
				{
					glDeleteVertexArrays(1, &m_VertexArrayObjectID);
					m_VertexArrayObjectID = 0;
				}
				return *this;
			}
	};

	class FramebufferObject
	{
		public:
			static void bindFramebuffer0()
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			static bool checkIsComplete()
			{
				return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
			}

		private:
			GLuint m_framebufferObjectID;

		public:
			FramebufferObject()
			{
				genFramebuffer();
			}

			~FramebufferObject()
			{
				deleteFramebuffer();
			}

		public:
			FramebufferObject& setFramebufferID(GLuint id)
			{
				deleteFramebuffer();
				m_framebufferObjectID = id;
				return *this;
			}

			GLuint getFramebufferID()
			{
				return m_framebufferObjectID;
			}

		public:
			bool genFramebuffer()
			{
				deleteFramebuffer();
				glGenFramebuffers(1, &m_framebufferObjectID);
				return (m_framebufferObjectID != 0);
			}

			FramebufferObject& bindFramebuffer()
			{
				if (m_framebufferObjectID != 0)
					glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObjectID);
				return *this;
			}

			FramebufferObject& deleteFramebuffer()
			{
				if (m_framebufferObjectID != 0)
					glDeleteFramebuffers(1, &m_framebufferObjectID);
				return *this;
			}

		public:

	};

	class RenderbufferObject
	{
		public:
			static void bindRenderbuffer0()
			{
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}

		private:
			GLuint m_renderbufferObjectID;

		public:
			RenderbufferObject()
			{
				genRenderbuffer();
			}

			~RenderbufferObject()
			{
				deleteRenderbuffer();
			}

		public:
			RenderbufferObject& setRenderbufferID(GLuint id)
			{
				deleteRenderbuffer();
				m_renderbufferObjectID = id;
				return *this;
			}

			GLuint getRenderbufferID()
			{
				return m_renderbufferObjectID;
			}

		public:
			bool genRenderbuffer()
			{
				deleteRenderbuffer();
				glGenRenderbuffers(1, &m_renderbufferObjectID);
				return (m_renderbufferObjectID != 0);
			}

			RenderbufferObject& bindRenderbuffer()
			{
				if (m_renderbufferObjectID != 0)
					glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferObjectID);
				return *this;
			}

			RenderbufferObject& deleteRenderbuffer()
			{
				if (m_renderbufferObjectID != 0)
					glDeleteRenderbuffers(1, &m_renderbufferObjectID);
				return *this;
			}

		public:
			RenderbufferObject& bindAsAttachment(GLenum internalformat,int width,int height,GLenum attachment_type)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferObjectID); 
				glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width,height);  
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment_type, GL_RENDERBUFFER, m_renderbufferObjectID);
				return *this;
			}
	};
}

#endif
