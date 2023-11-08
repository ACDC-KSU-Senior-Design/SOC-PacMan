#include "PacMan.h"
#include "stdlib.h"

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a > b ? b : a)

#define PackMan_Color 0xFD

typedef struct{
    uint16_t x;
    uint16_t y;
}Coordinates;

// https://www.pixilart.com/draw/mine-is-the-coordinates-4e1b3d00ba4c277


static uint16_t **GameBoard;
static uint16_t **PacMan;
static uint16_t **Combined;

// Pacman Variables
static Direction NewDirection;
static Direction PrevDirection;
// 0=closed, 1=almost_closed, half_open, fully_open, half_open, almost_closed
static int AnimationPhase = 0;


static uint16_t** CreateArray(int xSize, int ySize);
static void PlaceBox(uint16_t ***array, int x1, int y1, int x2, int y2, short color);
static void PlacePixel(uint16_t ***array, int x, int y, short color);
static void PlacePacMan(uint16_t ***array, uint16_t **gameboard, uint16_t ** pacman, Coordinates old, Coordinates new);

static void InitializePacMan(uint16_t ***pacMan);
static void Move_Pacman(uint16_t ***pacMan, int xCenter, int yCenter, Direction NewDirection, Direction PrevDirection, void *virtual_base);
static void Move_PacmanUp(uint16_t ***pacMan, int xCenter, int yCenter, void *virtual_base);
static void Move_PacmanDown(uint16_t ***pacMan, int xCenter, int yCenter, void *virtual_base);
static void Move_PacmanLeft(uint16_t ***pacMan, int xCenter, int yCenter, void *virtual_base);
static void Move_PacmanRight(uint16_t ***pacMan, int xCenter, int yCenter, void *virtual_base);
static void ClearPacman(uint16_t ***pacMan);


void InitGameBoard(uint16_t pixel_color, void *virtual_base){

    GameBoard = CreateArray(SCREEN_WIDTH, SCREEN_HEIGHT);
	PacMan = CreateArray(19, 19);
	Combined = CreateArray(SCREEN_WIDTH, SCREEN_HEIGHT);
	
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
	#pragma endregion

	#pragma region InitPacMan
		InitializePacMan(&PacMan);
	#pragma endregion

	int x, y;
	for(x = 0; x < SCREEN_WIDTH; x++)
		for(y = 0; y < SCREEN_HEIGHT; y++)
			PlacePixel(&Combined, x,y,GameBoard[x][y]);	//Copies the Gameboard into Combined

	Coordinates old = {300,300};
	Coordinates new = {321,264};	//Starting Coordinates
	PlacePacMan(&Combined, GameBoard, PacMan, old, new);

	//Dark Blue 0xce43
	VGA_draw_buffer(Combined, virtual_base);
}

static void InitializePacMan(uint16_t ***pacMan)
{
	PlaceBox(pacMan, 0, 0, 18, 18, PackMan_Color);
	// Top Left Corner too black
	PlaceBox(pacMan, 0, 0, 6, 0, BLACK);
	PlaceBox(pacMan, 0, 1, 4, 1, BLACK);
	PlaceBox(pacMan, 0, 2, 2, 2, BLACK);
	PlaceBox(pacMan, 0, 3, 1, 3, BLACK);
	PlaceBox(pacMan, 0, 4, 1, 4, BLACK);
	PlaceBox(pacMan, 0, 5, 0, 5, BLACK);
	PlaceBox(pacMan, 0, 6, 0, 6, BLACK);

	// Bottom Left Corner too black
	PlaceBox(pacMan, 0, 12, 0, 12, BLACK);
	PlaceBox(pacMan, 0, 13, 0, 13, BLACK);
	PlaceBox(pacMan, 0, 14, 1, 14, BLACK);
	PlaceBox(pacMan, 0, 15, 1, 15, BLACK);
	PlaceBox(pacMan, 0, 16, 2, 16, BLACK);
	PlaceBox(pacMan, 0, 17, 5, 17, BLACK);
	PlaceBox(pacMan, 0, 18, 6, 18, BLACK);

	// Top Right Corner too black
	PlaceBox(pacMan, 12, 0, 18, 0, BLACK);
	PlaceBox(pacMan, 14, 0, 18, 1, BLACK);
	PlaceBox(pacMan, 16, 1, 18, 2, BLACK);
	PlaceBox(pacMan, 17, 1, 18, 3, BLACK);
	PlaceBox(pacMan, 17, 2, 18, 4, BLACK);
	PlaceBox(pacMan, 18, 4, 18, 5, BLACK);
	PlaceBox(pacMan, 18, 6, 18, 6, BLACK);

	// Bottom Right Corner too black
	PlaceBox(pacMan, 18, 12, 18, 12, BLACK);
	PlaceBox(pacMan, 18, 13, 18, 13, BLACK);
	PlaceBox(pacMan, 17, 14, 18, 14, BLACK);
	PlaceBox(pacMan, 17, 15, 18, 15, BLACK);
	PlaceBox(pacMan, 16, 16, 18, 16, BLACK);
	PlaceBox(pacMan, 14, 17, 18, 17, BLACK);
	PlaceBox(pacMan, 13, 18, 18, 18, BLACK);

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
	PlaceBox(pacMan, 7, 0, 11, 18, PackMan_Color);
	//Left half too yellow
	PlaceBox(pacMan, 5, 1, 6, 17, PackMan_Color);
	PlaceBox(pacMan, 3, 2, 4, 16, PackMan_Color);
	PlaceBox(pacMan, 2, 3, 2, 15, PackMan_Color);
	PlaceBox(pacMan, 1, 5, 1, 13, PackMan_Color);
	PlaceBox(pacMan, 0, 7, 0, 11, PackMan_Color);
	//Right half too yellow
	PlaceBox(pacMan, 12, 1, 13, 17, PackMan_Color);
	PlaceBox(pacMan, 14, 2, 15, 16, PackMan_Color);
	PlaceBox(pacMan, 16, 3, 16, 15, PackMan_Color);
	PlaceBox(pacMan, 17, 5, 17, 13, PackMan_Color);
	PlaceBox(pacMan, 18, 7, 18, 11, PackMan_Color);	
}	
static void PacManBufferTooUp(uint16_t ***pacMan, int CurrPhase)
{
	switch(CurrPhase)
	{
		case 0:		// Closed			
			break;

		case 1:		// Almost Closed

			break;

		case 4:	
		case 2:		// Half Open
			break;

		case 3:		// Fully Open
			PlaceBox(pacMan, 7, 0, 11, 6, PackMan_Color);
			PlaceBox(pacMan, 5, 1, 6, 4, PackMan_Color);
			PlaceBox(pacMan, 4, 3, 4, 3, PackMan_Color);
			PlaceBox(pacMan, 3, 2, 3, 2, PackMan_Color);
			PlaceBox(pacMan, 6, 5, 6, 5, PackMan_Color);
			PlaceBox(pacMan, 8, 7, 10, 7, PackMan_Color);
			PlaceBox(pacMan, 9, 8, 9, 8, PackMan_Color);
			PlaceBox(pacMan, 12, 1, 13, 4, PackMan_Color);
			PlaceBox(pacMan, 14, 2, 14, 3, PackMan_Color);
			PlaceBox(pacMan, 15, 2, 15, 2, PackMan_Color);
			PlaceBox(pacMan, 12, 5, 12, 5, PackMan_Color);
			break;

			// Half Open
			break;

		case 5:		// Almost Closed
			break;		
	}

}
static void Move_PacmanDown(uint16_t ***pacMan, int xCenter, int yCenter, void *virtual_base){}
static void Move_PacmanLeft(uint16_t ***pacMan, int xCenter, int yCenter, void *virtual_base){}
static void Move_PacmanRight(uint16_t ***pacMan, int xCenter, int yCenter, void *virtual_base){}



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

