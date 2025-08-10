#include "tkpgl.h"
#include "tkpmem.h"

#ifndef NO_TKPGL_IMPLEMENTATION

static const char* default_vs_c = 
"#version 330 core\n"
"layout (location = 0) in vec2 in_pos;\n"
"layout (location = 1) in vec3 in_color;\n"
"out vec3 inter_color;\n"
"void main() {\n"
"	inter_color = in_color;\n"
"	gl_Position = vec4(in_pos, 0, 1);\n"
"}\n\0";

static const char* default_fs_c = 
"#version 330 core\n"
"in vec3 inter_color;\n"
"out vec4 out_color;\n"
"void main() {\n"
"	out_color = vec4(inter_color, 1);\n"
"}\n\0";

static u32 tkpgl_default_program = 0;
void tkpgl_create_default_shader() {
	u32 vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &default_vs_c, NULL);
    glCompileShader(vs);

	u32 fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &default_fs_c, NULL);
    glCompileShader(fs);

	tkpgl_default_program = glCreateProgram();
	glAttachShader(tkpgl_default_program, vs);
	glAttachShader(tkpgl_default_program, fs);
	glLinkProgram(tkpgl_default_program);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void tkpgl_bind_default_shader(tkpgl_context* ctx) {
	if (ctx->batches[ctx->batch_index].vertex_count != 0) {
		tkpgl_create_batch(ctx);
	}

	ctx->batches[ctx->batch_index].program = tkpgl_default_program;
	glUseProgram(tkpgl_default_program);
}

tkpgl_context* tkpgl_create_context() {
	tkpgl_context* ctx = TKPLIB_MALLOC(sizeof(tkpgl_context));
	ctx->batch_capacity = TKPGL_DEFAULT_BATCH_PREALLOC_SIZE;
	ctx->batch_index = 0;
	ctx->batch_count = 0;
	ctx->batches = TKPLIB_MALLOC(sizeof(tkpgl_batch) * TKPGL_DEFAULT_BATCH_PREALLOC_SIZE);
	TKPLIB_MEMSET(ctx->batches, 0, sizeof(tkpgl_batch) * TKPGL_DEFAULT_BATCH_PREALLOC_SIZE);
	
	glewInit();
	tkpgl_create_default_shader();
	tkpgl_bind_default_shader(ctx);
	tkpgl_create_batch(ctx);

	return ctx;
}

u16 tkpgl_create_batch(tkpgl_context* ctx) {
	if (ctx->batch_count+1 > ctx->batch_capacity) {
		ctx->batch_capacity *= 2;
		ctx->batches = TKPLIB_REALLOC(ctx->batches, sizeof(tkpgl_batch) * ctx->batch_capacity);
	}

	if (ctx->batches[ctx->batch_index+1].vertex_capacity != 0) {
		return ++ctx->batch_index;
	}

	tkpgl_batch* batch = &ctx->batches[ctx->batch_index];
	batch->program = tkpgl_default_program;
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(tkpgl_vertex2f), (void*)offsetof(tkpgl_vertex2f, r));
	glEnableVertexAttribArray(1);

	ctx->batch_index = ctx->batch_count;
	ctx->batch_count += 1;
	return ctx->batch_index; 
}

void tkpgl_batch_resize(tkpgl_batch* batch, u32 size) {
	batch->vertex_buffer = TKPLIB_REALLOC(batch->vertex_buffer, size);
	batch->vertex_capacity = size/sizeof(tkpgl_vertex2f);
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferData(GL_ARRAY_BUFFER, size, batch->vertex_buffer, GL_DYNAMIC_DRAW);
}

void tkpgl_clear_batches(tkpgl_context* ctx) {
	for (u16 i = 0; i < ctx->batch_count; ++i) {
		if (ctx->batches[i].vertex_count > 0) {
			TKPLIB_MEMSET(ctx->batches[i].vertex_buffer, 0, ctx->batches[i].vertex_count*sizeof(tkpgl_vertex2f));
			ctx->batches[i].vertex_count = 0;
		}
		ctx->batches[i].was_buffer_changed = 0;
	}
	ctx->batch_index = 0;
}

/* shapes */
void tkpgl_batch_triangle(tkpgl_context* ctx, tkpgl_vertex2f x1, tkpgl_vertex2f x2, tkpgl_vertex2f x3) {
	tkpgl_batch* batch = &ctx->batches[ctx->batch_index];

	if (batch->vertex_capacity < batch->vertex_count+3) {
		tkpgl_batch_resize(batch, batch->vertex_capacity*2*sizeof(tkpgl_vertex2f));
	}

	batch->was_buffer_changed = 1;

	batch->vertex_buffer[batch->vertex_count++] = x1;
	batch->vertex_buffer[batch->vertex_count++] = x2;
	batch->vertex_buffer[batch->vertex_count++] = x3;
}

void tkpgl_dispatch(tkpgl_context* ctx) {
	for (u16 i = 0; i < ctx->batch_count; ++i) {
		if (ctx->batches[i].was_buffer_changed) {
			glBindBuffer(GL_ARRAY_BUFFER, ctx->batches[i].vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(tkpgl_vertex2f)*ctx->batches[i].vertex_capacity, ctx->batches[i].vertex_buffer, GL_DYNAMIC_DRAW);
		}

		glUseProgram(ctx->batches[i].program);
		glBindVertexArray(ctx->batches[i].vao);
		glDrawArrays(GL_TRIANGLES, 0, ctx->batches[i].vertex_count);
	}
}

#endif
