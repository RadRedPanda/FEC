#include "map.h"

map::map(){
	size.x = 10;
	size.y = 10;
	terrainMap.resize(size.x, std::vector<int>(size.y, 1));
	unitMap.resize(size.x, std::vector<unit>(size.y, unit()));
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
	return size.x;
}

int map::getSizeY() {
	return size.y;
}

int map::getCoord(coord c) {
	return terrainMap[c.x][c.y];
}

void map::setCoord(coord c, int id) {
	terrainMap[c.x][c.y] = id;
}

unit map::getUnitOn(coord c) {
	return unitMap[c.x][c.y];
}

void map::setUnitOn(coord c, int id) {
	unitMap[c.x][c.y] = unit(id, 1);
}

void map::loadMap(std::string fileName) {
	if (!agk::GetFileExists(fileName.c_str()))
		return;
	int file = agk::OpenToRead(fileName.c_str());
	size.x = agk::ReadInteger(file);
	size.y = agk::ReadInteger(file);
	terrainMap.resize(size.x, std::vector<int>(size.y, 0));
	for (int y = 0; y < size.y; y+=1)
		for (int x = 0; x < size.x; x += 1) {
			terrainMap[x][y] = agk::ReadInteger(file);
			unitMap[x][y] = unit(agk::ReadInteger(file), agk::ReadInteger(file));
		}
	agk::CloseFile(file);
}

void map::saveMap(std::string fileName) {
	int file = agk::OpenToWrite(fileName.c_str());
	agk::WriteInteger(file, size.x);
	agk::WriteInteger(file, size.y);
	for (int y = 0; y < size.y; y+=1)
		for (int x = 0; x < size.x; x += 1) {
			agk::WriteInteger(file, terrainMap[x][y]);
			agk::WriteInteger(file, unitMap[x][y].getId());
			agk::WriteInteger(file, unitMap[x][y].getTeam());
		}
}

int map::getTurn() {
	return playerTurn;
}

void map::moveUnit(coord start, coord end) {
	unitMap[end.x][end.y] = unitMap[start.x][start.y];
	unitMap[start.x][start.y] = unit();
}