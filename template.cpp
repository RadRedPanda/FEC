// Includes
#include "template.h"
#include "map.h"
#include "unit.h"
#include <iostream>

// Namespace
using namespace AGK;

// game rules
#define CAMERASPEED 8
#define INPUTDELAY 10
#define TILE_SIZE 50
#define CONSOLE false

// C:/Users/Kevin/AppData/Local/AGKApps/My Company/Template64/ write path
// C:/Users/Kevin/Documents/AGKStuff/CPP stuff/apps/FEC/Final/ read path

// renaming key codes
#define KEY_W 87
#define KEY_S 83
#define KEY_A 65
#define KEY_D 68
#define KEY_UP 38
#define KEY_DOWN 40
#define KEY_LEFT 37
#define KEY_RIGHT 39
#define SPACE_BAR 32

app App;

// put stuff here you want to run at the beginning when the game loads up
void app::Begin(void){

	// allows a console to show up, mostly for debugging. Allows use of std::cout
	if (CONSOLE) {
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen("CON", "w", stdout);
	}

	// setup, gotta fix this later
	agk::SetVirtualResolution (1024, 768);
	agk::SetClearColor(0, 0, 0);
	agk::SetSyncRate(30,0);
	agk::SetScissor(0,0,0,0);

	// map setup
	currentMap = map();

	// preload images
	terrainImages = {
		agk::LoadImageResized(("/sprites/terrain/0.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/terrain/1.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/terrain/2.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/terrain/3.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/terrain/4.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/terrain/5.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/terrain/6.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/terrain/7.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/terrain/8.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/terrain/9.png"), TILE_SIZE, TILE_SIZE, 0)
	};
	unitImages = {
		agk::LoadImageResized(("/sprites/units/0.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/units/1.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/units/2.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/units/3.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/units/4.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/units/5.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/units/6.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/units/7.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/units/8.png"), TILE_SIZE, TILE_SIZE, 0),
		agk::LoadImageResized(("/sprites/units/9.png"), TILE_SIZE, TILE_SIZE, 0)
	};

	// sets up the grid
	gridSprites.resize(currentMap.getSizeX(), std::vector<UINT>(currentMap.getSizeY(), 0));
	for (int x = 0; x < (int)gridSprites.size(); x += 1)
		for (int y = 0; y < (int)gridSprites[0].size(); y += 1) {
			coord c;
			c.x = x;
			c.y = y;
			gridSprites[x][y] = agk::CreateSprite(terrainImages[currentMap.getCoord(c)]);
			agk::SetSpriteSize(gridSprites[x][y], TILE_SIZE, TILE_SIZE);
			agk::SetSpritePosition(gridSprites[x][y], x * (TILE_SIZE + 1.0f), y * (TILE_SIZE + 1.0f));
			agk::SetSpriteColor(gridSprites[x][y], 255, 255, 255, 255);
			agk::SetSpriteDepth(gridSprites[x][y], 15);
		}

	unitSprites.resize(currentMap.getSizeX(), std::vector<UINT>(currentMap.getSizeY(), 0));
	for (int x = 0; x < (int)unitSprites.size(); x += 1)
		for (int y = 0; y < (int)unitSprites[0].size(); y += 1) {
			coord c;
			c.x = x;
			c.y = y;
			unitSprites[x][y] = agk::CreateSprite(unitImages[currentMap.getUnitOn(c).getId()]);
			agk::SetSpriteSize(unitSprites[x][y], TILE_SIZE, TILE_SIZE);
			agk::SetSpritePosition(unitSprites[x][y], x * (TILE_SIZE + 1.0f), y * (TILE_SIZE + 1.0f));
			agk::SetSpriteColor(unitSprites[x][y], 255, 255, 255, 255);
			agk::SetSpriteVisible(unitSprites[x][y], 0);
			agk::SetSpriteDepth(unitSprites[x][y], 10);
		}

	// how many moves that tile takes to move onto
	tileWeight = { 1, 1, 2, 2 };

	// chooses sprite for cursor, it should be in the /Final folder
	cursor.x = cursor.y = 0;
	cursorSprite = agk::CreateSprite(agk::LoadImage("/sprites/cursor.png"));
	agk::SetSpritePosition(cursorSprite, 0, 0);

	// sets sprite depth. Most sprites will start at depth 15 I think, lower depths will show up on top
	agk::SetSpriteDepth(cursorSprite, 1);

	// beginning values
	inputBuffer = 0;
	holdDown = false;
	selected = false;

	std::cout << "Write Path: " << agk::GetWritePath() << std::endl << "Read Path: " << agk::GetReadPath() << std::endl;
}

// put stuff here you want to run every frame
int app::Loop (void){
	// will print out the code for the last key pressed in the top left (might have to move the map to see it)
	agk::Print(agk::GetRawLastKey());
	agk::Print(currentMap.getUnitOn(cursor).getId());

	// whenever + or - are pressed it changes the terrain 107 189
	if (agk::GetRawKeyPressed(107) || agk::GetRawKeyPressed(189)) {
		currentMap.setCoord(cursor, currentMap.getCoord(cursor) + agk::GetRawKeyPressed(107) - agk::GetRawKeyPressed(189));
		updateTerrainSprite(cursor);
	}

	// whenever a number is pressed sets its unit id to that number
	if (agk::GetRawKeyPressed(48) || agk::GetRawKeyPressed(49) || agk::GetRawKeyPressed(50) || agk::GetRawKeyPressed(51) || agk::GetRawKeyPressed(52) || agk::GetRawKeyPressed(53) || agk::GetRawKeyPressed(54) || agk::GetRawKeyPressed(55) || agk::GetRawKeyPressed(56) || agk::GetRawKeyPressed(57)) {
		currentMap.setUnitOn(cursor, agk::GetRawLastKey() - 48);
		updateUnitSprite(cursor);
	}

	// allows the user to toggle selection
	if (agk::GetRawKeyPressed(SPACE_BAR))
		spaceBar();

	// moves the camera with WASD
	agk::SetViewOffset(agk::GetViewOffsetX() + CAMERASPEED * (agk::GetRawKeyState(KEY_D) - agk::GetRawKeyState(KEY_A)),	agk::GetViewOffsetY() + CAMERASPEED * (agk::GetRawKeyState(KEY_S) - agk::GetRawKeyState(KEY_W)));
	
	// moves character on press
	moveCursor(agk::GetRawKeyPressed(KEY_UP), agk::GetRawKeyPressed(KEY_DOWN), agk::GetRawKeyPressed(KEY_LEFT), agk::GetRawKeyPressed(KEY_RIGHT));
	
	// starts the buffer
	if (agk::GetRawKeyPressed(KEY_UP) + agk::GetRawKeyPressed(KEY_LEFT) + agk::GetRawKeyPressed(KEY_RIGHT) + agk::GetRawKeyPressed(KEY_DOWN) > 0) {
		holdDown = true;
		inputBuffer = 0;
	}

	// stops it if they let go
	if (agk::GetRawKeyState(KEY_UP) + agk::GetRawKeyState(KEY_LEFT) + agk::GetRawKeyState(KEY_RIGHT) + agk::GetRawKeyState(KEY_DOWN) == 0)
		holdDown = false;

	// does the hold down thingy when you hold down a direction
	if (holdDown) {
		inputBuffer++;
		if (inputBuffer >= INPUTDELAY) 
			moveCursor(agk::GetRawLastKey() == KEY_UP, agk::GetRawLastKey() == KEY_DOWN, agk::GetRawLastKey() == KEY_LEFT, agk::GetRawLastKey() == KEY_RIGHT);
	}
	
	// sets the cursor sprite
	agk::SetSpritePosition(cursorSprite, agk::GetSpriteX(gridSprites[cursor.x][cursor.y]), agk::GetSpriteY(gridSprites[cursor.x][cursor.y]));
	
	// don't touch this
	agk::Sync();

	// press esc to quit
	return agk::GetRawKeyState(27);
}

// put code here that you want to run when the game closes
void app::End (void){

}

// function to read inputs so I don't have to type it out every time
void app::moveCursor(int up, int down, int left, int right) {
	// if not moving just return
	if (!(up || down || left || right))
		return;

	// moves cursor
	cursor.x += right - left;
	cursor.y += down - up;

	// bounds the cursor so it doesn't go off grid
	if (cursor.x >= currentMap.getSizeX())
		cursor.x = currentMap.getSizeX() - 1;
	if (cursor.y >= currentMap.getSizeY())
		cursor.y = currentMap.getSizeY() - 1;
	if (cursor.x < 0)
		cursor.x = 0;
	if (cursor.y < 0)
		cursor.y = 0;

	// space bar stuff. This does the snake thingy
	if (selected) {
		int cross = -1;
		for (int i = (int)select.size() - 2; i >= 0; i-=1) {
			if (cursor.x == select[i].x && cursor.y == select[i].y) {
				cross = i;
				break;
			}
		}
		if (cross >= 0)
			for (int j = (int)select.size() - 1; j > cross; j-=1) {
				agk::SetSpriteColorRed(gridSprites[select[j].x][select[j].y], 255);
				select.pop_back();
			}
		else if (cursor.x != select[select.size() - 1].x || cursor.y != select[select.size() - 1].y) {
			select.push_back(cursor);
			agk::SetSpriteColorRed(gridSprites[cursor.x][cursor.y], 150);
		}
	}
}

// does stuff when space bar is pressed
void app::spaceBar() {
	if (selected) {
		while (select.size() > 0) {
			agk::SetSpriteColorRed(gridSprites[select[select.size() - 1].x][select[select.size() - 1].y], 255);
			select.pop_back();
		}
		for (int x = 0; x < gridSprites.size(); x += 1)
			for (int y = 0; y < gridSprites[0].size(); y += 1)
				agk::SetSpriteColor(gridSprites[x][y], 255, 255, 255, 255);
		selected = false;
	}
	else {
		if (currentMap.getUnitOn(cursor).getId() > 0) {	// only toggles if the spot contains a unit, need to check if correct player later
			int moveDistance = currentMap.getUnitOn(cursor).getMoveDistance();
			std::vector<std::vector<int>> distanceTiles;
			std::vector<coord> queue;
			queue.push_back(cursor);
			distanceTiles.resize(currentMap.getSizeX(), std::vector<int>(currentMap.getSizeY(), -1));
			distanceTiles[cursor.x][cursor.y] = moveDistance;
			for (int i = 0; i < queue.size(); i += 1) {
				if (queue[i].x > 0 && distanceTiles[queue[i].x][queue[i].y] - tileWeight[currentMap.getCoord({ queue[i].x - 1, queue[i].y })] > distanceTiles[queue[i].x - 1][queue[i].y]) {
					queue.push_back({ queue[i].x - 1, queue[i].y });
					distanceTiles[queue[i].x - 1][queue[i].y] = distanceTiles[queue[i].x][queue[i].y] - tileWeight[currentMap.getCoord({ queue[i].x - 1, queue[i].y })];
				}
				if (queue[i].y > 0 && distanceTiles[queue[i].x][queue[i].y] - tileWeight[currentMap.getCoord({ queue[i].x, queue[i].y - 1 })] > distanceTiles[queue[i].x][queue[i].y - 1]) {
					queue.push_back({ queue[i].x, queue[i].y - 1 });
					distanceTiles[queue[i].x][queue[i].y - 1] = distanceTiles[queue[i].x][queue[i].y] - tileWeight[currentMap.getCoord({ queue[i].x, queue[i].y - 1 })];
				}
				if (queue[i].x < currentMap.getSizeX() - 1 && distanceTiles[queue[i].x][queue[i].y] - tileWeight[currentMap.getCoord({ queue[i].x + 1, queue[i].y })] > distanceTiles[queue[i].x + 1][queue[i].y]) {
					queue.push_back({ queue[i].x + 1, queue[i].y });
					distanceTiles[queue[i].x + 1][queue[i].y] = distanceTiles[queue[i].x][queue[i].y] - tileWeight[currentMap.getCoord({ queue[i].x + 1, queue[i].y })];
				}
				if (queue[i].y < currentMap.getSizeY() - 1 && distanceTiles[queue[i].x][queue[i].y] - tileWeight[currentMap.getCoord({ queue[i].x, queue[i].y + 1 })] > distanceTiles[queue[i].x][queue[i].y + 1]) {
					queue.push_back({ queue[i].x, queue[i].y + 1});
					distanceTiles[queue[i].x][queue[i].y + 1] = distanceTiles[queue[i].x][queue[i].y] - tileWeight[currentMap.getCoord({ queue[i].x, queue[i].y + 1 })];
				}
			}
			// queue is all the tiles that are within moving distance
			for (int i = 0; i < queue.size(); i += 1)
				agk::SetSpriteColorGreen(gridSprites[queue[i].x][queue[i].y], 0);

			select.push_back(cursor);
			agk::SetSpriteColorRed(gridSprites[select[select.size() - 1].x][select[select.size() - 1].y], 150);
			selected = true;
		}
	}
	return;
}

void app::updateTerrainSprite(coord c) {
	agk::SetSpriteImage(gridSprites[c.x][c.y], terrainImages[currentMap.getCoord(c)]);
}

void app::updateUnitSprite(coord c) {
	agk::SetSpriteVisible(unitSprites[c.x][c.y], currentMap.getUnitOn(c).getId() > 0);
	agk::SetSpriteImage(unitSprites[c.x][c.y], unitImages[currentMap.getUnitOn(c).getId()]);
}