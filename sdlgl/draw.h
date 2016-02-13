#pragma once

#include "window.h"
#include "data_types.h"
#include <GL/glew.h>

/* Uniform indexes */
#define UN_MODEL_MATRIX 0
#define UN_MODEL_VIEW_MATRIX 1
#define UN_MODEL_VIEW_PROJ_MATRIX 2
#define UN_BONES 3
#define UN_NUM_BONES 4

#define UN_LEN 5

/* Attribute names */
#define AT_COLOR "color"
#define AT_POSITION "vertex"
#define AT_NORMAL "normal"
#define AT_TEXCOORD "uv"
#define AT_TANGENT "tangent"
#define AT_BITANGENT "bitangent"
#define AT_BONE_WEIGHT "bone_weight"

typedef struct {
	char* name;
	int size;
	GLenum type;
} attr_t;

/* Frag data */
#define OUT_FRAG "outColor"

#define MAX_TEXTURES 4 /* Max textures per object */

/* Indexes in vbuff_t */
#define VBF_VAO			0
#define VBF_EBO			1
#define VBF_VERTEX		2
#define VBF_NORMAL		3
#define VBF_UV			4
#define VBF_COLOR		5
#define VBF_TANGENT		6
#define VBF_BITANGENT	7
#define VBF_BONE_WEIGHT	8

#define VBF_LEN			9

typedef GLuint vbuff_t[VBF_LEN];

/* Drawable object */
typedef struct {
	mat4 model_mat;
} dobject_t;

typedef struct {
	GLuint vao;
	GLuint shader;//Compiled vertex + fragment
	GLuint textures[MAX_TEXTURES];
	unsigned int num_elem;
	int num_tex;
	int len;
	GLint* uniform_index;
} render_state_t;

/* No need for a pointer becouse it's an array */
void gen_vbuff(vbuff_t);
void fill_buff(vbuff_t vbuff, unsigned int index, void* data, size_t size, GLuint shader);
void draw_dobjects(dobject_t*, render_state_t*, int);
