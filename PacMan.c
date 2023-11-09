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
static void PlaceSmallDot(uint16_t ***array, uint16_t x, uint16_t y);
static void PlaceBigDot(uint16_t ***array, uint16_t x, uint16_t y);

static void InitializePacMan(uint16_t ***pacMan);
static void Move_Pacman(uint16_t ***pacMan, int xCenter, int yCenter, Direction NewDirection, Direction PrevDirection, void *virtual_base);
static void PacManBufferTooUp(uint16_t ***pacMan, int CurrPhase);
static void PacManBufferTooLeft(uint16_t ***pacMan, int CurrPhase);
static void PacManBufferTooRight(uint16_t ***pacMan, int CurrPhase);
static void PacManBufferTooDown(uint16_t ***pacMan, int CurrPhase);
static void ResetPacmanBuffer(uint16_t ***pacMan);


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

	int x, y;
	for(x = 0; x < SCREEN_WIDTH; x++)
		for(y = 0; y < SCREEN_HEIGHT; y++)
			PlacePixel(&Combined, x,y,GameBoard[x][y]);	//Copies the Gameboard into Combined
	#pragma endregion

	#pragma region InitPacMan
		InitializePacMan(&PacMan);
	#pragma endregion

	#pragma region InitDots
	PlaceSmallDot(&Combined, 139, 445);
	PlaceSmallDot(&Combined, 161, 445);
	PlaceSmallDot(&Combined, 184, 445);
	PlaceSmallDot(&Combined, 207, 445);
	PlaceSmallDot(&Combined, 230, 445);
	PlaceSmallDot(&Combined, 252, 445);
	PlaceSmallDot(&Combined, 275, 445);
	PlaceSmallDot(&Combined, 298, 445);
	PlaceSmallDot(&Combined, 320, 445);
	PlaceSmallDot(&Combined, 343, 445);
	PlaceSmallDot(&Combined, 366, 445);
	PlaceSmallDot(&Combined, 389, 445);
	PlaceSmallDot(&Combined, 411, 445);
	PlaceSmallDot(&Combined, 434, 445);
	PlaceSmallDot(&Combined, 457, 445);
	PlaceSmallDot(&Combined, 479, 445);
	PlaceSmallDot(&Combined, 502, 445); //Bottom Row

	PlaceSmallDot(&Combined, 139, 36);
	PlaceSmallDot(&Combined, 161, 36);
	PlaceSmallDot(&Combined, 184, 36);
	PlaceSmallDot(&Combined, 207, 36);
	PlaceSmallDot(&Combined, 230, 36);
	PlaceSmallDot(&Combined, 252, 36);
	PlaceSmallDot(&Combined, 275, 36);
	PlaceSmallDot(&Combined, 298, 36);
	PlaceSmallDot(&Combined, 343, 36);
	PlaceSmallDot(&Combined, 366, 36);
	PlaceSmallDot(&Combined, 389, 36);
	PlaceSmallDot(&Combined, 411, 36);
	PlaceSmallDot(&Combined, 434, 36);
	PlaceSmallDot(&Combined, 457, 36);
	PlaceSmallDot(&Combined, 479, 36);
	PlaceSmallDot(&Combined, 502, 36); //Top Row

	PlaceSmallDot(&Combined, 139, 82);
	PlaceSmallDot(&Combined, 161, 82);
	PlaceSmallDot(&Combined, 184, 82);
	PlaceSmallDot(&Combined, 207, 82);
	PlaceSmallDot(&Combined, 230, 82);
	PlaceSmallDot(&Combined, 252, 82);
	PlaceSmallDot(&Combined, 275, 82);
	PlaceSmallDot(&Combined, 298, 82);
	PlaceSmallDot(&Combined, 320, 82);
	PlaceSmallDot(&Combined, 343, 82);
	PlaceSmallDot(&Combined, 366, 82);
	PlaceSmallDot(&Combined, 389, 82);
	PlaceSmallDot(&Combined, 411, 82);
	PlaceSmallDot(&Combined, 434, 82);
	PlaceSmallDot(&Combined, 457, 82);
	PlaceSmallDot(&Combined, 479, 82);
	PlaceSmallDot(&Combined, 502, 82); //Second Top Row

	PlaceSmallDot(&Combined, 139, 309);
	PlaceSmallDot(&Combined, 161, 309);
	PlaceSmallDot(&Combined, 184, 309);
	PlaceSmallDot(&Combined, 207, 309);
	PlaceSmallDot(&Combined, 230, 309);
	PlaceSmallDot(&Combined, 252, 309);
	PlaceSmallDot(&Combined, 275, 309);
	PlaceSmallDot(&Combined, 298, 309);	//Causing Small Box Issue...??
	PlaceSmallDot(&Combined, 298, 309);	//Causing Small Box Issue...??
	PlaceSmallDot(&Combined, 343, 309);
	PlaceSmallDot(&Combined, 366, 309);
	PlaceSmallDot(&Combined, 389, 309);
	PlaceSmallDot(&Combined, 411, 309);
	PlaceSmallDot(&Combined, 434, 309);
	PlaceSmallDot(&Combined, 457, 309);
	PlaceSmallDot(&Combined, 479, 309);
	PlaceSmallDot(&Combined, 502, 309); //4th Row from bottom

	PlaceBigDot(  &Combined, 139, 354);	//BIG DOT
	PlaceSmallDot(&Combined, 161, 354);
	PlaceSmallDot(&Combined, 207, 354);
	PlaceSmallDot(&Combined, 230, 354);
	PlaceSmallDot(&Combined, 252, 354);
	PlaceSmallDot(&Combined, 275, 354);
	PlaceSmallDot(&Combined, 298, 354);
	PlaceSmallDot(&Combined, 320, 354);
	PlaceSmallDot(&Combined, 343, 354);
	PlaceSmallDot(&Combined, 366, 354);
	PlaceSmallDot(&Combined, 389, 354);
	PlaceSmallDot(&Combined, 411, 354);
	PlaceSmallDot(&Combined, 434, 354);
	PlaceSmallDot(&Combined, 479, 354);	//3rd row from bottom
	PlaceBigDot(  &Combined, 502, 354); //BIG DOT

	PlaceSmallDot(&Combined, 139, 400);
	PlaceSmallDot(&Combined, 161, 400);
	PlaceSmallDot(&Combined, 184, 400);
	PlaceSmallDot(&Combined, 207, 400);
	PlaceSmallDot(&Combined, 252, 400);
	PlaceSmallDot(&Combined, 275, 400);
	PlaceSmallDot(&Combined, 298, 400);
	PlaceSmallDot(&Combined, 343, 400);
	PlaceSmallDot(&Combined, 366, 400);
	PlaceSmallDot(&Combined, 389, 400);
	PlaceSmallDot(&Combined, 434, 400);
	PlaceSmallDot(&Combined, 457, 400);
	PlaceSmallDot(&Combined, 479, 400);
	PlaceSmallDot(&Combined, 502, 400); //2nd Row from bottom

	PlaceSmallDot(&Combined, 139, 331);
	PlaceSmallDot(&Combined, 298, 331);
	PlaceSmallDot(&Combined, 343, 331);
	PlaceSmallDot(&Combined, 502, 331); //misc dots

	PlaceSmallDot(&Combined, 161, 377);
	PlaceSmallDot(&Combined, 207, 377);
	PlaceSmallDot(&Combined, 252, 377);
	PlaceSmallDot(&Combined, 389, 377);
	PlaceSmallDot(&Combined, 434, 377);
	PlaceSmallDot(&Combined, 479, 377);	//misc dots

	PlaceSmallDot(&Combined, 139, 422);
	PlaceSmallDot(&Combined, 298, 422);
	PlaceSmallDot(&Combined, 343, 422);
	PlaceSmallDot(&Combined, 502, 422);

	PlaceBigDot(  &Combined, 139, 58);
	PlaceSmallDot(&Combined, 298, 58);
	PlaceSmallDot(&Combined, 343, 58);
	PlaceBigDot(  &Combined, 502, 58);

	PlaceSmallDot(&Combined, 252, 104);
	PlaceSmallDot(&Combined, 252, 127);
	PlaceSmallDot(&Combined, 275, 127);
	PlaceSmallDot(&Combined, 298, 127);

	PlaceSmallDot(&Combined, 389, 104);
	PlaceSmallDot(&Combined, 389, 127);
	PlaceSmallDot(&Combined, 366, 127);
	PlaceSmallDot(&Combined, 343, 127); 

	PlaceSmallDot(&Combined, 139, 104);
	PlaceSmallDot(&Combined, 139, 127);
	PlaceSmallDot(&Combined, 161, 127);
	PlaceSmallDot(&Combined, 184, 127);
	PlaceSmallDot(&Combined, 457, 127);
	PlaceSmallDot(&Combined, 479, 127);
	PlaceSmallDot(&Combined, 502, 127);
	PlaceSmallDot(&Combined, 502, 104);


	PlaceSmallDot(&Combined, 434, 59 ); 
	PlaceSmallDot(&Combined, 434, 104);
	PlaceSmallDot(&Combined, 434, 127);
	PlaceSmallDot(&Combined, 434, 150);
	PlaceSmallDot(&Combined, 434, 173);
	PlaceSmallDot(&Combined, 434, 195);
	PlaceSmallDot(&Combined, 434, 218);
	PlaceSmallDot(&Combined, 434, 241);
	PlaceSmallDot(&Combined, 434, 263);
	PlaceSmallDot(&Combined, 434, 286);
	PlaceSmallDot(&Combined, 434, 331);
	PlaceSmallDot(&Combined, 434, 377); //Left Vertical Column

	PlaceSmallDot(&Combined, 207, 59 ); 
	PlaceSmallDot(&Combined, 207, 104);
	PlaceSmallDot(&Combined, 207, 127);
	PlaceSmallDot(&Combined, 207, 150);
	PlaceSmallDot(&Combined, 207, 173);
	PlaceSmallDot(&Combined, 207, 195);
	PlaceSmallDot(&Combined, 207, 218);
	PlaceSmallDot(&Combined, 207, 241);
	PlaceSmallDot(&Combined, 207, 263);
	PlaceSmallDot(&Combined, 207, 286);
	PlaceSmallDot(&Combined, 207, 331);
	PlaceSmallDot(&Combined, 207, 377); //Right Vertical Column

	#pragma endregion


	Coordinates old = {300,300};
	Coordinates new = {321,264};	//Starting Coordinates

	int i;
	while(1)
	{
		for (i = 0; i <= 5; i++)
		{
			ResetPacmanBuffer(&PacMan);
			PacManBufferTooRight(&PacMan, i);
			PlacePacMan(&Combined, GameBoard, PacMan, old, new);
			VGA_draw_buffer(Combined, virtual_base);		
		}
		for (i = 0; i <= 5; i++)
		{
			ResetPacmanBuffer(&PacMan);
			PacManBufferTooLeft(&PacMan, i);
			PlacePacMan(&Combined, GameBoard, PacMan, old, new);
			VGA_draw_buffer(Combined, virtual_base);			
		}
		for (i = 0; i <= 5; i++)
		{
			ResetPacmanBuffer(&PacMan);
			PacManBufferTooUp(&PacMan, i);
			PlacePacMan(&Combined, GameBoard, PacMan, old, new);
			VGA_draw_buffer(Combined, virtual_base);			
		}
		for (i = 0; i <= 5; i++)
		{
			ResetPacmanBuffer(&PacMan);
			PacManBufferTooDown(&PacMan, i);
			PlacePacMan(&Combined, GameBoard, PacMan, old, new);
			VGA_draw_buffer(Combined, virtual_base);			
		}

	}


	PacManBufferTooRight(&PacMan, 1);
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
	//ResetPacmanBuffer(pacMan);
	switch(CurrPhase)
	{
		case 0:		// Closed			
			break;

		case 1:		// Almost Closed
		case 5:
			PlaceBox(pacMan, 7, 0, 11, 2, BLACK);
			PlaceBox(pacMan, 8, 3, 10, 7, BLACK);
			PlaceBox(pacMan, 9, 9, 9, 8, BLACK);		
			break;
	
		case 2:		// Half Open
		case 4:
			PlaceBox(pacMan, 7, 0, 11, 6, BLACK);
			PlaceBox(pacMan, 5, 1, 6, 2, BLACK);
			PlaceBox(pacMan, 6, 3, 6, 3, BLACK);
			PlaceBox(pacMan, 12, 1, 13, 2, BLACK);
			PlaceBox(pacMan, 12, 3, 12, 3, BLACK);
			PlaceBox(pacMan, 8, 7, 10, 7, BLACK);
			PlaceBox(pacMan, 9, 8, 9, 9, BLACK);
			break;

		case 3:		// Fully Open
			PlaceBox(pacMan, 7, 0, 11, 7, BLACK);
			PlaceBox(pacMan, 5, 1, 6, 5, BLACK);
			PlaceBox(pacMan, 3, 2, 4, 3, BLACK);
			PlaceBox(pacMan, 4, 4, 4, 4, BLACK);
			PlaceBox(pacMan, 6, 6, 6, 6, BLACK);
			PlaceBox(pacMan, 8, 8, 10, 8, BLACK);
			PlaceBox(pacMan, 9, 9, 9, 9, BLACK);
			PlaceBox(pacMan, 12, 1, 13, 5, BLACK);
			PlaceBox(pacMan, 14, 2, 15, 3, BLACK);
			PlaceBox(pacMan, 12, 6, 12, 6, BLACK);
			PlaceBox(pacMan, 14, 4, 14, 4, BLACK);
			break;		
	}

}
static void PacManBufferTooDown(uint16_t ***pacMan, int CurrPhase)
{
	switch(CurrPhase)
	{
		case 0:		// Closed			
			break;

		case 1:		// Almost Closed
		case 5:
			PlaceBox(pacMan, 7, 16, 11, 18, BLACK);
			PlaceBox(pacMan, 8, 11, 10, 15, BLACK);
			PlaceBox(pacMan, 9, 9, 9, 10, BLACK);		
			break;
	
		case 2:		// Half Open
		case 4:
			PlaceBox(pacMan, 7, 12, 11, 18, BLACK);
			PlaceBox(pacMan, 5, 16, 6, 17, BLACK);
			PlaceBox(pacMan, 6, 15, 6, 15, BLACK);
			PlaceBox(pacMan, 12, 16, 13, 17, BLACK);
			PlaceBox(pacMan, 12, 15, 12, 15, BLACK);
			PlaceBox(pacMan, 8, 11, 10, 11, BLACK);
			PlaceBox(pacMan, 9, 9, 9, 10, BLACK);
			PlaceBox(pacMan, 5, 16, 13, 18, BLACK); // debug 
			break;

		case 3:		// Fully Open
			PlaceBox(pacMan, 7, 11, 11, 18, BLACK);
			PlaceBox(pacMan, 8, 10, 10, 10, BLACK);
			PlaceBox(pacMan, 9, 9, 9, 9, BLACK);
			PlaceBox(pacMan, 5, 13, 6, 17, BLACK);
			PlaceBox(pacMan, 3, 15, 4, 16, BLACK);
			PlaceBox(pacMan, 4, 14, 4, 14, BLACK);
			PlaceBox(pacMan, 6, 12, 6, 12, BLACK);
			PlaceBox(pacMan, 12, 12, 12, 12, BLACK);
			PlaceBox(pacMan, 14, 14, 14, 14, BLACK);
			PlaceBox(pacMan, 12, 13, 13, 17, BLACK);
			PlaceBox(pacMan, 14, 15, 15, 16, BLACK);
			break;		
	}
}
static void PacManBufferTooLeft(uint16_t ***pacMan, int CurrPhase)
{
	switch(CurrPhase)
	{
		case 0:		// Closed			
			break;

		case 1:		// Almost Closed
		case 5:
			PlaceBox(pacMan, 0, 7, 2, 11, BLACK);
			PlaceBox(pacMan, 3, 8, 7, 10, BLACK);
			PlaceBox(pacMan, 8, 9, 9, 9, BLACK);		
			break;
	
		case 2:		// Half Open
		case 4:
			PlaceBox(pacMan, 0, 7, 6, 11, BLACK);
			PlaceBox(pacMan, 7, 8, 7, 10, BLACK);
			PlaceBox(pacMan, 8, 9, 9, 9, BLACK);
			PlaceBox(pacMan, 1, 12, 2, 13, BLACK);
			PlaceBox(pacMan, 3, 12, 3, 12, BLACK);
			PlaceBox(pacMan, 1, 5, 2, 6, BLACK);
			PlaceBox(pacMan, 3, 6, 3, 6, BLACK);
			break;

		case 3:		// Fully Open
			PlaceBox(pacMan, 0, 7, 7, 11, BLACK);
			PlaceBox(pacMan, 1, 12, 5, 13, BLACK);
			PlaceBox(pacMan, 1, 5, 5, 6, BLACK);
			PlaceBox(pacMan, 2, 3, 3, 4, BLACK);
			PlaceBox(pacMan, 2, 14, 3, 15, BLACK);
			PlaceBox(pacMan, 4, 14, 4, 14, BLACK);
			PlaceBox(pacMan, 6, 12, 6, 12, BLACK);
			PlaceBox(pacMan, 4, 4, 4, 4, BLACK);
			PlaceBox(pacMan, 6, 6, 6, 6, BLACK);
			PlaceBox(pacMan, 8, 8, 8, 10, BLACK);
			PlaceBox(pacMan, 9, 9, 9, 9, BLACK);
			break;		
	}	
}
static void PacManBufferTooRight(uint16_t ***pacMan, int CurrPhase)
{
	switch(CurrPhase)
	{
		case 0:		// Closed			
			break;

		case 1:		// Almost Closed
		case 5:
			PlaceBox(pacMan, 16, 7, 18, 11, BLACK);
			PlaceBox(pacMan, 11, 8, 15, 10, BLACK);
			PlaceBox(pacMan, 9, 9, 10, 9, BLACK);		
			break;
	
		case 2:		// Half Open
		case 4:
			PlaceBox(pacMan, 12, 7, 18, 11, BLACK);
			PlaceBox(pacMan, 16, 12, 17, 13, BLACK);
			PlaceBox(pacMan, 16, 5, 17, 6, BLACK);
			PlaceBox(pacMan, 15, 6, 15, 6, BLACK);
			PlaceBox(pacMan, 15, 12, 15, 12, BLACK);
			PlaceBox(pacMan, 9, 9, 10, 9, BLACK);
			PlaceBox(pacMan, 11, 8, 11, 10, BLACK);
			break;

		case 3:		// Fully Open
			PlaceBox(pacMan, 11, 7, 18, 11, BLACK);
			PlaceBox(pacMan, 13, 12, 17, 13, BLACK);
			PlaceBox(pacMan, 15, 14, 16, 15, BLACK);
			PlaceBox(pacMan, 12, 12, 12, 12, BLACK);
			PlaceBox(pacMan, 14, 14, 14, 14, BLACK);
			PlaceBox(pacMan, 13, 5, 17, 6, BLACK);
			PlaceBox(pacMan, 15, 3, 16, 4, BLACK);
			PlaceBox(pacMan, 14, 4, 14, 4, BLACK);
			PlaceBox(pacMan, 12, 6, 12, 6, BLACK);
			PlaceBox(pacMan, 10, 8, 10, 10, BLACK);
			PlaceBox(pacMan, 9, 9, 9, 9, BLACK);
			break;		
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

static void PlaceSmallDot(uint16_t ***array, uint16_t x, uint16_t y){
	PlaceBox(array, x-2, y-2, x+2, y+2, WHITE);
}

static void PlaceBigDot(uint16_t ***array, uint16_t x, uint16_t y){
	PlaceBox(array, x-8, y-8, x+8, y+8, WHITE);
}
