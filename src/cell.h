#ifndef _CELL_H_
#define _CELL_H_

#include "limits.h"
#include "vm/process.h"
#include "vm/mem.h"


typedef struct cell
{
	c_process_t process;
	int x, y;
	uint32_t color;
	uint8_t mass;
} cell_t;

int cell_create(cell_t* cell, c_mem_handle program, uint32_t color, uint8_t mass, int x, int y);
int cell_destroy(cell_t* cell);

#endif

