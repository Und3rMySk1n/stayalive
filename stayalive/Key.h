#pragma once
#include "LabyrinthObject.h"
#include "MeshP3NT2.h"

class CKey
	: public CLabyrinthObject
{
public:
	CKey(glm::vec2 position);

	void Draw(IRenderer3D &renderer) const;

	~CKey();
private:
	glm::vec2 m_position;
	CMeshP3NT2 m_mesh;
};

