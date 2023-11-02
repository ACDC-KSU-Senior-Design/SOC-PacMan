#ifndef __PACMAN_H
#define __PACMAN_H

#include "VGA.h"

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
}Direction;

void InitGameBoard(uint16_t pixel_color, void *virtual_base);


#endif