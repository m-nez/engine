#include "load.h"
#include "draw.h"
#include <stdio.h>
#include <stdlib.h>

char* readfile(const char* fname) {
	FILE* in = fopen(fname, "rb");
	if (in == NULL) {
		fprintf(stderr, "Error: Could not open file: %s", fname);
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

unsigned int load_vbb(vbuff_t vbuff, const char* fname, GLuint shader) {
	FILE* in = fopen(fname, "rb");
	if (in == NULL) {
		fprintf(stderr, "Error: Could not open file: %s", fname);
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
