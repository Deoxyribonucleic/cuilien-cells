#include "interaction.h"
#include "cell.h"
#include "world.h"
#include "mutate.h"

#include "vm/cpu.h"

#include <stdio.h>
#include <stdlib.h>


extern cell_t* current_cell;
extern world_t* world;

void random(c_cpu_t* cpu)
{
	cpu->context->reg.a = rand();
}

void move(c_cpu_t* cpu)
{
	switch(cpu->context->reg.a % 4)
	{
	case 0:
		if(++current_cell->x >= WORLD_WIDTH)
			current_cell->x = WORLD_WIDTH - 1;
		break;
	case 1:
		if(++current_cell->y >= WORLD_HEIGHT)
			current_cell->y = WORLD_HEIGHT - 1;
		break;
	case 2:
		if(--current_cell->x < 0)
			current_cell->x = 0;
		break;
	case 3:
		if(--current_cell->y < 0)
			current_cell->y = 0;
		break;
	default:
		break;
	}
}

void eat(c_cpu_t* cpu)
{
	if(world_remove_food(world, current_cell->x, current_cell->y))
	{
		if(current_cell->mass < 255)
			++current_cell->mass;
		cpu->context->reg.a = 1;
	}
	else
	{
		cpu->context->reg.a = 0;
	}
}

void get_mass(c_cpu_t* cpu)
{
	cpu->context->reg.a = current_cell->mass;
}

void split(c_cpu_t* cpu)
{
	printf("Split\n");
	if(current_cell->mass > 5 && cells_get_count() != MAX_CELLS) // arbitrary threshold
	{
		int half_mass = current_cell->mass / 2;
		current_cell->mass -= half_mass;

		int x = current_cell->x + 1;
		if(x >= WORLD_WIDTH)
			x -= 2;

		cell_spawn(mutate(c_mem_copy(current_cell->process.context.memory)),
					current_cell->color, half_mass, x, current_cell->y);
	}
}


c_interrupt_handler_t handlers[] = {random, move, eat, get_mass, split};
static c_interrupt_vector_table_t ivt =
{
	handlers,
	sizeof(handlers) / sizeof(c_interrupt_handler_t)
};

c_interrupt_vector_table_t build_ivt()
{
	printf("IVT length: %lu\n", ivt.length);
	return ivt;
}

