#pragma once
#include <vector>
#include "unit.h"
#include "agk.h"

class map
{
public:
	map();
	~map();
	std::vector<std::vector<int>> getTerrain();
	int getSizeX();
	int getSizeY();
	int getCoord(int x, int y);
	unit getUnitOn(int x, int y);
	void setUnitOn(int x, int y, int id);
	void loadMap(std::string fileName);
	void saveMap(std::string fileName);
private:
	int sizeX, sizeY;
	std::vector<std::vector<int>> terrainMap;
	std::vector<std::vector<unit>> unitMap;
	int weather;
	bool FoW;
	int playerTurn;
};
