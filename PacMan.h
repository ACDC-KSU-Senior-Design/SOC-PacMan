#ifndef __PACMAN_H
#define __PACMAN_H

#include "VGA.h"
#include <stdbool.h>

 
typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
}Direction;


void InitGameBoard(void *virtual_base);
bool MovePacman(Direction NewDirection, void *virtual_base);
void FreeGameBoard();
 


#endif