#pragma once
#include "draw.h"
/* render_state_t should be evetualy moved to this file */

void dobject_init(dobject_t* dobject, render_state_t* rs);

dobject_t* render_state_add(render_state_t* rs);
int render_state_remove_at(render_state_t* rs, int index);
