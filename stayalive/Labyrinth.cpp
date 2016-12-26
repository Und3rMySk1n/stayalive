#include "stdafx.h"
#include "Labyrinth.h"

namespace
{
	struct CWallTesselator : SMeshDataP3NT2
	{
	public:

		void AddTesselatedWall(glm::vec2 startBottom, glm::vec2 endBottom, float height)
		{
			MakeVertexAttributes({ startBottom.x, 0, startBottom.y });
			MakeVertexAttributes({ startBottom.x, height, startBottom.y });
			MakeVertexAttributes({ endBottom.x, 0, endBottom.y });
			MakeVertexAttributes({ endBottom.x, height, endBottom.y });

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
			if (empty(indicies))
			{
				indicies = {
					0, 1, 2,
					1, 2, 3
				};
			}
			else
			{
				uint32_t lastVertix = indicies.back();
				indicies.insert(indicies.end(), {
					lastVertix + 1, lastVertix + 2, lastVertix + 3,
					lastVertix + 2, lastVertix + 3, lastVertix + 4
				});
			}
		}
	};
}

Labyrinth::Labyrinth(unsigned slices, unsigned stacks, float height, unsigned tileSize)
	: m_slices(slices)
	, m_stacks(stacks)
	, m_height(height)
	, m_tileSize(tileSize)
	, m_floor(slices, stacks, 0, tileSize)
	, m_roof(slices, stacks, height, tileSize)
	, m_wallsMesh(MeshType::TriangleStrip)
{

}

WallPointsArrayType Labyrinth::GetWallsFromBlocksArray(BlockArrayType blocksArray)
{
	WallPointsArrayType wallPointsArray;
	
	for (unsigned widthNum = 0; widthNum < m_slices; widthNum++)
	{
		for (unsigned lengthNum = 0; lengthNum < m_stacks; lengthNum++)
		{
			if (blocksArray[widthNum][lengthNum] == 1)
			{
				glm::vec2 topLeft = { widthNum * m_tileSize, lengthNum * m_tileSize };
				glm::vec2 topRight = { (widthNum + 1) * m_tileSize, lengthNum * m_tileSize };
				glm::vec2 bottomLeft = { widthNum * m_tileSize, (lengthNum + 1) * m_tileSize };
				glm::vec2 bottomRight = { (widthNum + 1) * m_tileSize, (lengthNum + 1) * m_tileSize };

				std::unique_ptr<SolidBlock> block(new SolidBlock(topLeft, topRight, bottomLeft, bottomRight, m_height));
				m_blocks.push_back(std::move(block));
			}
		}
	}

	return wallPointsArray;
}

void Labyrinth::AddWalls(BlockArrayType blocksArray)
{
	WallPointsArrayType wallPointsArray = GetWallsFromBlocksArray(blocksArray);

	CWallTesselator wallTesselator;
	for (auto it = wallPointsArray.begin(); it != wallPointsArray.end(); it++)
	{
		wallTesselator.AddTesselatedWall(it[0][0], it[0][1], m_height);
	}

	m_wallsMesh.Copy(wallTesselator);
}

glm::vec2 Labyrinth::GetObjectPosition(BlockArrayType blocksArray, int objectId)
{
	glm::vec2 playerPosition;

	for (unsigned widthNum = 0; widthNum < m_slices; widthNum++)
	{
		for (unsigned lengthNum = 0; lengthNum < m_stacks; lengthNum++)
		{
			if (blocksArray[widthNum][lengthNum] == objectId)
			{
				playerPosition.x = (float)(widthNum * m_tileSize) + (m_tileSize / 2.f);
				playerPosition.y = (float)(lengthNum * m_tileSize) + (m_tileSize / 2.f);
			}
		}
	}

	return playerPosition;
}

glm::vec2 Labyrinth::CorrectActorMovement(glm::vec2 point, glm::vec2 newPoint, float offset)
{
	bool moveBlocked = false;
	glm::vec2 checkPoint = newPoint;

	for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
	{
		bool pointInBlock = (*it)->IsPointInBlock(checkPoint, offset);
		if (pointInBlock)
		{
			moveBlocked = true;
			break;
		}
	}

	if (moveBlocked)
	{
		moveBlocked = false;
		checkPoint.x = newPoint.x;
		checkPoint.y = point.y;

		for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
		{
			bool pointInBlock = (*it)->IsPointInBlock(checkPoint, offset);
			if (pointInBlock)
			{
				moveBlocked = true;
				break;
			}
		}		
	}

	if (moveBlocked)
	{
		moveBlocked = false;
		checkPoint.x = point.x;
		checkPoint.y = newPoint.y;

		for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
		{
			bool pointInBlock = (*it)->IsPointInBlock(checkPoint, offset);
			if (pointInBlock)
			{
				moveBlocked = true;
				break;
			}
		}
	}

	if (moveBlocked)
	{
		checkPoint = point;
	}

	return checkPoint;
}

void Labyrinth::Draw(IRenderer3D &renderer) const
{
	m_floor.Draw(renderer);	
	
	renderer.SetColormapSlot(2);
	m_roof.Draw(renderer);
	for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
	{
		(*it)->Draw(renderer);
	}

	for (auto it = m_objects.begin(); it != m_objects.end(); it++)
	{
		(*it)->Draw(renderer);
	}
}

Labyrinth::~Labyrinth()
{
}
