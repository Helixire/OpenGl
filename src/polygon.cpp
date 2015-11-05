#include "polygon.h"

Polygon::Polygon(float *vert, const uint vertLen, float *color) :
	m_vert(vert), m_add(color), m_vertSize(vertLen), m_addSize(vertLen),
	m_vbo(0), m_vao(0), m_texture(0), m_pos(glm::vec3(0.0, 0.0, 0.0)), m_rot(glm::mat4(1.0)), m_scale(glm::vec3(1.0, 1.0, 1.0))
{}

Polygon::Polygon(float *vert, const uint vertLen, float *texture, const GLuint texture_id) :
	m_vert(vert), m_add(texture), m_vertSize(vertLen), m_addSize(2 * vertLen / 3),
	m_vbo(0), m_vao(0), m_texture(texture_id), m_pos(glm::vec3(0.0, 0.0, 0.0)), m_rot(glm::mat4(1.0)), m_scale(glm::vec3(1.0, 1.0, 1.0))
{}

Polygon::~Polygon()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

bool Polygon::load()
{
	const uint vertSize = m_vertSize * sizeof(float);
	const uint addSize = m_addSize * sizeof(float);

	//VBO

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	{
		glBufferData(GL_ARRAY_BUFFER, vertSize + addSize, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertSize, m_vert);
		glBufferSubData(GL_ARRAY_BUFFER, vertSize, addSize, m_add);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//VA0

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		if (!m_texture)
		{
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (char *)NULL + vertSize);
			glEnableVertexAttribArray(1);
		}
		else
		{
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (char *)NULL + vertSize);
			glEnableVertexAttribArray(2);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
	return (true);
}

void Polygon::draw(const GLuint shader, const glm::mat4 viewProj)
{
	const glm::mat4	mvp = viewProj * m_transform;

	glBindVertexArray(m_vao);

	glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, m_vertSize);

	glBindVertexArray(0);
}

void Polygon::setPos(glm::vec3 pos)
{
	if (pos != m_pos)
	{
		m_pos = pos;
		update_transform();
	}
}

void Polygon::setRot(glm::mat4 rot)
{
	if (rot != m_rot)
	{
		m_rot = rot;
		update_transform();
	}
}

void Polygon::setScale(glm::vec3 scale)
{
	if (scale != m_scale)
	{
		m_scale = scale;
		update_transform();
	}
}

glm::vec3 Polygon::getPos() const
{
	return (m_pos);
}

glm::mat4 Polygon::getRot() const
{
	return (m_rot);
}

glm::vec3 Polygon::getScale() const
{
	return (m_scale);
}

void Polygon::update_transform()
{
	m_transform = glm::translate(m_pos) * m_rot * glm::scale(m_scale);
}

