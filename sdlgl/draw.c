#include "draw.h"
#include <stdio.h>
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
	{AT_BONE_WEIGHT,1,	GL_UNSIGNED_INT},
};

static inline void change_render_state(render_state_t rs) {
	int i;
    glBindVertexArray(rs.vao);
    glUseProgram(rs.shader);

	/* Set uniforms 
	 * TODO Expose uniform tab for easier(not hard coded in this function)
	 * setting.*/
	mat4 mm = {{1}};
	glUniformMatrix4fv(rs.uniform_index[UN_MODEL_MATRIX], 1, GL_FALSE, (GLfloat*) mm);

	for (i = 0; i < rs.num_tex; ++i) {
		if (rs.textures[i] != 0) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, rs.textures[i]);
		}
	}
}

static inline void render(dobject_t dobject) {

}

void draw_dobjects(dobject_t* dobjects, render_state_t* r_states, int num_r_states) {
	int i, j;
	for (i = 0; i < num_r_states; ++i) {
		change_render_state(r_states[i]);
		for (j = 0; j < r_states[i].len; ++j) {
			render(dobjects[i+j]);
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
