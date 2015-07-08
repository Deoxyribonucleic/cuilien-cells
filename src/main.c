#include <stdio.h>

#include "vm/mem.h"
#include "vm/error.h"
#include "vm/cpu.h"
#include "vm/vector.h"
#include "vm/scheduler.h"
#include "vm/process.h"
#include "vm/debug.h"
#include "vm/interrupt.h"

#include "graphics.h"
#include "cell.h"
#include "world.h"
#include "interaction.h"

#include <stdlib.h>
#include <string.h>


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
	world_load(&world, "terrain.bmp");
	graphics_init_world_image(world);

	printf("Spawning test cell.\n");
	cell_t test_cell;
	cell_create(&test_cell, seed, 0x00ff00, 5, 100, 100);
	cpu->context = &test_cell.process.context;
	current_cell = &test_cell;
	
	while(!graphics_update())
	{
		c_cpu_step(cpu);
		
		graphics_render_cell(&test_cell);
	}
	
	cell_destroy(&test_cell);
	
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

