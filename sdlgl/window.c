#include "window.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include <stdio.h>

void SDLGL_Init(SDLGL_Settings* s, SDL_Window** w, SDL_GLContext* c) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, s->gl_context_major_version);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, s->gl_context_minor_version);
    
	*w = SDL_CreateWindow( s->title, s->x, s->y, s->width, s->height, s->flags); 
	printf("%p\n", *w);

	c = malloc(sizeof(SDL_GLContext));
	*c = SDL_GL_CreateContext(*w);
    glewExperimental = GL_TRUE;
    glewInit();
	Error("Window Initialized(ignore GL_INVALID_ENUM):");
}

SDLGL_Settings* SDLGL_SettingsDefault() {
	SDLGL_Settings* s = malloc(sizeof(SDLGL_Settings));
	strcpy(s->title, "SDLGL");
	s->x = 0;
	s->y = 0;
	s->width = 800;
	s->height = 600;
	s->gl_context_major_version = 3;
	s->gl_context_minor_version = 3;
	s->flags = SDL_WINDOW_OPENGL;
	return s;
}
