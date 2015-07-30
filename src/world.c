#include "world.h"

#include "SDL/SDL.h"

#include "graphics.h"

#define min(a, b) (a > b ? b : a)

#define WASTE_TO_FOOD_INTERVAL 10
#define WASTE_TO_FOOD_PER_TICK 1


int world_load(world_t** world, char const* filename)
{
	*world = malloc(sizeof(world_t));
	memset((*world)->grid, 127, sizeof(tile_t) * WORLD_WIDTH * WORLD_HEIGHT);

	// Load terrain (food, etc) from a bitmap.
	// SDL is helping us here. :)
	// It could be considered stupid that we load this bitmap,
	// convert it to our own structure, and then convert it back
	// to a bitmap to render it, but oh well...

	SDL_Surface* tmp = SDL_LoadBMP(filename);
	SDL_Surface* surface = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	
	int x,y;
	for(y = 0; y < surface->h; ++y)
		for(x = 0; x < surface->w; ++x)
		{
			world_get_tile(*world, x, y)->food =
				((uint8_t*)surface->pixels)[(y * surface->w + x) * 4];
			world_get_tile(*world, x, y)->waste = 0;
			world_get_tile(*world, x, y)->cell = NULL;
		}

	SDL_FreeSurface(surface);
	return 0;
}

tile_t* world_get_tile(world_t* world, int x, int y)
{
	return &world->grid[y * WORLD_WIDTH + x];
}

tile_t const* world_get_tile_const(world_t const* world, int x, int y)
{
	return &world->grid[y * WORLD_WIDTH + x];
}

int world_remove_food(world_t* world, int x, int y)
{
	tile_t* tile = &world->grid[y * WORLD_WIDTH + x];
	if(tile->food)
	{
		graphics_update_world_image(x, y, tile);
		return tile->food--;
	}
	else
	{
		return 0;
	}
}

void world_update_waste(world_t* world)
{
	int x,y;
	for(y=0; y<WORLD_HEIGHT; ++y)
	{
		for(x=0; x<WORLD_WIDTH; ++x)
		{
			tile_t* tile = &world->grid[y * WORLD_WIDTH + x];

			if(tile->waste && ++tile->waste_decay_counter == WASTE_TO_FOOD_INTERVAL)
				tile->waste_decay_counter = 0;
			else
				continue;

			int waste = min(WASTE_TO_FOOD_PER_TICK, tile->waste);
			if(tile->food + waste <= 255)
			{
				tile->waste -= waste;
				tile->food += waste;
				graphics_update_world_image(x, y, tile);
			}
		}
	}
}

