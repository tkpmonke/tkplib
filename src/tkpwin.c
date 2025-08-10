#include "tkpwin.h"

#if TKPWIN_BACKEND == RGFW

#define RGFW_INT_DEFINED
#define RGFW_IMPLEMENTATION
#define RGFW_OPENGL
#include "external/RGFW.h"

tkpwin_t* tkpwin_create(const char* name, u32 w, u32 h) {
	RGFW_window* window = RGFW_createWindow(name, RGFW_RECT(0, 0, w, h), RGFW_windowCenter);
	RGFW_window_makeCurrent_OpenGL(window);

	return window;
}

void tkpwin_close(tkpwin_t* window) {
	RGFW_window_close(window);
}

boolean tkpwin_has_closed(tkpwin_t* window) {
	while (RGFW_window_checkEvent(window)) {
		if (((RGFW_window*)window)->event.type == RGFW_quit) {
			return 1;
		}
	}

	return RGFW_window_shouldClose(window);
}

void tkpwin_present(tkpwin_t* window) {
	RGFW_window_swapBuffers_OpenGL(window);
}

boolean tkpwin_pressed(tkpwin_t* window, u8 key) {
	return RGFW_wasPressed(window, key);
}

#endif
