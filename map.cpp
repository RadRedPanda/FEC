#include "map.h"

map::map(){
	sizeX = 10;
	sizeY = 10;
	terrainMap.resize(sizeX, std::vector<int>(sizeY, 0));
	unitMap.resize(sizeX, std::vector<unit>(sizeY, unit()));
	weather = 0;
	FoW = false;
	playerTurn = 1;
}

map::~map(){

}

std::vector<std::vector<int>> map::getTerrain() {
	return terrainMap;
}

int map::getSizeX() {
	return sizeX;
}

int map::getSizeY() {
	return sizeY;
}

int map::getCoord(int x, int y) {
	return terrainMap[x][y];
}

unit map::getUnitOn(int x, int y) {
	return unitMap[x][y];
}

void map::setUnitOn(int x, int y, int id) {
	unitMap[x][y] = unit(id, 1);
}

void map::loadMap(std::string fileName) {
	if (!agk::GetFileExists(fileName.c_str()))
		return;
	int file = agk::OpenToRead(fileName.c_str());
	sizeX = agk::ReadInteger(file);
	sizeY = agk::ReadInteger(file);
	terrainMap.resize(sizeX, std::vector<int>(sizeY, 0));
	for (int y = 0; y < sizeY; y+=1)
		for (int x = 0; x < sizeX; x += 1) {
			terrainMap[x][y] = agk::ReadInteger(file);
			unitMap[x][y] = unit(agk::ReadInteger(file), agk::ReadInteger(file));
		}
	agk::CloseFile(file);
}

void map::saveMap(std::string fileName) {
	int file = agk::OpenToWrite(fileName.c_str());
	agk::WriteInteger(file, sizeX);
	agk::WriteInteger(file, sizeY);
	for (int y = 0; y < sizeY; y+=1)
		for (int x = 0; x < sizeX; x += 1) {
			agk::WriteInteger(file, terrainMap[x][y]);
			agk::WriteInteger(file, unitMap[x][y].getId());
			agk::WriteInteger(file, unitMap[x][y].getTeam());
		}
}