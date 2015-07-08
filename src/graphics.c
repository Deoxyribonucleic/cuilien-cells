#include <SDL/SDL.h>

static struct
{
	SDL_Surface* screen;
} graphics;

int graphics_init()
{
	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("SDL Test", "SDL Test");

	/* create window */
	graphics.screen = SDL_SetVideoMode(640, 480, 0, 0);

	return 0;
}

int graphics_update()
{
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

	/* update the screen */
	SDL_UpdateRect(graphics.screen, 0, 0, 0, 0);
	
	return 0;
}

int graphics_destroy()
{
	/* cleanup SDL */
	SDL_Quit();

	return 0;
}

