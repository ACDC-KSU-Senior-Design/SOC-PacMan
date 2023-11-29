#include "PacMan.h"
#include "stdlib.h"
#include <stdio.h>
#include <unistd.h>

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a > b ? b : a)

#define PacMan_Color 0xFD

typedef struct{
    uint16_t x;
    uint16_t y;
}Coordinates;

// https://www.pixilart.com/draw/mine-is-the-coordinates-4e1b3d00ba4c277

static uint16_t **GameLines;
static uint16_t **GameBoard;
static uint16_t **PacMan;
static uint16_t **Combined;
static uint16_t **Dots;

static Coordinates PacMan_Old = {320,355};
static Direction PacMan_OldDir;

// Pacman Variables
static Direction PrevDirection;
static int AnimationPhase = 0;

static int Score = 0;

#pragma region Static Function Declarations
static uint16_t** CreateArray(int xSize, int ySize);
static void PlaceBox(uint16_t ***array, int x1, int y1, int x2, int y2, short color);
static void PlaceLine(uint16_t ***array, int x1, int y1, int x2, int y2, short color);
static void PlacePixel(uint16_t ***array, int x, int y, short color);
static void PlacePacMan(uint16_t ***array, uint16_t **gameboard, uint16_t ** pacman, Coordinates old, Coordinates new);
static void PlaceSmallDot(uint16_t ***combined, uint16_t ***dots, uint16_t x, uint16_t y);
static void PlaceBigDot(uint16_t ***combined, uint16_t ***dots, uint16_t x, uint16_t y);
static void PlaceSpriteWithTransparency(uint16_t **Combined, uint16_t **GameBoard, uint16_t ***Sprite, Coordinates spriteCoord);

static void InitializePacMan(uint16_t ***pacMan);
static void Move_Pacman(uint16_t ***pacMan, int xCenter, int yCenter, Direction NewDirection, Direction PrevDirection, void *virtual_base);
static void PacManBufferTooUp(uint16_t ***pacMan, int CurrPhase);
static void PacManBufferTooLeft(uint16_t ***pacMan, int CurrPhase);
static void PacManBufferTooRight(uint16_t ***pacMan, int CurrPhase);
static void PacManBufferTooDown(uint16_t ***pacMan, int CurrPhase);
static void ResetPacmanBuffer(uint16_t ***pacMan);
static bool CheckForLine(uint16_t **GameLinesArr, Coordinates PmCoordinates, Direction NewDirection);
static void IncrementScore();

#pragma endregion

void InitGameBoard(void *virtual_base){

	uint16_t pixel_color = LIGHT_BLUE;

	PacMan = CreateArray(19, 19);
    GameBoard = CreateArray(SCREEN_WIDTH, SCREEN_HEIGHT);
	Combined = CreateArray(SCREEN_WIDTH, SCREEN_HEIGHT);
	GameLines = CreateArray(SCREEN_WIDTH, SCREEN_HEIGHT);
	Dots = CreateArray(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	#pragma region InitGameboard

	PlaceBox(&GameBoard, 105,   3, 535,  24, pixel_color);
	PlaceBox(&GameBoard, 105,  25, 126, 161, pixel_color);
	PlaceBox(&GameBoard, 127, 140, 194, 161, pixel_color);
	PlaceBox(&GameBoard, 173, 162, 194, 206, pixel_color);
	PlaceBox(&GameBoard,  82, 185, 172, 206, pixel_color);	//Top Outside Boundry
	PlaceBox(&GameBoard, 310,  25, 330,  70, pixel_color);
	PlaceBox(&GameBoard, 514,  25, 535, 161, pixel_color);
	PlaceBox(&GameBoard, 446, 140, 513, 161, pixel_color);
	PlaceBox(&GameBoard, 446, 162, 467, 206, pixel_color);
	PlaceBox(&GameBoard, 468, 185, 558, 206, pixel_color);
	
	PlaceBox(&GameBoard, 151,  49, 194,  70, pixel_color);
	PlaceBox(&GameBoard, 151,  94, 194, 115, pixel_color);	//TL Boxes
	PlaceBox(&GameBoard, 219,  49, 285,  70, pixel_color);
	
	PlaceBox(&GameBoard, 355,  49, 421,  70, pixel_color);
	PlaceBox(&GameBoard, 446,  49, 489,  70, pixel_color);	//TR Boxes
	PlaceBox(&GameBoard, 446,  94, 489, 115, pixel_color);
	
	PlaceBox(&GameBoard, 219,  94, 240, 206, pixel_color);
	PlaceBox(&GameBoard, 241, 140, 285, 161, pixel_color);	
	PlaceBox(&GameBoard, 264,  94, 376, 115, pixel_color);	//TM Boxes
	PlaceBox(&GameBoard, 310, 116, 330, 161, pixel_color);
	PlaceBox(&GameBoard, 400,  94, 421, 206, pixel_color);
	PlaceBox(&GameBoard, 355, 140, 399, 161, pixel_color);
	
	
	PlaceBox(&GameBoard, 264, 185, 376, 206, pixel_color);
	PlaceBox(&GameBoard, 264, 230, 376, 251, pixel_color);
	PlaceBox(&GameBoard, 264, 207, 285, 229, pixel_color);	//Middle
	PlaceBox(&GameBoard, 355, 207, 376, 230, pixel_color);
	PlaceBox(&GameBoard, 310, 185, 330, 206, LIGHT_GREEN);	


	PlaceBox(&GameBoard, 82 , 230, 194, 252, pixel_color);
	PlaceBox(&GameBoard, 173, 253, 194, 297, pixel_color);
	PlaceBox(&GameBoard, 105, 298, 126, 479, pixel_color);
	PlaceBox(&GameBoard, 127, 367, 149, 388, pixel_color);

	PlaceBox(&GameBoard, 127, 458, 535, 479, pixel_color);
	PlaceBox(&GameBoard, 105,276,172,297, pixel_color);

	PlaceBox(&GameBoard, 150,321,194,342, pixel_color);
	PlaceBox(&GameBoard, 173,343,194,388, pixel_color);
	PlaceBox(&GameBoard, 151,412,285,433, pixel_color);
	PlaceBox(&GameBoard, 219,367,240,411, pixel_color);
	PlaceBox(&GameBoard, 264,367,376,388, pixel_color);
	PlaceBox(&GameBoard, 309,389,331,433, pixel_color);
	PlaceBox(&GameBoard, 355,412,490,433, pixel_color);
	PlaceBox(&GameBoard, 400,367,421,411, pixel_color);
	PlaceBox(&GameBoard, 446,321,489,342, pixel_color);
	PlaceBox(&GameBoard, 446,343,467,388, pixel_color);
	PlaceBox(&GameBoard, 355,321,421,342, pixel_color);
	PlaceBox(&GameBoard, 446,231,558,251, pixel_color);
	PlaceBox(&GameBoard, 446,252,467,297, pixel_color);
	PlaceBox(&GameBoard, 468,276,535,297, pixel_color);
	PlaceBox(&GameBoard, 514,298,535,457, pixel_color);
	PlaceBox(&GameBoard, 491,367,514,388, pixel_color);
	PlaceBox(&GameBoard, 264,276,376,297, pixel_color);
	PlaceBox(&GameBoard, 309,298,331,342, pixel_color);
	PlaceBox(&GameBoard, 219,321,285,342, pixel_color);
	PlaceBox(&GameBoard, 219,231,240,297, pixel_color);
	PlaceBox(&GameBoard, 400,230,421,297, pixel_color);

	int x, y;
	for(x = 0; x < SCREEN_WIDTH; x++)
		for(y = 0; y < SCREEN_HEIGHT; y++)
			PlacePixel(&Combined, x,y,GameBoard[x][y]);	//Copies the Gameboard into Combined
	#pragma endregion

	#pragma region InitPacMan
		InitializePacMan(&PacMan);
	#pragma endregion

	#pragma region InitDots
	PlaceSmallDot(&Combined, &Dots, 139, 445);
	PlaceSmallDot(&Combined, &Dots, 161, 445);
	PlaceSmallDot(&Combined, &Dots, 184, 445);
	PlaceSmallDot(&Combined, &Dots, 207, 445);
	PlaceSmallDot(&Combined, &Dots, 230, 445);
	PlaceSmallDot(&Combined, &Dots, 252, 445);
	PlaceSmallDot(&Combined, &Dots, 275, 445);
	PlaceSmallDot(&Combined, &Dots, 298, 445);
	PlaceSmallDot(&Combined, &Dots, 320, 445);
	PlaceSmallDot(&Combined, &Dots, 343, 445);
	PlaceSmallDot(&Combined, &Dots, 366, 445);
	PlaceSmallDot(&Combined, &Dots, 389, 445);
	PlaceSmallDot(&Combined, &Dots, 411, 445);
	PlaceSmallDot(&Combined, &Dots, 434, 445);
	PlaceSmallDot(&Combined, &Dots, 457, 445);
	PlaceSmallDot(&Combined, &Dots, 479, 445);
	PlaceSmallDot(&Combined, &Dots, 502, 445); //Bottom Row

	PlaceSmallDot(&Combined, &Dots, 139, 36);
	PlaceSmallDot(&Combined, &Dots, 161, 36);
	PlaceSmallDot(&Combined, &Dots, 184, 36);
	PlaceSmallDot(&Combined, &Dots, 207, 36);
	PlaceSmallDot(&Combined, &Dots, 230, 36);
	PlaceSmallDot(&Combined, &Dots, 252, 36);
	PlaceSmallDot(&Combined, &Dots, 275, 36);
	PlaceSmallDot(&Combined, &Dots, 298, 36);
	PlaceSmallDot(&Combined, &Dots, 343, 36);
	PlaceSmallDot(&Combined, &Dots, 366, 36);
	PlaceSmallDot(&Combined, &Dots, 389, 36);
	PlaceSmallDot(&Combined, &Dots, 411, 36);
	PlaceSmallDot(&Combined, &Dots, 434, 36);
	PlaceSmallDot(&Combined, &Dots, 457, 36);
	PlaceSmallDot(&Combined, &Dots, 479, 36);
	PlaceSmallDot(&Combined, &Dots, 502, 36); //Top Row

	PlaceSmallDot(&Combined, &Dots, 139, 82);
	PlaceSmallDot(&Combined, &Dots, 161, 82);
	PlaceSmallDot(&Combined, &Dots, 184, 82);
	PlaceSmallDot(&Combined, &Dots, 207, 82);
	PlaceSmallDot(&Combined, &Dots, 230, 82);
	PlaceSmallDot(&Combined, &Dots, 252, 82);
	PlaceSmallDot(&Combined, &Dots, 275, 82);
	PlaceSmallDot(&Combined, &Dots, 298, 82);
	PlaceSmallDot(&Combined, &Dots, 320, 82);
	PlaceSmallDot(&Combined, &Dots, 343, 82);
	PlaceSmallDot(&Combined, &Dots, 366, 82);
	PlaceSmallDot(&Combined, &Dots, 389, 82);
	PlaceSmallDot(&Combined, &Dots, 411, 82);
	PlaceSmallDot(&Combined, &Dots, 434, 82);
	PlaceSmallDot(&Combined, &Dots, 457, 82);
	PlaceSmallDot(&Combined, &Dots, 479, 82);
	PlaceSmallDot(&Combined, &Dots, 502, 82); //Second Top Row

	PlaceSmallDot(&Combined, &Dots, 139, 309);
	PlaceSmallDot(&Combined, &Dots, 161, 309);
	PlaceSmallDot(&Combined, &Dots, 184, 309);
	PlaceSmallDot(&Combined, &Dots, 207, 309);
	PlaceSmallDot(&Combined, &Dots, 230, 309);
	PlaceSmallDot(&Combined, &Dots, 252, 309);
	PlaceSmallDot(&Combined, &Dots, 275, 309);
	PlaceSmallDot(&Combined, &Dots, 298, 309);	//Causing Small Box Issue...??
	PlaceSmallDot(&Combined, &Dots, 298, 309);	//Causing Small Box Issue...??
	PlaceSmallDot(&Combined, &Dots, 343, 309);
	PlaceSmallDot(&Combined, &Dots, 366, 309);
	PlaceSmallDot(&Combined, &Dots, 389, 309);
	PlaceSmallDot(&Combined, &Dots, 411, 309);
	PlaceSmallDot(&Combined, &Dots, 434, 309);
	PlaceSmallDot(&Combined, &Dots, 457, 309);
	PlaceSmallDot(&Combined, &Dots, 479, 309);
	PlaceSmallDot(&Combined, &Dots, 502, 309); //4th Row from bottom

	PlaceBigDot(  &Combined, &Dots, 139, 354);	//BIG DOT
	PlaceSmallDot(&Combined, &Dots, 161, 354);
	PlaceSmallDot(&Combined, &Dots, 207, 354);
	PlaceSmallDot(&Combined, &Dots, 230, 354);
	PlaceSmallDot(&Combined, &Dots, 252, 354);
	PlaceSmallDot(&Combined, &Dots, 275, 354);
	PlaceSmallDot(&Combined, &Dots, 298, 354);
	PlaceSmallDot(&Combined, &Dots, 320, 354);
	PlaceSmallDot(&Combined, &Dots, 343, 354);
	PlaceSmallDot(&Combined, &Dots, 366, 354);
	PlaceSmallDot(&Combined, &Dots, 389, 354);
	PlaceSmallDot(&Combined, &Dots, 411, 354);
	PlaceSmallDot(&Combined, &Dots, 434, 354);
	PlaceSmallDot(&Combined, &Dots, 479, 354);	//3rd row from bottom
	PlaceBigDot(  &Combined, &Dots, 502, 354); //BIG DOT

	PlaceSmallDot(&Combined, &Dots, 139, 400);
	PlaceSmallDot(&Combined, &Dots, 161, 400);
	PlaceSmallDot(&Combined, &Dots, 184, 400);
	PlaceSmallDot(&Combined, &Dots, 207, 400);
	PlaceSmallDot(&Combined, &Dots, 252, 400);
	PlaceSmallDot(&Combined, &Dots, 275, 400);
	PlaceSmallDot(&Combined, &Dots, 298, 400);
	PlaceSmallDot(&Combined, &Dots, 343, 400);
	PlaceSmallDot(&Combined, &Dots, 366, 400);
	PlaceSmallDot(&Combined, &Dots, 389, 400);
	PlaceSmallDot(&Combined, &Dots, 434, 400);
	PlaceSmallDot(&Combined, &Dots, 457, 400);
	PlaceSmallDot(&Combined, &Dots, 479, 400);
	PlaceSmallDot(&Combined, &Dots, 502, 400); //2nd Row from bottom

	PlaceSmallDot(&Combined, &Dots, 139, 331);
	PlaceSmallDot(&Combined, &Dots, 298, 331);
	PlaceSmallDot(&Combined, &Dots, 343, 331);
	PlaceSmallDot(&Combined, &Dots, 502, 331); //misc dots

	PlaceSmallDot(&Combined, &Dots, 161, 377);
	PlaceSmallDot(&Combined, &Dots, 207, 377);
	PlaceSmallDot(&Combined, &Dots, 252, 377);
	PlaceSmallDot(&Combined, &Dots, 389, 377);
	PlaceSmallDot(&Combined, &Dots, 434, 377);
	PlaceSmallDot(&Combined, &Dots, 479, 377);	//misc dots

	PlaceSmallDot(&Combined, &Dots, 139, 422);
	PlaceSmallDot(&Combined, &Dots, 298, 422);
	PlaceSmallDot(&Combined, &Dots, 343, 422);
	PlaceSmallDot(&Combined, &Dots, 502, 422);

	PlaceBigDot(  &Combined, &Dots, 139, 58);
	PlaceSmallDot(&Combined, &Dots, 298, 58);
	PlaceSmallDot(&Combined, &Dots, 343, 58);
	PlaceBigDot(  &Combined, &Dots, 502, 58);

	PlaceSmallDot(&Combined, &Dots, 252, 104);
	PlaceSmallDot(&Combined, &Dots, 252, 127);
	PlaceSmallDot(&Combined, &Dots, 275, 127);
	PlaceSmallDot(&Combined, &Dots, 298, 127);

	PlaceSmallDot(&Combined, &Dots, 389, 104);
	PlaceSmallDot(&Combined, &Dots, 389, 127);
	PlaceSmallDot(&Combined, &Dots, 366, 127);
	PlaceSmallDot(&Combined, &Dots, 343, 127); 

	PlaceSmallDot(&Combined, &Dots, 139, 104);
	PlaceSmallDot(&Combined, &Dots, 139, 127);
	PlaceSmallDot(&Combined, &Dots, 161, 127);
	PlaceSmallDot(&Combined, &Dots, 184, 127);
	PlaceSmallDot(&Combined, &Dots, 457, 127);
	PlaceSmallDot(&Combined, &Dots, 479, 127);
	PlaceSmallDot(&Combined, &Dots, 502, 127);
	PlaceSmallDot(&Combined, &Dots, 502, 104);


	PlaceSmallDot(&Combined, &Dots, 434, 59 ); 
	PlaceSmallDot(&Combined, &Dots, 434, 104);
	PlaceSmallDot(&Combined, &Dots, 434, 127);
	PlaceSmallDot(&Combined, &Dots, 434, 150);
	PlaceSmallDot(&Combined, &Dots, 434, 173);
	PlaceSmallDot(&Combined, &Dots, 434, 195);
	PlaceSmallDot(&Combined, &Dots, 434, 218);
	PlaceSmallDot(&Combined, &Dots, 434, 241);
	PlaceSmallDot(&Combined, &Dots, 434, 263);
	PlaceSmallDot(&Combined, &Dots, 434, 286);
	PlaceSmallDot(&Combined, &Dots, 434, 331);
	PlaceSmallDot(&Combined, &Dots, 434, 377); //Left Vertical Column

	PlaceSmallDot(&Combined, &Dots, 207, 59 ); 
	PlaceSmallDot(&Combined, &Dots, 207, 104);
	PlaceSmallDot(&Combined, &Dots, 207, 127);
	PlaceSmallDot(&Combined, &Dots, 207, 150);
	PlaceSmallDot(&Combined, &Dots, 207, 173);
	PlaceSmallDot(&Combined, &Dots, 207, 195);
	PlaceSmallDot(&Combined, &Dots, 207, 218);
	PlaceSmallDot(&Combined, &Dots, 207, 241);
	PlaceSmallDot(&Combined, &Dots, 207, 263);
	PlaceSmallDot(&Combined, &Dots, 207, 286);
	PlaceSmallDot(&Combined, &Dots, 207, 331);
	PlaceSmallDot(&Combined, &Dots, 207, 377); //Right Vertical Column

	#pragma endregion

	#pragma region GameLines
	PlaceBox(&GameLines, 138, 36 , 297, 36 , WHITE);
	PlaceBox(&GameLines, 138, 36 , 138, 128, WHITE);
	PlaceBox(&GameLines, 138, 82 , 206, 82 , WHITE);
	PlaceBox(&GameLines, 138, 128, 205, 128, WHITE);
	PlaceBox(&GameLines, 297, 36 , 297, 82 , WHITE);
	PlaceBox(&GameLines, 206, 82 , 297, 82 , WHITE);
	PlaceBox(&GameLines, 252, 82 , 252, 127, WHITE);
	PlaceBox(&GameLines, 206, 36 , 206, 400, WHITE);
	PlaceBox(&GameLines, 206, 400, 138, 400, WHITE);
	PlaceBox(&GameLines, 138, 400, 138, 445, WHITE);
	PlaceBox(&GameLines, 138, 445, 502, 445, WHITE);
	PlaceBox(&GameLines, 502, 445, 502, 400, WHITE);
	PlaceBox(&GameLines, 343, 36 , 502, 36 , WHITE);
	PlaceBox(&GameLines, 433, 400, 433, 36 , WHITE);
	PlaceBox(&GameLines, 502, 36 , 502, 128, WHITE);
	PlaceBox(&GameLines, 502, 128, 433, 128, WHITE);
	PlaceBox(&GameLines, 501, 82 , 433, 82 , WHITE);
	PlaceBox(&GameLines, 502, 82 , 138, 82 , WHITE);
	PlaceBox(&GameLines, 297, 445, 297, 400, WHITE);
	PlaceBox(&GameLines, 297, 400, 252, 400, WHITE);
	PlaceBox(&GameLines, 252, 400, 252, 354, WHITE);
	PlaceBox(&GameLines, 206, 355, 433, 355, WHITE);	// Start Position is in middle
	PlaceBox(&GameLines, 343, 445, 343, 400, WHITE);
	PlaceBox(&GameLines, 343, 400, 388, 400, WHITE);
	PlaceBox(&GameLines, 388, 400, 388, 355, WHITE);
	PlaceBox(&GameLines, 479, 400, 479, 355, WHITE);
	PlaceBox(&GameLines, 479, 355, 502, 355, WHITE);
	PlaceBox(&GameLines, 502, 355, 502, 309, WHITE);
	PlaceBox(&GameLines, 502, 309, 343, 309, WHITE);
	PlaceBox(&GameLines, 343, 309, 343, 355, WHITE);
	PlaceBox(&GameLines, 433, 400, 502, 400, WHITE);
	PlaceBox(&GameLines, 161, 400, 161, 355, WHITE);
	PlaceBox(&GameLines, 161, 355, 138, 355, WHITE);
	PlaceBox(&GameLines, 138, 355, 138, 309, WHITE);
	PlaceBox(&GameLines, 138, 309, 297, 309, WHITE);
	PlaceBox(&GameLines, 297, 309, 297, 355, WHITE);
	PlaceBox(&GameLines, 343, 36 , 343, 82 , WHITE);
	PlaceBox(&GameLines, 388, 82 , 388, 127, WHITE);
	PlaceBox(&GameLines, 388, 127, 342, 127, WHITE);
	PlaceBox(&GameLines, 342, 127, 342, 175, WHITE);
	PlaceBox(&GameLines, 252, 127, 297, 127, WHITE);
	PlaceBox(&GameLines, 297, 127, 297, 175, WHITE);
	PlaceBox(&GameLines, 252, 175, 387, 175, WHITE);
	PlaceBox(&GameLines, 387, 175, 387, 309, WHITE);
	PlaceBox(&GameLines, 252, 175, 252, 309, WHITE);
	PlaceBox(&GameLines, 252, 264, 387, 264, WHITE);
	PlaceBox(&GameLines, 387, 219, 557, 219, WHITE);
	PlaceBox(&GameLines, 252, 219, 82 , 219, WHITE);
	PlaceBox(&GameLines, 320, 176, 320, 219, DARK_RED);
	PlaceBox(&GameLines, 342, 219, 297, 219, DARK_RED);
	#pragma endregion

	#pragma region InitLogo

	#pragma endregion

	//Dark Blue 0xce43
	ResetPacmanBuffer(&PacMan);
	PlacePacMan(&Combined, GameBoard, PacMan, PacMan_Old, PacMan_Old);
	VGA_draw_buffer(Combined, virtual_base);
	//VGA_draw_buffer_NoColor(GameLines, BLACK, virtual_base);
}

bool MovePacman(Direction NewDirection, void *virtual_base){	//Return bool could be to tell if pacman is moving or stuck on a wall
	//printf("X: %d, Y: %d\n", PacMan_Old.x, PacMan_Old.y);	
	ResetPacmanBuffer(&PacMan);
	Coordinates PacMan_New = PacMan_Old;
	if (Dots[PacMan_New.x][PacMan_New.y] == WHITE)
	{
		printf("X: %d, Y: %d\n", PacMan_Old.x, PacMan_Old.y);
		Dots[PacMan_New.x][PacMan_New.y] = BLACK;
		IncrementScore();
	}


	//printf("X: %d, Y: %d\n", PacMan_Old.x, PacMan_Old.y);
	
	int someConstValue = 1;

	if(NewDirection != PacMan_OldDir && !CheckForLine(GameLines, PacMan_Old, NewDirection)) // if pacman changed directions and there is not a intersection
		NewDirection = PacMan_OldDir;

		//printf("X: %d, Y: %d, Line: %d\n",PacMan_New.x, PacMan_New.y, CheckForLine(GameLines, PacMan_Old, NewDirection));
		int x = PacMan_Old.x;
		int y = PacMan_Old.y;
		//printf("\n%d %d %d\n%d %d %d\t%s\n%d %d %d\n", GameLines[x-1][y-1] > 1 ? 1 : 0, GameLines[x+0][y-1] > 1 ? 1 : 0, GameLines[x+1][y-1] > 1 ? 1 : 0,
												   //GameLines[x-1][y+0] > 1 ? 1 : 0, GameLines[x+0][y+0] > 1 ? 1 : 0, GameLines[x+1][y+0] > 1 ? 1 : 0, NewDirection == RIGHT ? "Right" : NewDirection == LEFT ? "Left"  : NewDirection == DOWN  ? "DOWN" : "UP", 
												   //GameLines[x-1][y+1] > 1 ? 1 : 0, GameLines[x+0][y+1] > 1 ? 1 : 0, GameLines[x+1][y+1] > 1 ? 1 : 0);

	switch(NewDirection){
		case UP:
			if(CheckForLine(GameLines, PacMan_Old, UP)){ // if there is a line in that direction
				PacManBufferTooUp(&PacMan, AnimationPhase);
				PacMan_New.y -= someConstValue;								//move it in that direction
			}
			else PacManBufferTooUp(&PacMan, AnimationPhase);
				//Would need a else if our constMoveValue is not 1 since it wouldnt let it hit a corner 
		break;

		case DOWN:
			if(CheckForLine(GameLines, PacMan_Old, DOWN)){
				PacManBufferTooDown(&PacMan, AnimationPhase);
				PacMan_New.y += someConstValue;
			}else PacManBufferTooDown(&PacMan, AnimationPhase);
		break;

		case LEFT:
			if(CheckForLine(GameLines, PacMan_Old, LEFT)){
				PacManBufferTooLeft(&PacMan, AnimationPhase);
				PacMan_New.x -= someConstValue;
			}else PacManBufferTooLeft(&PacMan, AnimationPhase);
		break;

		case RIGHT:
			if(CheckForLine(GameLines, PacMan_Old, RIGHT)){
				PacManBufferTooRight(&PacMan, AnimationPhase);
				PacMan_New.x += someConstValue;
			}else PacManBufferTooRight(&PacMan, AnimationPhase);
		break;
	}
	
	AnimationPhase++;
	PlaceSpriteWithTransparency(Combined, GameBoard, &PacMan, PacMan_New);
	PlacePacMan(&Combined, GameBoard, PacMan, PacMan_Old, PacMan_New);
	VGA_draw_buffer_Section(Combined, PacMan_Old.x - 10, PacMan_Old.y - 10, PacMan_New.x + 10, PacMan_New.y + 11, virtual_base);
	PacMan_Old = PacMan_New;
	PacMan_OldDir = NewDirection;
	return true;
}

void FreeGameBoard(){
	free(GameBoard);
	free(PacMan);
	free(Combined);
}

#pragma region Static Functions
static void InitializePacMan(uint16_t ***pacMan)
{
	PlaceBox(pacMan, 0, 0, 18, 18, PacMan_Color);
	// Top Left Corner too black
	PlaceBox(pacMan, 0, 0, 6, 0, TRANSPARENT);
	PlaceBox(pacMan, 0, 1, 4, 1, TRANSPARENT);
	PlaceBox(pacMan, 0, 2, 2, 2, TRANSPARENT);
	PlaceBox(pacMan, 0, 3, 1, 3, TRANSPARENT);
	PlaceBox(pacMan, 0, 4, 1, 4, TRANSPARENT);
	PlaceBox(pacMan, 0, 5, 0, 5, TRANSPARENT);
	PlaceBox(pacMan, 0, 6, 0, 6, TRANSPARENT);

	// Bottom Left Corner too TRANSPARENT
	PlaceBox(pacMan, 0, 12, 0, 12, TRANSPARENT);
	PlaceBox(pacMan, 0, 13, 0, 13, TRANSPARENT);
	PlaceBox(pacMan, 0, 14, 1, 14, TRANSPARENT);
	PlaceBox(pacMan, 0, 15, 1, 15, TRANSPARENT);
	PlaceBox(pacMan, 0, 16, 2, 16, TRANSPARENT);
	PlaceBox(pacMan, 0, 17, 5, 17, TRANSPARENT);
	PlaceBox(pacMan, 0, 18, 6, 18, TRANSPARENT);

	// Top Right Corner too TRANSPARENT
	PlaceBox(pacMan, 12, 0, 18, 0, TRANSPARENT);
	PlaceBox(pacMan, 14, 0, 18, 1, TRANSPARENT);
	PlaceBox(pacMan, 16, 1, 18, 2, TRANSPARENT);
	PlaceBox(pacMan, 17, 1, 18, 3, TRANSPARENT);
	PlaceBox(pacMan, 17, 2, 18, 4, TRANSPARENT);
	PlaceBox(pacMan, 18, 4, 18, 5, TRANSPARENT);
	PlaceBox(pacMan, 18, 6, 18, 6, TRANSPARENT);

	// Bottom Right Corner too TRANSPARENT
	PlaceBox(pacMan, 18, 12, 18, 12, TRANSPARENT);
	PlaceBox(pacMan, 18, 13, 18, 13, TRANSPARENT);
	PlaceBox(pacMan, 17, 14, 18, 14, TRANSPARENT);
	PlaceBox(pacMan, 17, 15, 18, 15, TRANSPARENT);
	PlaceBox(pacMan, 16, 16, 18, 16, TRANSPARENT);
	PlaceBox(pacMan, 14, 17, 18, 17, TRANSPARENT);
	PlaceBox(pacMan, 13, 18, 18, 18, TRANSPARENT);

}

static void Move_Pacman(uint16_t ***pacMan, int xCenter, int yCenter, Direction NewDirection, Direction PrevDirection, void *virtual_base)
{

	switch(NewDirection)
	{
		case UP:
		//Move_PacmanUp();
		break;

		case DOWN:
		//Move_PacmanDown();
		break;

		case LEFT:
		//Move_PacmanLeft();
		break;

		case RIGHT:
		//Move_PacmanRight();
		break;
	}
}

static void ResetPacmanBuffer(uint16_t ***pacMan)
{
	//Middle too yellow
	PlaceBox(pacMan, 7, 0, 11, 18, PacMan_Color);
	//Left half too yellow
	PlaceBox(pacMan, 5, 1, 6, 17, PacMan_Color);
	PlaceBox(pacMan, 3, 2, 4, 16, PacMan_Color);
	PlaceBox(pacMan, 2, 3, 2, 15, PacMan_Color);
	PlaceBox(pacMan, 1, 5, 1, 13, PacMan_Color);
	PlaceBox(pacMan, 0, 7, 0, 11, PacMan_Color);
	//Right half too yellow
	PlaceBox(pacMan, 12, 1, 13, 17, PacMan_Color);
	PlaceBox(pacMan, 14, 2, 15, 16, PacMan_Color);
	PlaceBox(pacMan, 16, 3, 16, 15, PacMan_Color);
	PlaceBox(pacMan, 17, 5, 17, 13, PacMan_Color);
	PlaceBox(pacMan, 18, 7, 18, 11, PacMan_Color);	
}	
static void PacManBufferTooUp(uint16_t ***pacMan, int CurrPhase)
{
	//ResetPacmanBuffer(pacMan);
	switch(CurrPhase % 6)
	{
		case 0:		// Closed			
			break;

		case 1:		// Almost Closed
		case 5:
			PlaceBox(pacMan, 7, 0, 11, 2, TRANSPARENT);
			PlaceBox(pacMan, 8, 3, 10, 7, TRANSPARENT);
			PlaceBox(pacMan, 9, 9, 9, 8, TRANSPARENT);		
			break;
	
		case 2:		// Half Open
		case 4:
			PlaceBox(pacMan, 7, 0, 11, 6, TRANSPARENT);
			PlaceBox(pacMan, 5, 1, 6, 2, TRANSPARENT);
			PlaceBox(pacMan, 6, 3, 6, 3, TRANSPARENT);
			PlaceBox(pacMan, 12, 1, 13, 2, TRANSPARENT);
			PlaceBox(pacMan, 12, 3, 12, 3, TRANSPARENT);
			PlaceBox(pacMan, 8, 7, 10, 7, TRANSPARENT);
			PlaceBox(pacMan, 9, 8, 9, 9, TRANSPARENT);
			break;

		case 3:		// Fully Open
			PlaceBox(pacMan, 7, 0, 11, 7, TRANSPARENT);
			PlaceBox(pacMan, 5, 1, 6, 5, TRANSPARENT);
			PlaceBox(pacMan, 3, 2, 4, 3, TRANSPARENT);
			PlaceBox(pacMan, 4, 4, 4, 4, TRANSPARENT);
			PlaceBox(pacMan, 6, 6, 6, 6, TRANSPARENT);
			PlaceBox(pacMan, 8, 8, 10, 8, TRANSPARENT);
			PlaceBox(pacMan, 9, 9, 9, 9, TRANSPARENT);
			PlaceBox(pacMan, 12, 1, 13, 5, TRANSPARENT);
			PlaceBox(pacMan, 14, 2, 15, 3, TRANSPARENT);
			PlaceBox(pacMan, 12, 6, 12, 6, TRANSPARENT);
			PlaceBox(pacMan, 14, 4, 14, 4, TRANSPARENT);
			break;		
	}

}
static void PacManBufferTooDown(uint16_t ***pacMan, int CurrPhase)
{
	switch(CurrPhase % 6)
	{
		case 0:		// Closed			
			break;

		case 1:		// Almost Closed
		case 5:
			PlaceBox(pacMan, 7, 16, 11, 18, TRANSPARENT);
			PlaceBox(pacMan, 8, 11, 10, 15, TRANSPARENT);
			PlaceBox(pacMan, 9, 9, 9, 10, TRANSPARENT);		
			break;
	
		case 2:		// Half Open
		case 4:
			PlaceBox(pacMan, 7, 12, 11, 18, TRANSPARENT);
			PlaceBox(pacMan, 5, 16, 6, 17, TRANSPARENT);
			PlaceBox(pacMan, 6, 15, 6, 15, TRANSPARENT);
			PlaceBox(pacMan, 12, 16, 13, 17, TRANSPARENT);
			PlaceBox(pacMan, 12, 15, 12, 15, TRANSPARENT);
			PlaceBox(pacMan, 8, 11, 10, 11, TRANSPARENT);
			PlaceBox(pacMan, 9, 9, 9, 10, TRANSPARENT);
			PlaceBox(pacMan, 5, 16, 13, 18, TRANSPARENT); // debug 
			break;

		case 3:		// Fully Open
			PlaceBox(pacMan, 7, 11, 11, 18, TRANSPARENT);
			PlaceBox(pacMan, 8, 10, 10, 10, TRANSPARENT);
			PlaceBox(pacMan, 9, 9, 9, 9, TRANSPARENT);
			PlaceBox(pacMan, 5, 13, 6, 17, TRANSPARENT);
			PlaceBox(pacMan, 3, 15, 4, 16, TRANSPARENT);
			PlaceBox(pacMan, 4, 14, 4, 14, TRANSPARENT);
			PlaceBox(pacMan, 6, 12, 6, 12, TRANSPARENT);
			PlaceBox(pacMan, 12, 12, 12, 12, TRANSPARENT);
			PlaceBox(pacMan, 14, 14, 14, 14, TRANSPARENT);
			PlaceBox(pacMan, 12, 13, 13, 17, TRANSPARENT);
			PlaceBox(pacMan, 14, 15, 15, 16, TRANSPARENT);
			break;		
	}
}
static void PacManBufferTooLeft(uint16_t ***pacMan, int CurrPhase)
{
	switch(CurrPhase % 6)
	{
		case 0:		// Closed			
			break;

		case 1:		// Almost Closed
		case 5:
			PlaceBox(pacMan, 0, 7, 2, 11, TRANSPARENT);
			PlaceBox(pacMan, 3, 8, 7, 10, TRANSPARENT);
			PlaceBox(pacMan, 8, 9, 9, 9, TRANSPARENT);		
			break;
	
		case 2:		// Half Open
		case 4:
			PlaceBox(pacMan, 0, 7, 6, 11, TRANSPARENT);
			PlaceBox(pacMan, 7, 8, 7, 10, TRANSPARENT);
			PlaceBox(pacMan, 8, 9, 9, 9, TRANSPARENT);
			PlaceBox(pacMan, 1, 12, 2, 13, TRANSPARENT);
			PlaceBox(pacMan, 3, 12, 3, 12, TRANSPARENT);
			PlaceBox(pacMan, 1, 5, 2, 6, TRANSPARENT);
			PlaceBox(pacMan, 3, 6, 3, 6, TRANSPARENT);
			break;

		case 3:		// Fully Open
			PlaceBox(pacMan, 0, 7, 7, 11, TRANSPARENT);
			PlaceBox(pacMan, 1, 12, 5, 13, TRANSPARENT);
			PlaceBox(pacMan, 1, 5, 5, 6, TRANSPARENT);
			PlaceBox(pacMan, 2, 3, 3, 4, TRANSPARENT);
			PlaceBox(pacMan, 2, 14, 3, 15, TRANSPARENT);
			PlaceBox(pacMan, 4, 14, 4, 14, TRANSPARENT);
			PlaceBox(pacMan, 6, 12, 6, 12, TRANSPARENT);
			PlaceBox(pacMan, 4, 4, 4, 4, TRANSPARENT);
			PlaceBox(pacMan, 6, 6, 6, 6, TRANSPARENT);
			PlaceBox(pacMan, 8, 8, 8, 10, TRANSPARENT);
			PlaceBox(pacMan, 9, 9, 9, 9, TRANSPARENT);
			break;		
	}	
}
static void PacManBufferTooRight(uint16_t ***pacMan, int CurrPhase)
{
	switch(CurrPhase % 6)
	{
		case 0:		// Closed			
			break;

		case 1:		// Almost Closed
		case 5:
			PlaceBox(pacMan, 16, 7, 18, 11, TRANSPARENT);
			PlaceBox(pacMan, 11, 8, 15, 10, TRANSPARENT);
			PlaceBox(pacMan, 9, 9, 10, 9, TRANSPARENT);		
			break;
	
		case 2:		// Half Open
		case 4:
			PlaceBox(pacMan, 12, 7, 18, 11, TRANSPARENT);
			PlaceBox(pacMan, 16, 12, 17, 13, TRANSPARENT);
			PlaceBox(pacMan, 16, 5, 17, 6, TRANSPARENT);
			PlaceBox(pacMan, 15, 6, 15, 6, TRANSPARENT);
			PlaceBox(pacMan, 15, 12, 15, 12, TRANSPARENT);
			PlaceBox(pacMan, 9, 9, 10, 9, TRANSPARENT);
			PlaceBox(pacMan, 11, 8, 11, 10, TRANSPARENT);
			break;

		case 3:		// Fully Open
			PlaceBox(pacMan, 11, 7, 18, 11, TRANSPARENT);
			PlaceBox(pacMan, 13, 12, 17, 13, TRANSPARENT);
			PlaceBox(pacMan, 15, 14, 16, 15, TRANSPARENT);
			PlaceBox(pacMan, 12, 12, 12, 12, TRANSPARENT);
			PlaceBox(pacMan, 14, 14, 14, 14, TRANSPARENT);
			PlaceBox(pacMan, 13, 5, 17, 6, TRANSPARENT);
			PlaceBox(pacMan, 15, 3, 16, 4, TRANSPARENT);
			PlaceBox(pacMan, 14, 4, 14, 4, TRANSPARENT);
			PlaceBox(pacMan, 12, 6, 12, 6, TRANSPARENT);
			PlaceBox(pacMan, 10, 8, 10, 10, TRANSPARENT);
			PlaceBox(pacMan, 9, 9, 9, 9, TRANSPARENT);
			break;		
	}		
}
static bool CheckForLine(uint16_t **GameLinesArr, Coordinates CurrentCoordinates, Direction NewDirection){
	switch(NewDirection){
		case UP:
			return GameLinesArr[CurrentCoordinates.x][CurrentCoordinates.y - 1] == WHITE;

		case DOWN:
			return GameLinesArr[CurrentCoordinates.x][CurrentCoordinates.y + 1] == WHITE;

		case LEFT:
			return GameLinesArr[CurrentCoordinates.x - 1][CurrentCoordinates.y] == WHITE;

		case RIGHT:
			return GameLinesArr[CurrentCoordinates.x + 1][CurrentCoordinates.y] == WHITE;
		
		default:
			return false;
	}
}


static uint16_t** CreateArray(int xSize, int ySize){
	uint16_t **temp = malloc(sizeof(uint16_t*) * xSize);
	int i;
    for(i = 0; i < xSize; i++)
        temp[i] = malloc(sizeof(uint16_t) * ySize);

	return temp;
}

static void PlaceBox(uint16_t ***array, int x1, int y1, int x2, int y2, short color){
	uint32_t x, y;
	int xMin = MIN(x1, x2);
	int xMax = MAX(x1, x2);
	int yMin = MIN(y1, y2);
	int yMax = MAX(y1, y2);

	for (x = xMin; x <= xMax; x++)
		for (y = yMin; y <= yMax; ++y)
			PlacePixel(array, x , y, color);
}

static void PlaceLine(uint16_t ***array, int x1, int y1, int x2, int y2, short color){
	uint32_t row, col, xmax, xmin, ymax, ymin;
	
	if(x1 == x2 || y1 == y2){
		PlaceBox(array, x1, y1, x2, y2, color);
		return;
	}
	
	double m = ((double)(y2 - y1)) / ((double)(x2 - x1));	//Slope
	int b = -(m * x1) + y1;
	
	xmax = MIN(MAX(x1,x2), 640);
	xmin = MIN(MIN(x1,x2), 640);
	ymax = MIN(MAX(y1,y2), 480);
	ymin = MIN(MIN(y1,y2), 480);
	
	//printf("x1: %d, y1: %d, x2: %d, y2: %d, Slope: %f, Offset: %d\n", x1, y1, x2, y2, m, b);
	
	if(m >= -1 && m <= 1){	//Slope > 1
		for(row = xmin; row < xmax; row++){
			PlacePixel(array, row, (uint32_t)(m*row + b), color);
		}
	} else {
		for(col = ymin; col < ymax; col++){
			PlacePixel(array, (uint32_t) (((double)col-b)/m), col, color);
		}
	}
}

static void PlacePixel(uint16_t ***array, int x, int y, short color){
	(*array)[x][y] = color;
}

static void PlacePacMan(uint16_t ***array, uint16_t **gameboard, uint16_t ** pacman, Coordinates old, Coordinates new){
	int x,y;
	
	for(x = old.x-9; x < old.x+10; x++)
		for(y = old.y-9; y < old.y+10; y++)
			PlacePixel(array, x, y, gameboard[x][y]);

	for(x = new.x-9; x < new.x+10; x++)
		for(y = new.y-9; y < new.y+10; y++)
			PlacePixel(array, x, y, pacman[x - new.x+9][y - new.y+9]);
}

static void PlaceSmallDot(uint16_t ***combined, uint16_t ***dots, uint16_t x, uint16_t y){
	PlaceBox(combined, x-2, y-2, x+2, y+2, WHITE);
	PlaceBox(dots, x-2, y-2, x+2, y+2, WHITE);
}

static void PlaceBigDot(uint16_t ***combined, uint16_t ***dots, uint16_t x, uint16_t y){
	PlaceBox(combined, x-8, y-8, x+8, y+8, WHITE);
	PlaceBox(dots, x-8, y-8, x+8, y+8, WHITE);
}

static void PlaceSpriteWithTransparency(uint16_t **Combined, uint16_t **GameBoard, uint16_t ***Sprite, Coordinates spriteCoord){
	uint32_t x, y;
	for(x = 0; x < 19; x++)
		for(y = 0; y < 19; y++)
			if((*Sprite)[x][y] == TRANSPARENT){
				uint32_t tmpX = spriteCoord.x - 9 + x;
				uint32_t tmpY = spriteCoord.y - 9 + y;

				if(Combined[tmpX][tmpY] == WHITE)
					PlacePixel(Sprite, x, y, Combined[tmpX][tmpY]);
				else
					PlacePixel(Sprite, x, y, GameBoard[tmpX][tmpY]);		
			}		
}

static void IncrementScore()
{
	Score += 50;
	printf("%d\n",Score); 
}
#pragma endregion