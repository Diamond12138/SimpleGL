#ifndef __SIMPLEGL_TEXTURE_HPP__
#define __SIMPLEGL_TEXTURE_HPP__
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "SimpleGL.hpp"

namespace SimpleGL
{
	class Texture
	{
		public:
			class TextureData
			{
				public:
					int width;
					int height;
					int nrChannels;
					unsigned char* data;
					bool available;

				public:
					TextureData() : width(0), height(0), nrChannels(0), data(nullptr), available(false) {}

				public:
					TextureData& loadData(std::string filename)
					{
						data = stbi_load(filename.data(), &width, &height, &nrChannels, 0);
						if (data != nullptr)
						{
							available = true;
						}
						return *this;
					}

					TextureData& deleteData()
					{
						stbi_image_free(data);
						available = false;
						return *this;
					}
			};

			static void bindTexture0(GLenum type)
			{
				glBindTexture(type, 0);
			}

			static TextureData loadFromFile(std::string filename)
			{
				TextureData td;
				td.loadData(filename);
				return td;
			}

		private:
			GLuint m_textureID;
			GLenum m_type;

		public:
			Texture(GLenum type) : m_textureID(0), m_type(type)
			{

			}

		public:
			Texture& setTextureID(GLuint id)
			{
				m_textureID = id;
				return *this;
			}
			GLuint getTextureID()
			{
				return m_textureID;
			}
			GLenum getTextureType()
			{
				return m_type;
			}

		public:
			bool genTexture()
			{
				deleteTexture();
				glGenTextures(1, &m_textureID);
				return (m_textureID != 0);
			}

			Texture& bindTexture()
			{
				glBindTexture(m_type, m_textureID);
				return *this;
			}

			Texture& bindTexture0()
			{
				bindTexture0(m_type);
				return *this;
			}

			Texture& deleteTexture()
			{
				if (m_textureID != 0)
				{
					glDeleteTextures(1, &m_textureID);
				}
				return *this;
			}
			
			Texture& bindAsAttachment(GLenum internalformat,int width,int height,GLenum format,GLenum data_type,GLenum attachment_type)
			{
				glBindTexture(m_type, m_textureID);
				glTexImage2D(m_type, 0, internalformat, width, height, 0, format,data_type, nullptr);
				glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glBindTexture(m_type, 0);
				
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, m_type, m_textureID, 0);  
				return *this;
			}
	};

	class Texture2D : public Texture
	{
		public:
			Texture2D() : Texture(GL_TEXTURE_2D) {}

		public:
			bool bindData(std::string filename)
			{
				glBindTexture(getTextureType(), getTextureID());

				glTexParameteri(getTextureType(), GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(getTextureType(), GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(getTextureType(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(getTextureType(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				TextureData td = loadFromFile(filename);
				if (!td.available)
				{
					return false;
				}
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, td.width, td.height, 0, GL_RGB, GL_UNSIGNED_BYTE, td.data);
				glGenerateMipmap(GL_TEXTURE_2D);
				td.deleteData();

				return true;
			}
	};
}

#endif
