#pragma once
#include "MeshP3NT2.h"

class FlatSurface
{
public:
	FlatSurface(unsigned slices, unsigned stacks, float height, unsigned tileSize);

	void Draw(IRenderer3D &renderer) const;

	~FlatSurface();
private:
	float m_height = 0;
	std::vector<uint32_t> m_indicies;
	std::vector<glm::vec3> m_vertices;
	CMeshP3NT2 m_mesh;
};

