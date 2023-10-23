#ifndef __VGA_H__
#define __VGA_H__

#include <math.h>
#include <stdint.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define BLACK 		0x0000
#define LIGHT_BLUE  0x3333
#define DARK_BLUE   0x2222
#define LIGHT_GREEN 0x5555
#define DARK_GREEN  0x1111
#define WHITE		0xFFFF

void VGA_clear(void *virtual_base);

void VGA_box(int x1, int y1, int x2, int y2, short pixel_color, void *virtual_base);
void VGA_box_clear(void *virtual_base);

void VGA_text(int x, int y, char * text_ptr, void *virtual_base);
void VGA_text_clear(void *virtual_base);

void VGA_line(int x1, int y1, int x2, int y2, short pixel_color, void *virtual_base);
void VGA_circle(int x, int y, int radius, short pixel_color, void *virtual_base);
void VGA_setPixel(uint32_t col, uint32_t row, uint32_t pixel_color, void *virtual_base);

#endif