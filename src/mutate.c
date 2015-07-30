#include "mutate.h"

#include "vm/page.h"
#include "vm/vector.h"
#include "vm/instr.h"
#include "vm/operand.h"

#include <stdlib.h>
#include <stdio.h>

#define ABSOLUTE_ADDRESS(b, page) (page->id * C_PAGE_SIZE + b)
#define INSTRUCTION_OFFSET(b, page) (ABSOLUTE_ADDRESS(b, page) % 12)

#define INSTRUCTION_START(b, page) ((ABSOLUTE_ADDRESS(b, page) / 12) * 12) % C_PAGE_SIZE
#define INSTRUCTION_START_OFFSET(b, page, offset) (INSTRUCTION_START(b, page) + offset)

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
			if(rand() % 500 < 1)
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
				// else if((page->id * C_PAGE_SIZE + b) % 12 == 2) /* flags op1 */
				// {
				// }
				// else if((page->id * C_PAGE_SIZE + b) % 12 == 3) /* flags op2 */
				// {
				// }
				else if((page->id * C_PAGE_SIZE + b) % 12 >= 4 && (page->id * C_PAGE_SIZE + b) % 12 <= 7) /* op 1 */
				{
					if(page->mem[INSTRUCTION_START_OFFSET(b, page, 2)] & C_PF_REGISTER)
					{
						if(INSTRUCTION_OFFSET(b, page) > 4)
							page->mem[b] = 0;
						else
							page->mem[b] = (rand() % 7) + 1;
					}
					if(page->mem[INSTRUCTION_START_OFFSET(b, page, 0)] == C_INSTR_INT) /* INT */
					{
						if(INSTRUCTION_OFFSET(b, page) > 4)
							page->mem[b] = 0;
						else
							page->mem[b] = (rand() % 6);
					}
					else
					{
						page->mem[b] = rand() % 255;
					}
				}
				else if((page->id * C_PAGE_SIZE + b) % 12 >= 8 && (page->id * C_PAGE_SIZE + b) % 12 <= 11) /* op 2 */
				{
					if(page->mem[INSTRUCTION_START_OFFSET(b, page, 3)] & C_PF_REGISTER)
					{
						if(INSTRUCTION_OFFSET(b, page) > 8)
							page->mem[b] = 0;
						else
							page->mem[b] = (rand() % 7) + 1;
					}
					else
					{
						page->mem[b] = rand() % 255;
					}
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

