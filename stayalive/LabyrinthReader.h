#pragma once
#include "lib.h"

class CLabyrinthReader
{
public:
	CLabyrinthReader(const char* filename);

	unsigned GetWidth();
	unsigned GetLength();
	std::vector<std::vector<int>> GetBlocks(int width, int length);

	~CLabyrinthReader();
private:
	tinyxml2::XMLDocument m_doc;
};

