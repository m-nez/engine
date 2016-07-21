#include "draw.h"
#include <stdio.h>
#include <stdlib.h>
#include "data_types.h"

attr_t ATTRS[] = {
	{"",	0,	0},
	{"",	0,	0},
	{AT_POSITION,	3,	GL_FLOAT},
	{AT_NORMAL,		3,	GL_FLOAT},
	{AT_TEXCOORD,	2,	GL_FLOAT},
	{AT_COLOR,		4,	GL_FLOAT},
	{AT_TANGENT,	3,	GL_FLOAT},
	{AT_BITANGENT,	3,	GL_FLOAT},
	{AT_BONE_WEIGHT,4,	GL_FLOAT},
	{AT_BONE_INDEX,	4,	GL_FLOAT}, /* After using int() it should be the correct index.
									  In glsl there are only float vectors */
};

frame_buffer_t* frame_buffer_new(int w, int h) {
	frame_buffer_t* fb = malloc(sizeof(frame_buffer_t));
	glGenFramebuffers(1, &fb->frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fb->frame_buffer);

	glGenTextures(1, &fb->tex_color_buffer);
	glBindTexture(GL_TEXTURE_2D, fb->tex_color_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &fb->tex_depth_buffer);
	glBindTexture(GL_TEXTURE_2D, fb->tex_depth_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->tex_color_buffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb->tex_depth_buffer, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return fb;
}

static inline void calc_scene_prop(scene_properties_t* scene) {
	mat4invert(scene->camera, scene->_view);
	mat4mul(scene->projection, scene->_view, scene->_view_proj);
}

void scene_bind_frame_buffer(scene_properties_t* scene) {
	if (scene->frame_buffer != 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, scene->frame_buffer->frame_buffer);
	}
}

void scene_init(scene_properties_t* scene, SDLGL_Settings* settings) {
	light_init(&scene->point_lights);
	scene->time = 0;
	mat4identity(scene->camera);
	mat4projection(
			scene->projection,
			settings->width, settings->height,
			1.0, 1000.0);
	scene->frame_buffer = frame_buffer_new(settings->width, settings->height);
}

static inline void change_render_state(render_state_t rs, scene_properties_t* scene) {
	int i;
    glBindVertexArray(rs.vao);
    glUseProgram(rs.shader);

	glUniform1f(rs.uniform_index[UN_TIME], scene->time);
	glUniformMatrix4fv(
			rs.uniform_index[UN_POINT_LIGHTS],
			scene->point_lights.len, GL_FALSE,
			(float*)scene->point_lights.pos);
	glUniform1i(rs.uniform_index[UN_POINT_LIGHTS_COUNT], scene->point_lights.len);

	for (i = 0; i < rs.num_tex; ++i) {
		if (rs.textures[i] != 0) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, rs.textures[i]);
			glUniform1i( rs.uniform_index[UN_TEXTURE0 + i], i);
		}
	}
}


static inline void render(render_state_t rs, dobject_t dobject, scene_properties_t* scene) {
	mat4 mvp_mat;
	mat4mul(scene->_view_proj, dobject.model_mat, mvp_mat);
	glUniformMatrix4fv(
			rs.uniform_index[UN_MODEL_MATRIX],
			1, GL_FALSE, dobject.model_mat);
	glUniformMatrix4fv(
			rs.uniform_index[UN_MODEL_VIEW_PROJ_MATRIX],
			1, GL_FALSE, mvp_mat);
	glUniformMatrix4fv(rs.uniform_index[UN_BONES],
			rs.num_bones, GL_FALSE, (float*)dobject.bones);
	glUniform1f(rs.uniform_index[UN_TIME], scene->time);
	glUniform1ui(rs.uniform_index[UN_NUM_BONES], rs.num_bones);
}

void draw_render_states(render_state_t* r_states, int num_r_states, scene_properties_t* scene_prop) {
	int i, j;
	calc_scene_prop(scene_prop);

	for (i = 0; i < num_r_states; ++i) {
		change_render_state(r_states[i], scene_prop);
		for (j = 0; j < r_states[i].len; ++j) {
			render(r_states[i], r_states[i].dobjects[j], scene_prop);
			glDrawElements(GL_TRIANGLES, r_states[i].num_elem, GL_UNSIGNED_INT, NULL);
		}
	}
}

void gen_vbuff(vbuff_t vb) {
    glGenVertexArrays(1, &vb[0]);
    glGenBuffers(VBF_LEN-1, &vb[1]);
}

void fill_buff(vbuff_t vbuff, unsigned int index, void* data, size_t size, GLuint shader) {
	if (index == VBF_EBO) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbuff[index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, vbuff[index]);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(
				glGetAttribLocation(shader, ATTRS[index].name));
		glVertexAttribPointer(
				glGetAttribLocation(shader, ATTRS[index].name),
				ATTRS[index].size,
				ATTRS[index].type, GL_FALSE,
				ATTRS[index].size * 4, 0);
				/* size of GLfloat and GLuint is 4 */
	}
}
