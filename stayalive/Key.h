#pragma once
#include "LabyrinthObject.h"
#include "MeshP3NT2.h"

class CKey
	: public CLabyrinthObject
{
public:
	CKey();

	void SetPosition(glm::vec2 position);
	void Draw(IRenderer3D &renderer) const;
	bool Reached(glm::vec2 position);

	~CKey();
private:
	glm::vec2 m_position = { 0.0f, 0.0f };
	float m_pickUpRadius = 0.5f;
	CMeshP3NT2 m_mesh;
};

