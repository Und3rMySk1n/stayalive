#include "stdafx.h"
#include "SolidBlock.h"

namespace
{
	struct CSolidBlockTesselator : SMeshDataP3NT2
	{
	public:

		void Tesselate(glm::vec2 topLeft, glm::vec2 topRight, glm::vec2 bottomLeft, glm::vec2 bottomRight, float height)
		{
			MakeVertexAttributes({ topLeft.x, 0, topLeft.y });
			MakeVertexAttributes({ topLeft.x, height, topLeft.y });
			MakeVertexAttributes({ bottomLeft.x, 0, bottomLeft.y });
			MakeVertexAttributes({ bottomLeft.x, height, bottomLeft.y });
			MakeVertexAttributes({ bottomRight.x, 0, bottomRight.y });
			MakeVertexAttributes({ bottomRight.x, height, bottomRight.y });
			MakeVertexAttributes({ topRight.x, 0, topRight.y });
			MakeVertexAttributes({ topRight.x, height, topRight.y });

			MakeTriangleStripIndicies();
		}

	private:
		void MakeVertexAttributes(glm::vec3 blockVertex)
		{
			SVertexP3NT2 vertex;

			vertex.position = { blockVertex };

			glm::vec3 offsetX = { blockVertex.x + 0.1f, blockVertex.y, blockVertex.z + 0.1f };
			glm::vec3 dirX = offsetX - vertex.position;
			glm::vec3 offsetZ = { blockVertex.x, blockVertex.y + 0.1f, blockVertex.z };
			glm::vec3 dirZ = offsetZ - vertex.position;

			vertex.normal = glm::normalize(glm::cross(dirX, dirZ));
			vertex.texCoord = { blockVertex.x + blockVertex.z, blockVertex.y };

			vertices.push_back(vertex);
		}

		void MakeTriangleStripIndicies()
		{
			indicies = { 
				0, 1, 2,
				1, 2, 3,
				2, 3, 4,
				3, 4, 5,
				4, 5, 6,
				5, 6, 7,
				6, 7, 0,
				7, 0, 1
			};
		}
	};
}

SolidBlock::SolidBlock(glm::vec2 topLeft, glm::vec2 topRight, glm::vec2 bottomLeft, glm::vec2 bottomRight, float height)
	: m_mesh(MeshType::TriangleStrip)
	, m_topLeft(topLeft)
	, m_topRight(topRight)
	, m_bottomLeft(bottomLeft)
	, m_bottomRight(bottomRight)
	, m_height(height)
{
	CSolidBlockTesselator tesselator;
	tesselator.Tesselate(m_topLeft, m_topRight, m_bottomLeft, m_bottomRight, m_height);
	m_mesh.Copy(tesselator);
}

bool SolidBlock::IsPointInBlock(glm::vec2 point, float offset)
{
	if ((point.x > (m_topLeft.x - offset) && point.x < (m_topRight.x + offset)) &&
		(point.y > (m_topLeft.y - offset) && point.y < (m_bottomLeft.y + offset)))
	{
		return true;
	}
	
	return false;	
}

void SolidBlock::Draw(IRenderer3D &renderer) const
{
	m_mesh.Draw(renderer);
}

glm::vec2 SolidBlock::GetCorrectedPoint(glm::vec2 point, glm::vec2 newPoint, float offset, bool &moveBlocked)
{
	glm::vec2 resultPoint;
	if ((point.x > (m_topLeft.x - offset) && point.x < (m_topRight.x + offset)))
	{
		resultPoint.x = newPoint.x;
		resultPoint.y = point.y;
		moveBlocked = false;
	}
	else if ((point.y > (m_topLeft.y - offset) && point.y < (m_bottomLeft.y + offset)))
	{
		resultPoint.x = point.x;
		resultPoint.y = newPoint.y;
		moveBlocked = false;
	}
	else
	{
		resultPoint.x = newPoint.x;
		resultPoint.y = newPoint.y;
		moveBlocked = true;

		std::cout << "X: " << m_topLeft.x - offset << " - " << m_bottomRight.x + offset << std::endl;
		std::cout << "Y: " << m_topLeft.y - offset << " - " << m_bottomRight.y + offset << std::endl;
	}

	return resultPoint;
}


SolidBlock::~SolidBlock()
{
}
