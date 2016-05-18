#include "model.h"
#include <stdio.h>

void model_from_vbb(model_t* model, const char* filename, GLuint shader) {
	gen_vbuff(model->vbuff);
	model->num_elems = load_vbb(model->vbuff, filename, shader, &model->num_bones);
}
