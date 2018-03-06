#pragma once
class unit
{
public:
	unit();
	unit(int xPos, int yPos);
	~unit();

private:
	int x;
	int y;
	int health;
	bool turn;
	int team;
};

