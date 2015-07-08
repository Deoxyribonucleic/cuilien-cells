#include "cell.h"


int cell_create(cell_t* cell, c_mem_handle dna, uint32_t color, uint8_t mass, int x, int y)
{
	cell->process.context.memory = dna;
	cell->color = color;
	cell->mass = mass;
	cell->x = x;
	cell->y = y;

	// zero out registers
	memset(&cell->process.context.reg, 0, sizeof(cell->process.context.reg));
	cell->process.context.reg.ip = 12;
	cell->process.context.reg.sp = 0xFFFFFF00;
	
	return 0;
}

int cell_destroy(cell_t* cell)
{
	c_mem_free(cell->process.context.memory);

	return 0;
}
