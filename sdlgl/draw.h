#pragma once

#include "window.h"
#include "data_types.h"
#include "light.h"
#include <GL/glew.h>

/* Uniform indexes 
 * Make sure to update UNIFORMS in shader.c
 * */

enum {
	UN_MODEL_MATRIX = 0,
    UN_MODEL_VIEW_MATRIX,
    UN_MODEL_VIEW_PROJ_MATRIX,
	UN_CAMERA_MATRIX,
    UN_BONES,
    UN_NUM_BONES,
    UN_TIME,
    UN_TEXTURE0,
    UN_TEXTURE1,
    UN_TEXTURE2,
    UN_TEXTURE3,
    UN_POINT_LIGHTS,
    UN_POINT_LIGHTS_COUNT,
    UN_POINT_LIGHTS_COLOR,
	UN_LEN,
} uniform;

typedef struct {
	GLuint frame_buffer;
	GLuint tex_color_buffer;
	GLuint tex_depth_buffer;
} frame_buffer_t;

frame_buffer_t* frame_buffer_new(int w, int h);

typedef struct {
	mat4 camera;
	mat4 projection;
	light_t point_lights;
	mat4 _view; /* This is calculated during drawing */
	mat4 _view_proj; /* This is calculated during drawing */
	float time;
	frame_buffer_t* frame_buffer;
} scene_properties_t;

void scene_bind_frame_buffer(scene_properties_t* scene);
void scene_init(scene_properties_t* scene, SDLGL_Settings* settings);

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

/* Drawable object 
 * gobject_ptr:		gobject is referencing this dobject.
 * 					Used when removing gobjects.
 * 					NULL if no gobject is referencing.
 * */
typedef struct {
	mat4 model_mat;
	mat4* bones;
	void* gobject_ptr;
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

	/* Dynamic vector of dobjects */
	dobject_t* dobjects;
	int dobjects_size;
} render_state_t;

/* No need for a pointer becouse it's an array */
void gen_vbuff(vbuff_t);
void fill_buff(vbuff_t vbuff, unsigned int index, void* data, size_t size, GLuint shader);
void draw_render_states(render_state_t*, int num_r_states, scene_properties_t* scene_prop);
