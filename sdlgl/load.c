#include "load.h"
#include "draw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_image.h>

char* readfile(const char* fname) {
	FILE* in = fopen(fname, "rb");
	if (in == NULL) {
		fprintf(stderr, "Error: Could not open file: %s\n", fname);
		return NULL;
	}
	fseek(in, 0, SEEK_END);
	size_t fsize = ftell(in);
	fseek(in, 0, SEEK_SET);
	char* buf = malloc(fsize + 1);
	fread(buf, fsize, 1, in);
	buf[fsize] = '\0';
	fclose(in);
	return buf;
}

/* Return 0 if compiled successfully,
 * otherwise return 1 and print an error message to stderr.
 */
static int check_shader(GLuint shader,const char* header) {
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE) {
		fprintf(stderr, "%s\n", header);
		GLint len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		char* error_log = malloc(len);
		glGetShaderInfoLog(shader, len, &len, error_log);
		fprintf(stderr, "%s", error_log);

		free(error_log);
		return 1;
	}
	return 0;
}

GLuint load_shader(const char* vert, const char* frag) {
	char* source;
	// Create and compile the vertex shader
	source = readfile(vert);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char**) &source, NULL);
    glCompileShader(vertexShader);

	check_shader(vertexShader, vert);
	free(source);

    // Create and compile the fragment shader
	source = readfile(frag);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char**) &source, NULL);
    glCompileShader(fragmentShader);

	check_shader(fragmentShader, frag);
	free(source);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, OUT_FRAG);
    glLinkProgram(shaderProgram);

	return shaderProgram;
}

unsigned int load_vbb(vbuff_t vbuff,
		const char* fname,
		GLuint shader,
		unsigned int* num_bones) {
	FILE* in = fopen(fname, "rb");
	if (in == NULL) {
		fprintf(stderr, "Error: Could not open file: %s\n", fname);
	}
	
	unsigned int n;
	unsigned int count; /* 4 Verts each has X, Y, Z. count = 4 * 3 */
	unsigned int index;
	size_t size;
	void* data = NULL;
	unsigned int num_elem;
	int i;

    glBindVertexArray(vbuff[VBF_VAO]);
	fread(&n, sizeof(unsigned int), 1, in);
	fread(num_bones, sizeof(unsigned int), 1, in);
	for (i = 0; i < n; ++i) {
		fread(&count, sizeof(unsigned int), 1, in);
		fread(&index, sizeof(unsigned int), 1, in);
		/* Grab number of elements */
		if (index == VBF_EBO)
			num_elem = count;
		/* Only possible data types are unsigned int and float
		 * both have a size of 4 bytes */
		size = 4 * count;
		data = realloc(data, size); 
		fread(data, size, 1, in);
		/* Attach data to shader */
		fill_buff(vbuff, index, data, size, shader);
	}
	free(data);

	return num_elem;
}

static void invertY(char* data, int w, int h) {
	int i;
	char* tmp = malloc(w);
	for (i = 0; i < h/2; ++i) {
		memcpy(tmp, data + i*w, w);
		memcpy(data + i*w, data + (h - i - 1) * w, w);
		memcpy(data + (h - i - 1) * w, tmp, w);
	}
	free(tmp);
}

GLuint load_texture(const char* fname) {
	SDL_Surface* surf = IMG_Load(fname);
	/* Invert RGBA image */
	invertY(surf->pixels, surf->w*surf->format->BytesPerPixel, surf->h);
	GLenum format;
	if (SDL_ISPIXELFORMAT_ALPHA(surf->format->format)) {
		format = GL_RGBA;
	} else {
		format = GL_RGB;
	}
	
	GLuint texture;
	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, surf->w, surf->h, 0, format, GL_UNSIGNED_BYTE, surf->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(surf);

	return texture;
}
