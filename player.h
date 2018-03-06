#pragma once
class player
{
public:
	player();
	~player();
	bool playerTurn();
	void playerIdle(bool waiting);
private:
	int playerNumber;
	int power;
	int money;
};

