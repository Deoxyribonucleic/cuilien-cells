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
	union
	{
		uint8_t mass;
		uint8_t alive;
	};
} cell_t;

cell_t* cell_spawn(c_mem_handle program, uint32_t color, uint8_t mass, int x, int y);
int cell_kill(cell_t* cell);

cell_t* cell_next(int* i);

void cells_init();
void cells_free();

#endif

