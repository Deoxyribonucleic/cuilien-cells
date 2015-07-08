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
	switch(cpu->context->reg.a % 4)
	{
	case 0:
		++current_cell->x;
		break;
	case 1:
		++current_cell->y;
		break;
	case 2:
		--current_cell->x;
		break;
	case 3:
		--current_cell->y;
		break;
	default:
		break;
	}
}

void eat(c_cpu_t* cpu)
{
}

void get_mass(c_cpu_t* cpu)
{
	cpu->context->reg.a = current_cell->mass;
}

void split(c_cpu_t* cpu)
{
	printf("split\n");
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

