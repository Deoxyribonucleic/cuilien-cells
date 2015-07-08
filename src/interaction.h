#ifndef _INTERACTION_H_
#define _INTERACTION_H_

#include "vm/interrupt.h"

typedef struct c_cpu c_cpu_t;

c_interrupt_vector_table_t build_ivt();

#endif
