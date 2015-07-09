#include "cell.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


cell_t cells[MAX_CELLS];
extern uint32_t time; // main.c (sorry)

cell_t* cell_spawn(c_mem_handle dna, uint16_t generation, uint32_t color, uint8_t mass, int x, int y)
{
	cell_t* cell;

	int i=0;
	while(i < MAX_CELLS && (cells[i].alive || cells[i].save)) ++i;
	if(i == MAX_CELLS)
	{
		printf("Cell spawn attempted despite max cell count being reached.\n");
		c_mem_free(dna);
		return NULL;
	}
	cell = &cells[i];

	printf("New cell index: %d\n", i);

	assert(dna != NULL);
	cell->process.context.memory = dna;
	cell->color = color;
	cell->mass = mass;
	cell->alive = 1;
	cell->x = x;
	cell->y = y;

	cell->generation = generation;
	cell->birth = time;
	cell->death = 0;
	cell->save = false;
	cell->times_split = 0;

	printf("Spawning cell.\n");
	// zero out registers
	memset(&cell->process.context.reg, 0, sizeof(cell->process.context.reg));
	cell->process.context.reg.ip = 12;
	cell->process.context.reg.sp = 0xFFFFFF00;

	printf("Cell spawned.\n");
	
	return cell;
}

int cell_kill(cell_t* cell)
{
	cell->alive = 0;
	cell->death = time;

	if(!cell->save)
		cell_free(cell);

	return 0;
}

int cell_free(cell_t* cell)
{
	c_mem_free(cell->process.context.memory);
	cell->alive = 0;

	return 0;
}

uint32_t cell_lifetime(cell_t* cell)
{
	int death = cell->death;
	if(cell->alive)
	{
		death = time;
	}
	return death - cell->birth;
}

cell_t* cell_next(int* i)
{
	do
	{
		if(cells[*i].alive)
			return &cells[(*i)++];
	}
	while(++*i < MAX_CELLS);
	return NULL;
}

void cells_init()
{
	memset(cells, 0, sizeof(cells));
}

void cells_free()
{
	int i = 0;
	cell_t* cell;
	while((cell = cell_next(&i)))
	{
		cell_free(cell);
	}
}

int cells_get_count()
{
	int count = 0;
	int i;
	for(i=0; i<MAX_CELLS; ++i)
	{
		if(cells[i].alive || cells[i].save)
			++count;
	}
	return count;
}

