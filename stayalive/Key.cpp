#include "stdafx.h"
#include "Key.h"

namespace
{
	struct CKeyTesselator : SMeshDataP3NT2
	{
	public:

		void Tesselate(glm::vec2 position)
		{
			MakeVertexAttributes({ position.x, 0.25f, position.y });

			MakeVertexAttributes({ position.x - 0.2f, 0.75f, position.y - 0.2f });
			MakeVertexAttributes({ position.x - 0.2f, 0.75f, position.y + 0.2f });
			MakeVertexAttributes({ position.x + 0.2f, 0.75f, position.y + 0.2f });
			MakeVertexAttributes({ position.x + 0.2f, 0.75f, position.y - 0.2f });

			MakeVertexAttributes({ position.x, 1.25f, position.y });

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
				0, 2, 3,
				0, 3, 4,
				0, 4, 1,

				5, 1, 2,
				5, 2, 3,
				5, 3, 4,
				5, 4, 1
			};
		}
	};
}

CKey::CKey()
	: m_mesh(MeshType::TriangleStrip)
{
}

void CKey::SetPosition(glm::vec2 position)
{
	m_position = position;
	CKeyTesselator tesselator;
	tesselator.Tesselate(m_position);
	m_mesh.Copy(tesselator);
}

bool CKey::Reached(glm::vec2 position)
{
	if ((position.x > m_position.x - m_pickUpRadius) && (position.x < m_position.x + m_pickUpRadius)
		&& (position.y > m_position.y - m_pickUpRadius) && (position.y < m_position.y + m_pickUpRadius))
	{
		return true;
	}
	return false;
}

void CKey::Draw(IRenderer3D &renderer) const
{
	renderer.SetColormapSlot(3);
	m_mesh.Draw(renderer);
}

CKey::~CKey()
{
}
