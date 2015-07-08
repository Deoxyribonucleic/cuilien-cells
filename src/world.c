#include "world.h"

#include "SDL/SDL.h"


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
		return tile->food--;
	}
	else
	{
		return 0;
	}
}

