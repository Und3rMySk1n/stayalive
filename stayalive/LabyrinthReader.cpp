#include "stdafx.h"
#include "LabyrinthReader.h"
#include <sstream>

namespace xml = tinyxml2;

CLabyrinthReader::CLabyrinthReader(const char* filename)
{
	m_doc.LoadFile(filename);
}

unsigned CLabyrinthReader::GetWidth()
{
	xml::XMLElement* rootElement = m_doc.RootElement();
	std::string mapWidthAsString(rootElement->Attribute("width"));
	unsigned mapWidth = (unsigned)atoi(mapWidthAsString.c_str());
	return mapWidth;
}

unsigned CLabyrinthReader::GetLength()
{
	xml::XMLElement* rootElement = m_doc.RootElement();
	std::string mapLengthAsString(rootElement->Attribute("height"));
	unsigned mapLength = (unsigned)atoi(mapLengthAsString.c_str());
	return mapLength;
}

std::vector<std::vector<int>> CLabyrinthReader::GetBlocks(int width, int length)
{
	std::vector<std::vector<int>> blocksArray;

	xml::XMLElement* dataElement = m_doc.RootElement()->FirstChildElement("layer")->FirstChildElement("data");
	std::string blocksAsString(dataElement->GetText());
	std::istringstream blockAsStringStream(blocksAsString);
	std::string numAsLine;

	for (int i = 0; i < width; i++)
	{
		std::vector<int> blocksString;
		for (int j = 0; j < length; j++)
		{
			std::getline(blockAsStringStream, numAsLine, ',');
			int x = atoi(numAsLine.c_str());
			blocksString.push_back(x);
		}
		blocksArray.push_back(blocksString);
	}

	return blocksArray;
}

CLabyrinthReader::~CLabyrinthReader()
{
}
