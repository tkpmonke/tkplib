#pragma once

#include "tkpint.h"

#include <stdint.h>

#if !defined(TKPGL_NO_GLEW)
#include "external/glew/include/GL/glew.h"
#endif

/* how many batches are preallocated before needing to be reallocated */
#if !defined(TKPGL_DEFAULT_BATCH_PREALLOC_SIZE)
	#define TKPGL_DEFAULT_BATCH_PREALLOC_SIZE 8
#endif

#if !defined(TKPGL_DEFAULT_VERTEX_PREALLOC_SIZE)
	#define TKPGL_DEFAULT_VERTEX_PREALLOC_SIZE 6
#endif

typedef struct {
	float x;
	float y;
	u8 r;
	u8 g;
	u8 b;
	u8 a;
} tkpgl_vertex2f;

typedef struct {
	u32 program;
	u32 texture;
	u32 vao;
	u32 vbo;

	enum {
		tkpgl_points,
		tkpgl_triangles,
		tkpgl_quads
	} mode;

	u32 vertex_count;
	u32 vertex_capacity;
	tkpgl_vertex2f* vertex_buffer;
} tkpgl_batch;

typedef struct {
	u16 batch_count;
	u16 batch_capacity;
	tkpgl_batch* batches;

	struct {
		u32 program;
		u32 texture;
	} state;
} tkpgl_context;

tkpgl_context* tkpgl_create_context();

/* created automatically by tkpgl_batch_* functions */
u16 tkpgl_create_batch(tkpgl_context* ctx);

/* general batch utilites */
void tkpgl_batch_resize(tkpgl_batch* batch, u32 size);
void tkpgl_clear_batches(tkpgl_context* ctx);

/* shapes */
void tkpgl_batch_points(tkpgl_context* ctx, tkpgl_vertex2f* points, u32 count);
void tkpgl_batch_triangle(tkpgl_context* ctx, tkpgl_vertex2f x1, tkpgl_vertex2f x2, tkpgl_vertex2f x3);

/* rendering */
void tkpgl_begin(tkpgl_context* ctx);
void tkpgl_dispatch(tkpgl_context* ctx);
