#include "../../src/tkpgl.h"
#include "../../src/tkpwin.h"

int main(void) {
	tkpwin_t* window = tkpwin_create("TKPLIB Triangle", 620, 480);
	
	tkpgl_context* ctx = tkpgl_create_context();

	while (!tkpwin_has_closed(window)) {
		tkpgl_clear_batches(ctx);
		tkpgl_begin(ctx);

		tkpgl_batch_triangle(ctx,
			(tkpgl_vertex2f) {
				.x = -0.5f,
				.y = -0.5f,
				.r = 255,
				.g = 0,
				.b = 0,
				.a = 255
			}, (tkpgl_vertex2f) {
				.x = 0.5f,
				.y = -0.5f,
				.r = 0,
				.g = 0,
				.b = 255,
				.a = 255
			}, (tkpgl_vertex2f) {
				.x = 0,
				.y = 0.5f,
				.r = 0,
				.g = 255,
				.b = 0,
				.a = 255
		});

		tkpgl_dispatch(ctx);
		tkpwin_present(window);
	}

	tkpwin_close(window);
}
