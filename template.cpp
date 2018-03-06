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
#define GRID_SIZE 10
#define TILE_SIZE 50

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

// function to read inputs so I don't have to type it out every time
void app::moveCursor(int up, int down, int left, int right) {
	// if not moving just return
	if (!(up || down || left || right))
		return;

	// moves cursor
	cursorX += right - left;
	cursorY += down - up;

	// bounds the cursor so it doesn't go off grid
	if (cursorX >= GRID_SIZE)
		cursorX = GRID_SIZE - 1;
	if (cursorY >= GRID_SIZE)
		cursorY = GRID_SIZE - 1;
	if (cursorX < 0)
		cursorX = 0;
	if (cursorY < 0)
		cursorY = 0;

	// space bar stuff. This does the snake thingy
	if (selected) {
		for (int i = (int)selectX.size() - 2; i >= 0; i--) {
			if (cursorX == selectX[i] && cursorY == selectY[i]) {
				for (int j = selectX.size() - 1; j >= i; j--){
					agk::SetSpriteColorRed(grid[selectX[j]][selectY[j]], 255);
					selectX.pop_back();
					selectY.pop_back();
				}
				break;
			}
		}
		if (!(cursorX == selectX.size() - 1 && cursorY == selectY.size() - 1)) {
			selectX.push_back(cursorX);
			selectY.push_back(cursorY);
			agk::SetSpriteColorRed(grid[cursorX][cursorY], 150);
		}
	}
}

// put stuff here you want to run at the beginning when the game loads up
void app::Begin(void){

	// allows a console to show up, mostly for debugging. Allows use of std::cout
	/*
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
	*/

	// setup, gotta fix this later
	agk::SetVirtualResolution (1024, 768);
	agk::SetClearColor(0, 0, 0);
	agk::SetSyncRate(30,0);
	agk::SetScissor(0,0,0,0);

	// sets up the grid
	grid.resize(GRID_SIZE, std::vector<UINT>(GRID_SIZE, 1));
	cursorX = cursorY = 0;
	for (int y = 0; y < (int)grid[0].size(); y++)
		for (int x = 0; x < (int)grid.size(); x++) {
			grid[x][y] = agk::CreateSprite(0);
			agk::SetSpriteSize(grid[x][y], TILE_SIZE, TILE_SIZE);
			agk::SetSpritePosition(grid[x][y], x * (TILE_SIZE + 1.0f), y * (TILE_SIZE + 1.0f));
			agk::SetSpriteColor(grid[x][y], 255, 255, 255, 255);
		}

	// chooses sprite for cursor, it should be in the /Final folder
	cursorSprite = agk::CreateSprite(agk::LoadImage("cursor.png"));
	agk::SetSpritePosition(cursorSprite, 0, 0);

	// sets sprite depth. Most sprites will start at depth 15 I think, lower depths will show up on top
	agk::SetSpriteDepth(cursorSprite, 1);

	// beginning values
	inputBuffer = 0;
	holdDown = false;
	selected = false;
}

// put stuff here you want to run every frame
int app::Loop (void){
	// will print out the code for the last key pressed in the top left (might have to move the map to see it)
	agk::Print(agk::GetRawLastKey());

	// allows the user to toggle selection
	if (agk::GetRawKeyPressed(SPACE_BAR)) {
		selected = !selected;
		if (!selected) {
			std::cout << selectX.size();
			while (selectX.size() > 0) {
				agk::SetSpriteColorRed(grid[selectX[selectX.size() - 1]][selectY[selectY.size() - 1]], 255);
				selectX.pop_back();
				selectY.pop_back();
			}
		}
		else {
			selectX.push_back(cursorX);
			selectY.push_back(cursorY);
			agk::SetSpriteColorRed(grid[selectX[selectX.size() - 1]][selectY[selectY.size() - 1]], 150);
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
	agk::SetSpritePosition(cursorSprite, agk::GetSpriteX(grid[cursorX][cursorY]), agk::GetSpriteY(grid[cursorX][cursorY]));
	
	// don't touch this
	agk::Sync();

	// press esc to quit
	return agk::GetRawKeyState(27);
}

// put code here that you want to run when the game closes
void app::End (void){

}
