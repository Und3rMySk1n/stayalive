#include "stdafx.h"
#include "Key.h"

namespace
{
	struct CKeyTesselator : SMeshDataP3NT2
	{
	public:

		void Tesselate(glm::vec2 position)
		{
			MakeVertexAttributes({ position.x - 0.2f, 0.5f, position.y - 0.2f });
			MakeVertexAttributes({ position.x - 0.2f, 0.5f, position.y + 0.2f });
			MakeVertexAttributes({ position.x + 0.2f, 0.5f, position.y + 0.2f });
			MakeVertexAttributes({ position.x + 0.2f, 0.5f, position.y - 0.2f });

			MakeVertexAttributes({ position.x - 0.2f, 1.5f, position.y - 0.2f });
			MakeVertexAttributes({ position.x - 0.2f, 1.5f, position.y + 0.2f });
			MakeVertexAttributes({ position.x + 0.2f, 1.5f, position.y + 0.2f });
			MakeVertexAttributes({ position.x + 0.2f, 1.5f, position.y - 0.2f });

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
				2, 3, 0,

				0, 4, 1,
				1, 4, 5,

				1, 2, 5,

			};
		}
	};
}

CKey::CKey(glm::vec2 position)
	: m_position(position)
	, m_mesh(MeshType::TriangleStrip)
{
	CKeyTesselator tesselator;
	tesselator.Tesselate(m_position);
	m_mesh.Copy(tesselator);
}

void CKey::Draw(IRenderer3D &renderer) const
{
	m_mesh.Draw(renderer);
}

CKey::~CKey()
{
}
