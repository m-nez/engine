#include "debug.h"
#include <stdio.h>
#include <string.h>

void Error(const char* text) {
	GLenum error = glGetError();
	char type[256];
	switch (error) {
		case GL_NO_ERROR:
			strcpy(type, "GL_NO_ERROR");
			break;
		case GL_INVALID_ENUM:
			strcpy(type, "GL_INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:
			strcpy(type, "GL_INVALID_VALUE");
			break;
		case GL_INVALID_OPERATION:
			strcpy(type, "GL_INVALID_OPERATION");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			strcpy(type, "GL_INVALID_FRAMEBUFFER_OPERATION");
			break;
		case GL_OUT_OF_MEMORY:
			strcpy(type, "GL_OUT_OF_MEMORY");
			break;

	}
	printf("%s %s\n", text, type);
}

