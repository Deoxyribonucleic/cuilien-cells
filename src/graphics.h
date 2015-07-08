#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

typedef struct cell cell_t;
typedef struct world world_t;
typedef struct tile tile_t;

int graphics_init();
int graphics_update();
int graphics_destroy();

void graphics_render_cell(cell_t const* cell);
void graphics_init_world_image(world_t const* world);
void graphics_update_world_image(int x, int y, tile_t const* tile);

#endif
