#include "../../src/tkpgl.h"
#include "../../src/tkpwin.h"

int main(void) {
	tkpwin_t* window = tkpwin_create("TKPLIB Triangle", 620, 480);
	
	tkpgl_context* ctx = tkpgl_create_context();

	tkpgl_clear_batches(ctx);

	for (int i = 0; i < 2048; ++i) {
		tkpgl_batch_triangle(ctx,
			(tkpgl_vertex2f) {
				.x = -0.5f,
				.y = -0.5f,
				.r = 1,
				.g = 0,
				.b = 0
			}, (tkpgl_vertex2f) {
				.x = 0.5f,
				.y = -0.5f,
				.r = 0,
				.g = 0,
				.b = 1
			}, (tkpgl_vertex2f) {
				.x = 0,
				.y = 0.5f,
				.r = 0,
				.g = 1,
				.b = 0
		});
	}
	while (!tkpwin_has_closed(window)) {
		tkpgl_dispatch(ctx);
		tkpwin_present(window);
	}

	tkpwin_close(window);
}
