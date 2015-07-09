#include "mutate.h"

#include "vm/page.h"
#include "vm/vector.h"
#include "vm/instr.h"

#include <stdlib.h>


c_mem_handle mutate(c_mem_handle dna)
{
	// only mutate mapped pages
	int i;
	for(i = 0; i < dna->page_table.size; ++i)
	{
		c_page_t* page = (c_page_t*)c_vector_resolve(&dna->page_table, i);

		int b;
		for(b = 0; b < C_PAGE_SIZE; ++b)
		{
			if(rand() % 100 < 5)
			{
				/* BEWARE! Little-endian trickery ahead */
				if((page->id * C_PAGE_SIZE + b) % 12 == 1); /* don't mess with second byte of opcode */
				else if((page->id * C_PAGE_SIZE + b) % 12 == 0)
				{
					/* if this byte is part of an opcode, only mutate it to a valid opcode */
					c_short instruction;
					
					do
					{
						instruction = rand() % C_INSTRUCTION_VECTOR_LENGTH;
					}
					while(c_instruction_vector[instruction] == NULL);

					page->mem[b] = instruction;
				}
				else
				{
					page->mem[b] = rand() % 255;
				}
			}
		}
	}

	return dna;
}

