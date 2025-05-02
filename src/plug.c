#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <raylib.h>

#include "plug.h"

typedef struct {
	Color background;
} Plug;

static Plug *e = NULL;

void plug_init(void)
{
	e = malloc(sizeof(*e));

	assert(e != NULL && "Buy more RAM lol");

	memset(e, 0, sizeof(*e));

	e->background = (Color) {215, 24, 24, 225};
}

void plug_free(void)
{
	free(e);
}

void plug_frame(void)
{
	ClearBackground(e->background);
	BeginDrawing();
	EndDrawing();
}

void *plug_pre_reload(void)
{
	return e;
}

void plug_post_reload(void *ee)
{
	e = ee;
}
