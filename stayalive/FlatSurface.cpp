#include "stdafx.h"
#include "FlatSurface.h"

namespace
{
	struct CFlatSurfaceTesselator : SMeshDataP3NT2
	{
	public:
		static const unsigned MIN_PRECISION = 1;

		void Tesselate(unsigned slices, unsigned stacks, float height, unsigned tileSize)
		{
			assert((slices >= MIN_PRECISION) && (stacks >= MIN_PRECISION));
			MakeVertexAttributes(slices, stacks, tileSize, height);
			unsigned columnCount = slices * tileSize + 1;
			unsigned rowCount = stacks * tileSize + 1;
			MakeTriangleStripIndicies(columnCount, rowCount);
		}

	private:
		void MakeVertexAttributes(unsigned slices, unsigned stacks, unsigned tileSize, float height)
		{
			for (unsigned ci = 0; ci <= slices * tileSize; ++ci)
			{
				float x = (float)ci;
				for (unsigned ri = 0; ri <= stacks * tileSize; ++ri)
				{
					float z = (float)ri;
					SVertexP3NT2 vertex;

					vertex.position = { x, height, z };

					glm::vec3 offsetX = { x + 0.1f, height, z }; 
					glm::vec3 dirX = offsetX - vertex.position;
					glm::vec3 offsetZ = { x, height, z + 0.1f };
					glm::vec3 dirZ = offsetZ - vertex.position;

					vertex.normal = glm::normalize(glm::cross(dirX, dirZ));
					vertex.texCoord = { x, z };

					vertices.push_back(vertex);
				}
			}			
		}

		void MakeTriangleStripIndicies(unsigned columnCount, unsigned rowCount)
		{
			indicies.clear();
			indicies.reserve((columnCount - 1) * rowCount * 2);
			// вычисляем индексы вершин.
			for (unsigned ci = 0; ci < columnCount - 1; ++ci)
			{
				if (ci % 2 == 0)
				{
					for (unsigned ri = 0; ri < rowCount; ++ri)
					{
						unsigned index = ci * rowCount + ri;
						indicies.push_back(index + rowCount);
						indicies.push_back(index);
					}
				}
				else
				{
					for (unsigned ri = rowCount - 1; ri < rowCount; --ri)
					{
						unsigned index = ci * rowCount + ri;
						indicies.push_back(index);
						indicies.push_back(index + rowCount);
					}
				}
			}
		}		
	};
}

FlatSurface::FlatSurface(unsigned slices, unsigned stacks, float height, unsigned tileSize)
	: m_mesh(MeshType::TriangleStrip)
	, m_height(height)
{
	CFlatSurfaceTesselator tesselator;
	tesselator.Tesselate(slices, stacks, m_height, tileSize);
	m_mesh.Copy(tesselator);
}

void FlatSurface::Draw(IRenderer3D &renderer) const
{
	m_mesh.Draw(renderer);
}

FlatSurface::~FlatSurface()
{
}
