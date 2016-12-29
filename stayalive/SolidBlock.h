#pragma once
#include "MeshP3NT2.h"

class SolidBlock
{
public:
	SolidBlock(glm::vec2 topLeft, glm::vec2 topRight, glm::vec2 bottomLeft, glm::vec2 bottomRight, float height);

	void Draw(IRenderer3D &renderer) const;
	bool IsPointInBlock(glm::vec2 point, float offset);

	~SolidBlock();
private:
	glm::vec2 m_topLeft = { 0, 0 };
	glm::vec2 m_topRight = { 0, 0 };
	glm::vec2 m_bottomLeft = { 0, 0 };
	glm::vec2 m_bottomRight = { 0, 0 };

	float m_height = 0;
	std::vector<glm::vec3> m_verticies;
	CMeshP3NT2 m_mesh;
};

