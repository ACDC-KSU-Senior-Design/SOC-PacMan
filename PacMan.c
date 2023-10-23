#include "PacMan.h"

void InitGameBoard(short pixel_color, void *virtual_base){
	
	VGA_box(105,   3, 535,  24, pixel_color, virtual_base);
	VGA_box(105,  25, 126, 161, pixel_color, virtual_base);
	VGA_box(127, 140, 194, 161, pixel_color, virtual_base);
	VGA_box(173, 162, 194, 206, pixel_color, virtual_base);//Top Outside Boundry
	VGA_box( 82, 185, 172, 206, pixel_color, virtual_base);
	VGA_box(310,  25, 330,  70, pixel_color, virtual_base);
	VGA_box(514,  25, 535, 161, pixel_color, virtual_base);
	VGA_box(446, 140, 513, 161, pixel_color, virtual_base);
	VGA_box(446, 162, 467, 206, pixel_color, virtual_base);
	VGA_box(468, 185, 558, 206, pixel_color, virtual_base);
	
	VGA_box(151,  49, 194,  70, pixel_color, virtual_base);
	VGA_box(151,  94, 194, 115, pixel_color, virtual_base);	//TL Boxes
	VGA_box(219,  49, 285,  70, pixel_color, virtual_base);
	
	VGA_box(355,  49, 421,  70, pixel_color, virtual_base);
	VGA_box(446,  49, 489,  70, pixel_color, virtual_base);	//TR Boxes
	VGA_box(446,  94, 489, 115, pixel_color, virtual_base);
	
	VGA_box(219,  94, 240, 206, pixel_color, virtual_base);
	VGA_box(241, 140, 285, 161, pixel_color, virtual_base);	
	VGA_box(264,  94, 376, 115, pixel_color, virtual_base);
	VGA_box(310, 116, 330, 161, pixel_color, virtual_base);
	VGA_box(400,  94, 421, 206, pixel_color, virtual_base);	//TM Boxes
	VGA_box(355, 140, 399, 161, pixel_color, virtual_base);
	
	
	VGA_box(264, 185, 376, 206, pixel_color, virtual_base);
	VGA_box(264, 230, 376, 251, pixel_color, virtual_base);
	VGA_box(264, 207, 285, 229, pixel_color, virtual_base);	//Middle
	VGA_box(355, 207, 376, 230, pixel_color, virtual_base);
	VGA_box(310, 185, 330, 206, LIGHT_GREEN, virtual_base);	
	
	
	
	
}