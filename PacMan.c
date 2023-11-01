#include "PacMan.h"
#include "stdlib.h"

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a > b ? b : a)

static uint16_t **GameBoard;
static uint16_t **PacMan;
static uint16_t **Combined;

static uint16_t** CreateArray(int xSize, int ySize);
static void PlaceBox(uint16_t ***array, int x1, int y1, int x2, int y2, short color);
static void PlacePixel(uint16_t ***array, int x, int y, short color);

void InitGameBoard(uint16_t pixel_color, void *virtual_base){

    GameBoard = CreateArray(SCREEN_WIDTH, SCREEN_HEIGHT);
	PacMan = CreateArray(SCREEN_WIDTH, SCREEN_HEIGHT);
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
	PlaceBox(&PacMan, 200,200,300,300, DARK_PINK);
	#pragma endregion
	//Dark Blue 0xce43
	//PlaceBox(&GameBoard, 200,200,400,400, 0x2000);

	VGA_draw_buffer_NoColor(GameBoard, BLACK, virtual_base);
	VGA_draw_buffer_NoColor(PacMan   , BLACK, virtual_base);
	//VGA_draw_buffer(&GameBoard, virtual_base);
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
