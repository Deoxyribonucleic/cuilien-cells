#include "interaction.h"
#include "cell.h"
#include "world.h"

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
	cpu->context->reg.a = cell_move(current_cell, cpu->context->reg.a % 4);
}

void eat(c_cpu_t* cpu)
{
	int food;
	if((food = world_remove_food(world, current_cell->x, current_cell->y)))
	{
		if(current_cell->mass < 255)
			++current_cell->mass;
		cpu->context->reg.a = food + 1;
	}
	else
	{
		cpu->context->reg.a = 0;
		if(current_cell->mass > 0)
			--current_cell->mass;
		//cell_kill(current_cell); // punish harder
	}
}

void get_mass(c_cpu_t* cpu)
{
	cpu->context->reg.a = current_cell->mass;
}

void split(c_cpu_t* cpu)
{
	cpu->context->reg.a = cell_split(current_cell);
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

