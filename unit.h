#pragma once

class unit
{
public:
	unit();
	unit(int type, int team);
	~unit();
	int getId();
	int getTeam();
private:
	int id;
	int moveDistance;
	int attackRange;
	int sightRange;
	bool attackAndMove;
	int health;
	bool turn;
	int team;
};

