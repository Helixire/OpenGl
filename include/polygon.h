#ifndef POLYGON_H
# define POLYGON_H

// Include openGL

# ifdef WIN32
#  include <GL/glew.h>
# elif __APPLE__
#  define GL3_PROTOTYPES 1
#  include <OpenGL/gl3.h>
# else
#  define GL3_PROTOTYPES 1
#  include <GLES3/gl3.h>
# endif

// Include GLM

# define GLM_FORCE_RADIANS
# include <glm/glm.hpp>
# include <glm/gtx/transform.hpp>
# include <glm/gtc/type_ptr.hpp>

class Polygon
{
	public:
		Polygon(float *vert = NULL, const uint vertLen = 0, float *color = NULL);
		Polygon(float *vert, const uint vertLen, float *texture, const GLuint texture_id);
		~Polygon();

		bool	load();
		void	draw(const GLuint shader, const glm::mat4 viewProj);

		void	setPos(glm::vec3 pos);
		void	setRot(glm::mat4 rot);
		void	setScale(glm::vec3 scale);

		glm::vec3	getPos() const;
		glm::mat4	getRot() const;
		glm::vec3	getScale() const;

	protected:
		void	update_transform(void);

		// draw
		float	*m_vert;
		float	*m_add;
		uint	m_vertSize;
		uint	m_addSize;
		GLuint	m_vbo;
		GLuint	m_vao;
		GLuint	m_texture;

		//pos
		glm::vec3	m_pos;
		glm::mat4	m_rot;
		glm::vec3	m_scale;
		glm::mat4	m_transform;
};

#endif // POLYGON_H
