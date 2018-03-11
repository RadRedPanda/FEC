#ifndef _H_AGK_TEMPLATE_
#define _H_AGK_TEMPLATE_

// Link to GDK libraries
#include "AGK.h"
#include "map.h"
#include "player.h"

#define DEVICE_WIDTH 1024
#define DEVICE_HEIGHT 768
#define DEVICE_POS_X 32
#define DEVICE_POS_Y 32
#define FULLSCREEN false

// used to make a more unique folder for the write path
#define COMPANY_NAME "My Company"

// Global values for the app
class app
{
public:
	// constructor
	app() { memset ( this, 0, sizeof(app)); }

	// main app functions - mike to experiment with a derived class for this..
	void Begin( void );
	int Loop( void );
	void End( void );
private:
	void moveCursor(int up, int down, int left, int right);
	void spaceBar();
	void updateTerrainSprite(coord c);
	void updateUnitSprite(coord c);
	std::vector<std::vector<UINT>> gridSprites;
	std::vector<int> tileWeight;
	std::vector<std::vector<UINT>> unitSprites;
	std::vector<UINT> canMove;
	std::vector<UINT> canAttack;
	std::vector<UINT> terrainImages;
	std::vector<UINT> unitImages;
	map currentMap;
	coord cursor;
	std::vector<coord> select;
	UINT cursorSprite;
	int inputBuffer;
	bool holdDown;
	bool selected;
};

extern app App;

#endif

// Allow us to use the LoadImage function name
#ifdef LoadImage
 #undef LoadImage
#endif