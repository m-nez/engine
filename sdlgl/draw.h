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
#define UN_TIME 5
#define UN_TEXTURE0 6
#define UN_TEXTURE1 7
#define UN_TEXTURE2 8
#define UN_TEXTURE3 9

#define UN_LEN 10

typedef struct {
	GLuint frame_buffer;
	GLuint tex_color_buffer;
	GLuint tex_depth_buffer;
} frame_buffer_t;

frame_buffer_t* frame_buffer_new(int w, int h);

typedef struct {
	mat4 camera;
	mat4 projection;
	mat4 _view;
	mat4 _view_proj;
	float time;
	frame_buffer_t* frame_buffer;
} scene_properties_t;

void scene_bind_frame_buffer(scene_properties_t* scene);

/* Attribute names */
#define AT_COLOR "color"
#define AT_POSITION "vertex"
#define AT_NORMAL "normal"
#define AT_TEXCOORD "uv"
#define AT_TANGENT "tangent"
#define AT_BITANGENT "bitangent"
#define AT_BONE_WEIGHT "bone_weight"
#define AT_BONE_INDEX "bone_index"

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
#define VBF_BONE_INDEX	9

#define VBF_LEN			10

typedef GLuint vbuff_t[VBF_LEN];

/* Drawable object */
typedef struct {
	mat4 model_mat;
	mat4* bones;
} dobject_t;

typedef struct {
	GLuint vao;
	GLuint shader; /* Compiled vertex + fragment */
	GLuint textures[MAX_TEXTURES];
	unsigned int num_elem;
	unsigned int num_bones;
	int num_tex;
	int len;
	GLint* uniform_index;
} render_state_t;

/* No need for a pointer becouse it's an array */
void gen_vbuff(vbuff_t);
void fill_buff(vbuff_t vbuff, unsigned int index, void* data, size_t size, GLuint shader);
void draw_dobjects(dobject_t*, render_state_t*, int, scene_properties_t* scene_prop);
