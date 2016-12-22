#pragma once
#include "FlatSurface.h"
#include "SolidBlock.h"
#include "LabyrinthObject.h"
#include "Key.h"

typedef std::vector<std::vector<int>> BlockArrayType;
typedef std::vector<std::vector<glm::vec2>> WallPointsArrayType;

class Labyrinth
{
public:
	Labyrinth(unsigned slices, unsigned stacks, float height, unsigned tileSize);

	void Draw(IRenderer3D &renderer) const;
	void AddWalls(BlockArrayType wallPointsArray);
	void AddKey(glm::vec2 position);
	glm::vec2 GetObjectPosition(BlockArrayType wallPointsArray, int objectId);
	glm::vec2 CorrectActorMovement(glm::vec2 point, glm::vec2 newPoint, float offset);

	~Labyrinth();
private:
	WallPointsArrayType GetWallsFromBlocksArray(BlockArrayType blocksArray);

	float m_height = 0;
	unsigned m_slices = 0;
	unsigned m_stacks = 0;
	unsigned m_tileSize = 0;

	FlatSurface m_floor;
	FlatSurface m_roof;
	CMeshP3NT2 m_wallsMesh;

	std::vector<std::unique_ptr<SolidBlock>> m_blocks;
	std::vector<std::unique_ptr<CLabyrinthObject>> m_objects;
};

