#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_opengl.h"

typedef struct {
	char title[256];
	int x;
	int y;
	int width;
	int height;
	int gl_context_major_version;
	int gl_context_minor_version;
	int flags;
} SDLGL_Settings;

SDLGL_Settings* SDLGL_SettingsDefault();
void SDLGL_Init(SDLGL_Settings*, SDL_Window**, SDL_GLContext*);
