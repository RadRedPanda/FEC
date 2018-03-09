#include "unit.h"

unit::unit(){

}

unit::unit(int type, int color) {
	team = color;
	turn = false;
	id = type;
	switch (type) {
	case 0: // no unit
		break;
	case 1: // basic unit
		health = 100;
		moveDistance = 5;
		attackAndMove = true;
		attackRange = 1;
		break;
	case 2: // next unit
		break;
	}
}

unit::~unit(){
}

int unit::getId() {
	return id;
}

int unit::getTeam() {
	return team;
}