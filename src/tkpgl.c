#include "tkpgl.h"
#include "tkpmem.h"

#ifndef NO_TKPGL_IMPLEMENTATION

tkpgl_context* tkpgl_create_context() {
	tkpgl_context* ctx = TKPLIB_MALLOC(sizeof(tkpgl_context));
	ctx->batch_capacity = TKPGL_DEFAULT_BATCH_PREALLOC_SIZE;
	ctx->batches = TKPLIB_MALLOC(sizeof(tkpgl_batch) * TKPGL_DEFAULT_BATCH_PREALLOC_SIZE);
	
	glewInit();

	return ctx;
}

u16 tkpgl_create_batch(tkpgl_context* ctx) {
	if (ctx->batch_count+1 > ctx->batch_capacity) {
		ctx->batch_capacity *= 2;
		ctx->batches = TKPLIB_REALLOC(ctx->batches, sizeof(tkpgl_batch) * ctx->batch_capacity);
	}

	tkpgl_batch* batch = &ctx->batches[ctx->batch_count];
	batch->vertex_count = 0;
	batch->vertex_capacity = TKPGL_DEFAULT_VERTEX_PREALLOC_SIZE;

	glGenVertexArrays(1, &batch->vao);
	glGenBuffers(1, &batch->vbo);

	glBindVertexArray(batch->vao);
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);

	batch->vertex_buffer = TKPLIB_MALLOC(sizeof(tkpgl_vertex2f)*batch->vertex_capacity);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tkpgl_vertex2f)*batch->vertex_capacity, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(tkpgl_vertex2f), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(tkpgl_vertex2f), (void*)offsetof(tkpgl_vertex2f, r));
	glEnableVertexAttribArray(1);

	ctx->batch_count++;
	return ctx->batch_count-1; 
}

void tkpgl_batch_resize(tkpgl_batch* batch, u32 size) {
	batch->vertex_buffer = TKPLIB_REALLOC(batch->vertex_buffer, size);
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferData(GL_ARRAY_BUFFER, size, batch->vertex_buffer, GL_DYNAMIC_DRAW);
}

boolean tkpgl_should_create_new_batch(tkpgl_context* ctx) {
	if (ctx->state.program != ctx->batches[ctx->batch_count-1].program ||
		ctx->state.texture != ctx->batches[ctx->batch_count-1].texture) {
		return 1;
	}

	return 0;
}

void tkpgl_reallocate_batch_vertices(tkpgl_context* ctx, u8 batch, u32 count) {
	
	u32 vertex_capacity = ctx->batches[batch].vertex_capacity;
	while (ctx->batches[batch].vertex_count+count > ctx->batches[batch].vertex_capacity) {
		vertex_capacity *= 2;
	}

	if (vertex_capacity != ctx->batches[batch].vertex_capacity) {
		tkpgl_batch_resize(&ctx->batches[batch], count*sizeof(tkpgl_vertex2f));
	}
}

/* shapes */
void tkpgl_batch_triangle(tkpgl_context* ctx, tkpgl_vertex2f x1, tkpgl_vertex2f x2, tkpgl_vertex2f x3) {
	if (tkpgl_should_create_new_batch(ctx)) {
		tkpgl_create_batch(ctx);
	}
	tkpgl_batch* batch = &ctx->batches[ctx->batch_count-1];

	batch->vertex_buffer[batch->vertex_count++] = x1;
	batch->vertex_buffer[batch->vertex_count++] = x2;
	batch->vertex_buffer[batch->vertex_count++] = x3;
	
}

#endif
