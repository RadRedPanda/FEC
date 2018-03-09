#pragma once
#include <vector>
#include "unit.h"
#include "agk.h"

class map
{
public:
	map();
	~map();
	void loadMap(std::string fileName);
	void saveMap(std::string fileName);
private:
	int sizeX, sizeY;
	std::vector<std::vector<int>> terrainMap;
	std::vector<std::vector<int>> unitMap;
	int weather;
	bool FoW;
	int playerTurn;
};

