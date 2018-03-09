#include "map.h"

map::map(){
	sizeX = sizeY = 10;
	terrainMap.resize(sizeX, std::vector<int>(sizeY, 0));
	unitMap.resize(sizeX, std::vector<int>(sizeY, 0));
}

map::~map(){
}

void map::loadMap(std::string fileName) {
	if (!agk::GetFileExists(fileName.c_str()))
		return;
	int file = agk::OpenToRead(fileName.c_str());
	sizeX = agk::ReadInteger(file);
	sizeY = agk::ReadInteger(file);
	terrainMap.resize(sizeX, std::vector<int>(sizeY, 0));
	for (int y = 0; y < sizeY; y+=1)
		for (int x = 0; x < sizeX; x+=1)
			terrainMap[x][y] = agk::ReadInteger(file);
	agk::CloseFile(file);
}

void map::saveMap(std::string fileName) {
	int file = agk::OpenToWrite(fileName.c_str());
	agk::WriteInteger(file, sizeX);
	agk::WriteInteger(file, sizeY);
	for (int y = 0; y < sizeY; y+=1)
		for (int x = 0; x < sizeX; x+=1)
			agk::WriteInteger(file, terrainMap[x][y]);
}