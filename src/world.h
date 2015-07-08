#ifndef _WORLD_H_
#define _WORLD_H_

#include <stdint.h>

#define WORLD_WIDTH 640
#define WORLD_HEIGHT 480


typedef struct tile
{
	uint8_t food;
} tile_t;

typedef struct world
{
	tile_t grid[WORLD_WIDTH * WORLD_HEIGHT];
} world_t;

int world_load(world_t** world, char const* filename);
tile_t* world_get_tile(world_t* world, int x, int y);
tile_t const* world_get_tile_const(world_t const* world, int x, int y);

#endif
