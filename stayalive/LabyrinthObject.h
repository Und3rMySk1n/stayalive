#pragma once
#include "IRenderer3D.h"

class CLabyrinthObject
{
public:
	CLabyrinthObject();

	virtual void Draw(IRenderer3D &renderer)const = 0;

	~CLabyrinthObject();
};

