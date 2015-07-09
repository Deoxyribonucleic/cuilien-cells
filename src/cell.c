#include "cell.h"
#include "world.h"
#include "mutate.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


cell_t cells[MAX_CELLS];

extern uint32_t time; // main.c (sorry)
extern world_t* world;

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

	world_get_tile(world, x, y)->cell = cell;

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

	world_get_tile(world, cell->x, cell->y)->cell = NULL;

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

int cell_move(cell_t* cell, int direction)
{
	int x = cell->x, y = cell->y;
	switch(direction)
	{
	case 0:
		++x;
		break;
	case 1:
		++y;
		break;
	case 2:
		--x;
		break;
	case 3:
		--y;
		break;
	default:
		break;
	}

	if(x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT &&
			world_get_tile(world, x, y)->cell == NULL)
	{
		world_get_tile(world, cell->x, cell->y)->cell = NULL;
		cell->x = x;
		cell->y = y;
		world_get_tile(world, cell->x, cell->y)->cell = cell;
		return 1;
	}

	return 0;
}

int cell_split(cell_t* cell)
{
	if(cell->mass > 5 && cells_get_count() != MAX_CELLS) // arbitrary threshold
	{
		int half_mass = cell->mass / 2;
		cell->mass -= half_mass;

		int x;
		int y;
		int found = 0;
		for(y = cell->y - 1; y <= cell->y + 1 && !found; ++y)
			for(x = cell->x - 1; x <= cell->x + 1; ++x)
				if(x >= 0 && x < WORLD_WIDTH &&
				   y >= 0 && y < WORLD_HEIGHT &&
				   world_get_tile(world, x, y)->cell == NULL)
				{
					found = 1;
					break;
				}

		/* no free tile */
		if(!found)
			return 0;

		printf("Split\n");
		++cell->times_split;
		cell_spawn(mutate(c_mem_copy(cell->process.context.memory)),
					cell->generation + 1, cell->color, half_mass, x, cell->y);

		return 1;
	}

	return 0;
}
