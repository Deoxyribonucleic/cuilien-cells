#include <SDL/SDL.h>

#include "cell.h"
#include "world.h"

#include <math.h>
#include <stdlib.h>

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 900

#define GRAY(value) (value | (value << 8) | (value << 16))
#define RGB(r, g, b) (b | (g << 8) | (r << 16))

static struct
{
	SDL_Surface* screen;
	SDL_Surface* terrain;
} graphics;

int graphics_init()
{
	graphics.terrain = NULL;

	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("SDL Test", "SDL Test");

	/* create window */
	graphics.screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	return 0;
}

int graphics_update()
{
	/* update the screen */
	SDL_UpdateRect(graphics.screen, 0, 0, 0, 0);

	SDL_Event event;

	/* look for an event */
	if (SDL_PollEvent(&event)) {
		/* an event was found */
		switch (event.type) {
			/* close button clicked */
			case SDL_QUIT:
				return 1;
				break;

				/* handle the keyboard */
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						return 1;
						break;
				}
				break;
		}
	}

	// render terrain
	SDL_BlitSurface(graphics.terrain, NULL, graphics.screen, NULL);
	
	return 0;
}

int graphics_destroy()
{
	if(graphics.terrain)
		SDL_FreeSurface(graphics.terrain);

	/* cleanup SDL */
	SDL_Quit();

	return 0;
}

void graphics_render_cell(cell_t const* cell)
{
	((uint32_t*)graphics.screen->pixels)[cell->y * graphics.screen->w + cell->x] = cell->color;
	if(cell->x != graphics.screen->w - 1 && cell->y != graphics.screen->h - 1)
	{
		((uint32_t*)graphics.screen->pixels)[cell->y * graphics.screen->w + cell->x + 1] = cell->color;
		((uint32_t*)graphics.screen->pixels)[(cell->y+1) * graphics.screen->w + cell->x] = cell->color;
		((uint32_t*)graphics.screen->pixels)[(cell->y+1) * graphics.screen->w + cell->x + 1] = cell->color;
	}
}

void graphics_update_world_image(int x, int y, tile_t const* tile)
{
	uint32_t color;
	color = RGB(
			min(0xff, tile->food * 3 + ((int)((float)0x50 * ((float)tile->waste / 255.0f) * 20.0))),
			min(0xff, tile->food * 3 + ((int)((float)0x50 * ((float)tile->waste / 255.0f) * 8.0))),
			min(0xff, tile->food * 3));
	((uint32_t*)graphics.terrain->pixels)[y * WORLD_WIDTH + x] = color;
}

void graphics_init_world_image(world_t const* world)
{
	graphics.terrain = SDL_CreateRGBSurface(0, WORLD_WIDTH, WORLD_HEIGHT, 32, 0, 0, 0, 0);

	int x,y;
	for(y = 0; y<WORLD_HEIGHT; ++y)
		for(x = 0; x<WORLD_WIDTH; ++x)
		{
			graphics_update_world_image(x, y, world_get_tile_const(world, x, y));
		}
}

