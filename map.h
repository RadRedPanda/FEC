#pragma once
#include <vector>
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
	std::vector<std::vector<int>> terrain;
	std::vector<std::vector<int>> units;
	int weather;
	bool FoW;
	int playerTurn;
};

