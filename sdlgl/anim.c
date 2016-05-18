#include "anim.h"
#include <stdlib.h>

static void anim_read_stream(anim_t* anim, FILE* stream) {
	fread(&anim->num_bones, sizeof(anim->num_bones), 1, stream);
	fread(&anim->num_keyframes, sizeof(anim->num_keyframes), 1, stream);

	anim->times = malloc(sizeof(*anim->times) * anim->num_keyframes);
	anim->keyframes = malloc(sizeof(*anim->keyframes) * anim->num_keyframes * anim->num_bones);

	fread(anim->times, sizeof(*anim->times), anim->num_keyframes, stream);
	fread(anim->keyframes, sizeof(*anim->keyframes), anim->num_keyframes*anim->num_bones, stream);
}

anim_t* anim_new_from_file(const char* filename) {
	FILE* stream = fopen(filename, "rb");
	if (stream == NULL) {
		fprintf(stderr, "Can't open file: %s\n", filename);
	}
	anim_t* anim = malloc(sizeof(anim_t));
	anim_read_stream(anim, stream);
	fclose(stream);
	return anim;
}

void anim_from_file(anim_t* anim, const char* filename) {
	FILE* stream = fopen(filename, "rb");
	if (stream == NULL) {
		fprintf(stderr, "Can't open file: %s\n", filename);
	}
	anim_read_stream(anim, stream);
	fclose(stream);
}

void anim_delete(anim_t* anim) {
	free(anim->times);
	free(anim->keyframes);
}

void anim_eval(anim_t* anim, float time, mat4* bones) {
	int i, j, up;

	/* Find index of the upper keyframe
	 * Linear search (good for anims with small num_keyframes) */
	for (j = 0; j < anim->num_keyframes; ++j) {
		if (time < anim->times[j]) {
			break;
		}
	}
	up = j;
	float u = anim->times[up];
	float l = anim->times[up - 1];
	float x = (time - l) / (u - l);

	for (i = 0; i < anim->num_bones; ++i) {

		if (up == 0) {
			mat4cpy(bones[i], anim->keyframes[anim->num_keyframes * i]);
		} else if (up == anim->num_keyframes) {
			mat4cpy(bones[i], anim->keyframes[anim->num_keyframes * (i + 1) - 1]);
		} else {

			mat4slerp(
					anim->keyframes[anim->num_keyframes * i + up - 1],
					anim->keyframes[anim->num_keyframes * i + up],
					x,
					bones[i]);
		}

	}
}
