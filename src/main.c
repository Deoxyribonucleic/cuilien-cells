#include <stdio.h>

#include "vm/vm.h"
#include "vm/args.h"
#include "vm/mem.h"
#include "vm/error.h"
#include "vm/cpu.h"
#include "vm/vector.h"
#include "vm/scheduler.h"
#include "vm/process.h"
#include "vm/debug.h"
#include "vm/interrupt.h"

#include "graphics.h"

#include <stdlib.h>
#include <string.h>


int main(int argc, char** args)
{
	int error;
	
	printf("Cuilien cell simulation.\n");

	printf("Setting up graphics...\n");
	if((error = graphics_init()))
	{
		// TODO: handle error
	}
	
	while(!graphics_update())
	{
	}
	
	printf("Destroying graphics...\n");
	if((error = graphics_destroy()))
	{
		// TODO: handle error
	}
	
	printf("Good bye!\n");
}
