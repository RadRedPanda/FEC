#pragma once
#include <vector>

class map
{
public:
	map();
	~map();
	void loadMap();
private:
	int sizeX, sizeY;
	int weather;
	bool FoW;
	int playerTurn;
};

