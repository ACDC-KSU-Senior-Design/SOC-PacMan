#include "VGA.h"

#define HW_REGS_BASE ( 0xFC000000 )
#define HW_OCRAM_BASE ( 0xC8000000 )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define FPGA_CHAR_BASE      0xC9000000

#define PHYSMEM_32(addr) (*((unsigned int *)(virtual_base + (addr & HW_REGS_MASK))))
#define PHYSMEM_16(addr) (*((unsigned short *)(virtual_base + (addr & HW_REGS_MASK))))

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a > b ? b : a)

/****************************************************************************************
* Draw a filled rectangle on the VGA monitor 
****************************************************************************************/
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color, void *virtual_base){ 
	uint32_t row, col;

	/* assume that the box coordinates are valid */
	for (row = x1; row <= x2; row++)
		for (col = y1; col <= y2; ++col)
			VGA_setPixel(col, row, pixel_color, virtual_base);	
}

void VGA_text(int x, int y, char * text_ptr, void *virtual_base){
	int offset;
  	unsigned int  char_ptr;

	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while ( *(text_ptr) )
	{
		char_ptr =  FPGA_CHAR_BASE + offset;
		
		PHYSMEM_32(char_ptr) = *(text_ptr);
		
		++text_ptr;
		++offset;
	}
}

void VGA_text_clear(void *virtual_base){
	int x,y;
	int offset;
	char blank[1] = " ";
  	unsigned int  char_ptr;

	// Character coordinats are from 0,0 to 79,59 (x,y) position
	for(x = 0; x < 80; x++)
		for(y = 0; y < 60; y++)
		{
		/* assume that the text string fits on one line */
		offset = (y << 7) + x;
		char_ptr =  FPGA_CHAR_BASE + offset;
		PHYSMEM_32(char_ptr) = *blank;
		}
}

void VGA_box_clear(void *virtual_base){
	VGA_box(0,0,640,480, 0x0000, virtual_base);
}

void VGA_clear(void *virtual_base){
	VGA_box_clear(virtual_base);
	VGA_text_clear(virtual_base);
}

void VGA_draw_buffer(uint16_t** pixel_buff, void *virtual_base){
	int x, y;
	for(x = 0; x < SCREEN_WIDTH; x++)
		for(y = 0; y < SCREEN_HEIGHT; y++)
			VGA_setPixel(y, x, pixel_buff[x][y], virtual_base);	
}

void VGA_draw_buffer_NoColor(uint16_t **pixel_buff, short color, void *virtual_base){
	int x, y;
	for(x = 0; x < SCREEN_WIDTH; x++)
		for(y = 0; y < SCREEN_HEIGHT; y++)
			if(pixel_buff[x][y] != color)
				VGA_setPixel(y, x, pixel_buff[x][y], virtual_base);
}

void VGA_draw_buffer_Section(uint16_t **pixel_buff, int x1, int y1, int x2, int y2, void *virtual_base){
	uint16_t x, y, xMin, xMax, yMin, yMax;
	xMin = MIN(x1, x2);
	xMax = MAX(x1, x2);
	yMin = MIN(y1, y2);
	yMax = MAX(y1, y2);

	for(x = xMin; x < xMax; x++)
		for(y = yMin; y < yMax; y++)
			VGA_setPixel(y, x, pixel_buff[x][y], virtual_base);
}

void VGA_line(int x1, int y1, int x2, int y2, short pixel_color, void *virtual_base){
	uint32_t row, col, xmax, xmin, ymax, ymin;
	
	if(x1 == x2 || y1 == y2){
		VGA_box(x1, y1, x2, y2, pixel_color, virtual_base);
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
			VGA_setPixel(((uint32_t) (m*row + b)), row, pixel_color, virtual_base);
		}
	} else {
		for(col = ymin; col < ymax; col++){
			VGA_setPixel(col, ((uint32_t) (((double)col-b)/m)), pixel_color, virtual_base);
		}
	}
}

void VGA_circle(int x, int y, int radius, short pixel_color, void *virtual_base){
	int radSq = pow(radius,2);
	
	int i;
	for(i = x - radius; i < x + radius; i++){
		int y2 = sqrt(radSq - pow(i - x, 2));

		VGA_setPixel( y2 + y, i, pixel_color, virtual_base);
		VGA_setPixel(-y2 + y, i, pixel_color, virtual_base);
	}
	
	for(i = y - radius; i < y + radius; i++){
		int x2 = sqrt(radSq - pow(i - y, 2));
		
		VGA_setPixel(i,  x2 + x, pixel_color, virtual_base);
		VGA_setPixel(i, -x2 + x, pixel_color, virtual_base);
	}
}	

void VGA_setPixel(uint32_t x, uint32_t y, uint32_t pixel_color, void *virtual_base){
	uint32_t pixel_ptr = HW_OCRAM_BASE + (x << 10) + y; 	// New one for 640x480
	PHYSMEM_16(pixel_ptr) = pixel_color;					// set pixel color
}