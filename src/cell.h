#ifndef _CELL_H_
#define _CELL_H_

#include "limits.h"
#include "vm/process.h"
#include "vm/mem.h"

#define MAX_CELLS	1000


typedef struct cell
{
	c_process_t process;
	int x, y;
	uint32_t color;

	uint8_t mass;
	uint8_t alive;
	uint8_t signal;

	/* statistics */
	uint16_t generation, times_split;
	uint32_t birth, death;
	uint8_t save;
} cell_t;

cell_t* cell_spawn(c_mem_handle program, uint16_t generation, uint32_t color, uint8_t mass, int x, int y);
int cell_kill(cell_t* cell);
int cell_free(cell_t* cell);
uint32_t cell_lifetime(cell_t* cell);
int cell_move(cell_t* cell, int dirction);
int cell_split(cell_t* cell);

cell_t* cell_next(int* i);

void cells_init();
void cells_free();
int cells_get_count();

#endif

