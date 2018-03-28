#pragma once
#include <vector>
#include "unit.h"
#include "agk.h"

struct coord { int x; int y; };

class map
{
public:
	map();
	~map();
	std::vector<std::vector<int>> getTerrain();
	int getSizeX();
	int getSizeY();
	int getCoord(coord c);
	void setCoord(coord c, int id);
	unit getUnitOn(coord c);
	void setUnitOn(coord c, int id);
	void loadMap(std::string fileName);
	void saveMap(std::string fileName);
	int getTurn();
	void moveUnit(coord start, coord end);
private:
	coord size;
	std::vector<std::vector<int>> terrainMap;
	std::vector<std::vector<unit>> unitMap;
	int weather;
	bool FoW;
	int playerTurn;
};
