#include <stdio.h>

#include "vm/mem.h"
#include "vm/error.h"
#include "vm/cpu.h"
#include "vm/vector.h"
#include "vm/scheduler.h"
#include "vm/process.h"
#include "vm/debug.h"
#include "vm/instr.h"
#include "vm/interrupt.h"

#include "graphics.h"
#include "cell.h"
#include "world.h"
#include "interaction.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


cell_t* current_cell = NULL;
world_t* world = NULL;


int main(int argc, char** args)
{
	int error;
	c_cpu_handle cpu;
	
	printf("Cuilien cell simulation.\n");

	printf("Initializing VM...\n");
	cpu = c_cpu_init();
	if(c_error_last)
	{
		c_error_print(c_error_last);
		return 1;
	}
	
	printf("Disabling a few opcodes...\n");
	c_instruction_vector[C_INSTR_PUTC] = NULL;
	c_instruction_vector[C_INSTR_GETC] = NULL;
	c_instruction_vector[C_INSTR_SHOW] = NULL;

	printf("Initializing IVT...\n");
	cpu->ivt = build_ivt();

	printf("Loading seed...\n");
	c_mem_handle seed = c_mem_init(MAX_CELL_MEMORY);
	c_mem_load_file(seed, "seed.cx", 12);

	printf("Setting up graphics...\n");
	if((error = graphics_init()))
	{
		// TODO: handle error
		return 1;
	}
	
	printf("Creating world...\n");
	world_load(&world, "abundance.bmp");
	graphics_init_world_image(world);

	printf("Spawning inital cells...\n");
	cells_init();

	cell_spawn(c_mem_copy(seed), 0x00ff00, 5, 180, 200);
	cell_spawn(c_mem_copy(seed), 0xff0000, 5, 1000, 200);
	cell_spawn(c_mem_copy(seed), 0x00ffff, 5, 180, 600);
	cell_spawn(c_mem_copy(seed), 0xffff00, 5, 1000, 600);
	
	printf("Ready.\n");
	
	while(!graphics_update())
	{
		// simulate and render cells
		/* this turned out messier than I had envisioned it */
		int i = 0;
		while((current_cell = cell_next(&i)))
		{
			cpu->context = &current_cell->process.context;

			assert(cpu->context != NULL);
			assert(cpu->context->memory != NULL);

			int step;
			for(step = 0; step < 100; ++step)
			{
				c_cpu_step(cpu);
			}
			
			// mass decay
			--current_cell->mass;

			graphics_render_cell(current_cell);

			if(!current_cell->alive)
				cell_kill(current_cell); // how do you kill... that which has no life?
		}
	}
	
	printf("Freeing memory...\n");
	cells_free();
	c_mem_free(seed);
	
	printf("Destroying graphics...\n");
	if((error = graphics_destroy()))
	{
		// TODO: handle error
		return 1;
	}

	printf("Freeing VM...\n");
	c_cpu_free(cpu);
	
	printf("Good bye!\n");
}

