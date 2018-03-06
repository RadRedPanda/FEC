// Includes
#include "template.h"
#include "map.h"
#include "unit.h"
#include <iostream>

// Namespace
using namespace AGK;
#define CAMERASPEED 8
#define INPUTDELAY 10
#define GRID_SIZE 10
#define TILE_SIZE 50
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

void app::moveCursor(int up, int down, int left, int right) {
	if (!(up || down || left || right))
		return;
	cursorX += right - left;
	cursorY += down - up;
	if (cursorX >= GRID_SIZE)
		cursorX = GRID_SIZE - 1;
	if (cursorY >= GRID_SIZE)
		cursorY = GRID_SIZE - 1;
	if (cursorX < 0)
		cursorX = 0;
	if (cursorY < 0)
		cursorY = 0;
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
			agk::SetSpriteColorRed(grid[cursorX][cursorY], 200);
		}
	}
}

void app::Begin(void){
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);

	agk::SetVirtualResolution (1024, 768);
	agk::SetClearColor(0, 0, 0);
	agk::SetSyncRate(30,0);
	agk::SetScissor(0,0,0,0);
	grid.resize(GRID_SIZE, std::vector<UINT>(GRID_SIZE, 1));
	cursorX = cursorY = 0;
	for (int y = 0; y < (int)grid[0].size(); y++)
		for (int x = 0; x < (int)grid.size(); x++) {
			grid[x][y] = agk::CreateSprite(0);
			agk::SetSpriteSize(grid[x][y], TILE_SIZE, TILE_SIZE);
			agk::SetSpritePosition(grid[x][y], x * (TILE_SIZE + 1.0f), y * (TILE_SIZE + 1.0f));
			agk::SetSpriteColor(grid[x][y], 255, 255, 255, 255);
		}
	cursorSprite = agk::CreateSprite(agk::LoadImage("cursor.png"));
	agk::SetSpritePosition(cursorSprite, 0, 0);
	agk::SetSpriteDepth(cursorSprite, 1);
	inputBuffer = 0;
	holdDown = false;
	selected = false;
}

int app::Loop (void){
	agk::Print(agk::GetRawLastKey());
	agk::Print(cursorX);
	agk::Print(cursorY);
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
			agk::SetSpriteColorRed(grid[selectX[selectX.size() - 1]][selectY[selectY.size() - 1]], 200);
		}
	}
	agk::SetViewOffset(agk::GetViewOffsetX() + CAMERASPEED * (agk::GetRawKeyState(KEY_D) - agk::GetRawKeyState(KEY_A)),	agk::GetViewOffsetY() + CAMERASPEED * (agk::GetRawKeyState(KEY_S) - agk::GetRawKeyState(KEY_W)));
	moveCursor(agk::GetRawKeyPressed(KEY_UP), agk::GetRawKeyPressed(KEY_DOWN), agk::GetRawKeyPressed(KEY_LEFT), agk::GetRawKeyPressed(KEY_RIGHT));
	if (agk::GetRawKeyPressed(KEY_UP) + agk::GetRawKeyPressed(KEY_LEFT) + agk::GetRawKeyPressed(KEY_RIGHT) + agk::GetRawKeyPressed(KEY_DOWN) > 0) {
		holdDown = true;
		inputBuffer = 0;
	}
	if (agk::GetRawKeyState(KEY_UP) + agk::GetRawKeyState(KEY_LEFT) + agk::GetRawKeyState(KEY_RIGHT) + agk::GetRawKeyState(KEY_DOWN) == 0)
		holdDown = false;
	if (holdDown) {
		inputBuffer++;
		if (inputBuffer >= INPUTDELAY) 
			moveCursor(agk::GetRawLastKey() == KEY_UP, agk::GetRawLastKey() == KEY_DOWN, agk::GetRawLastKey() == KEY_LEFT, agk::GetRawLastKey() == KEY_RIGHT);
	}
	agk::SetSpritePosition(cursorSprite, agk::GetSpriteX(grid[cursorX][cursorY]), agk::GetSpriteY(grid[cursorX][cursorY]));
	agk::Sync();

	return agk::GetRawKeyState(27);
}


void app::End (void)
{

}
