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
	images = {
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
			gridSprites[x][y] = agk::CreateSprite(0);//agk::LoadImage("/sprites/terrain/" + currentMap.getCoord(x, y) + ".png"));
			agk::SetSpriteSize(gridSprites[x][y], TILE_SIZE, TILE_SIZE);
			agk::SetSpritePosition(gridSprites[x][y], x * (TILE_SIZE + 1.0f), y * (TILE_SIZE + 1.0f));
			agk::SetSpriteColor(gridSprites[x][y], 255, 255, 255, 255);
			agk::SetSpriteDepth(gridSprites[x][y], 15);
		}

	unitSprites.resize(currentMap.getSizeX(), std::vector<UINT>(currentMap.getSizeY(), 0));
	for (int x = 0; x < (int)unitSprites.size(); x += 1)
		for (int y = 0; y < (int)unitSprites[0].size(); y += 1) {
			unitSprites[x][y] = agk::CreateSprite(0);//agk::LoadImage("/sprites/units/" + currentMap.getUnitOn(x, y).getId() + ".png"));
			agk::SetSpriteSize(unitSprites[x][y], TILE_SIZE, TILE_SIZE);
			agk::SetSpritePosition(unitSprites[x][y], x * (TILE_SIZE + 1.0f), y * (TILE_SIZE + 1.0f));
			agk::SetSpriteColor(unitSprites[x][y], 255, 0, 255, 255);
			agk::SetSpriteVisible(unitSprites[x][y], 0);
			agk::SetSpriteDepth(unitSprites[x][y], 10);
		}

	// chooses sprite for cursor, it should be in the /Final folder
	cursorX = cursorY = 0;
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
	agk::Print(currentMap.getUnitOn(cursorX, cursorY).getId());
	agk::Print((int)images.size());

	// whenever a number is pressed sets its unit id to that number
	if (agk::GetRawKeyPressed(48) || agk::GetRawKeyPressed(49) || agk::GetRawKeyPressed(50) || agk::GetRawKeyPressed(51) || agk::GetRawKeyPressed(52) || agk::GetRawKeyPressed(53) || agk::GetRawKeyPressed(54) || agk::GetRawKeyPressed(55) || agk::GetRawKeyPressed(56) || agk::GetRawKeyPressed(57)) {
		currentMap.setUnitOn(cursorX, cursorY, agk::GetRawLastKey() - 48);
		updateUnitSprite(cursorX, cursorY);
	}

	// allows the user to toggle selection
	if (agk::GetRawKeyPressed(SPACE_BAR)) {
		if (selected) {
			while (selectX.size() > 0) {
				agk::SetSpriteColorRed(gridSprites[selectX[selectX.size() - 1]][selectY[selectY.size() - 1]], 255);
				selectX.pop_back();
				selectY.pop_back();
			}
			selected = false;
		}
		else {
			if (currentMap.getUnitOn(cursorX, cursorY).getId() > 0) {	// only toggles if the spot contains a unit
				selectX.push_back(cursorX);
				selectY.push_back(cursorY);
				agk::SetSpriteColorRed(gridSprites[selectX[selectX.size() - 1]][selectY[selectY.size() - 1]], 150);
				selected = true;
			}
		}
	}

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
	agk::SetSpritePosition(cursorSprite, agk::GetSpriteX(gridSprites[cursorX][cursorY]), agk::GetSpriteY(gridSprites[cursorX][cursorY]));
	
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
	cursorX += right - left;
	cursorY += down - up;

	// bounds the cursor so it doesn't go off grid
	if (cursorX >= currentMap.getSizeX())
		cursorX = currentMap.getSizeX() - 1;
	if (cursorY >= currentMap.getSizeY())
		cursorY = currentMap.getSizeY() - 1;
	if (cursorX < 0)
		cursorX = 0;
	if (cursorY < 0)
		cursorY = 0;

	// space bar stuff. This does the snake thingy
	if (selected) {
		int cross = -1;
		for (int i = (int)selectX.size() - 2; i >= 0; i-=1) {
			if (cursorX == selectX[i] && cursorY == selectY[i]) {
				cross = i;
				break;
			}
		}
		if (cross >= 0)
			for (int j = (int)selectX.size() - 1; j > cross; j-=1) {
				agk::SetSpriteColorRed(gridSprites[selectX[j]][selectY[j]], 255);
				selectX.pop_back();
				selectY.pop_back();
			}
		else if (cursorX != selectX[selectX.size() - 1] || cursorY != selectY[selectY.size() - 1]) {
			selectX.push_back(cursorX);
			selectY.push_back(cursorY);
			agk::SetSpriteColorRed(gridSprites[cursorX][cursorY], 150);
		}
	}
}

void app::updateUnitSprite(int x, int y) {
	agk::SetSpriteVisible(unitSprites[x][y], currentMap.getUnitOn(x, y).getId() > 0);
	agk::SetSpriteImage(unitSprites[x][y], images[currentMap.getUnitOn(x, y).getId()]);
}