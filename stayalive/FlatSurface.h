#pragma once
class FlatSurface
{
public:
	FlatSurface();

	void Draw();
	void SetHeight(float height);
	void Tesselate(unsigned slices, unsigned stacks);

	~FlatSurface();
private:
	float m_height = 0;
	std::vector<uint32_t> m_indicies;
	std::vector<glm::vec3> m_vertices;
};

