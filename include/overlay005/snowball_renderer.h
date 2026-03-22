#ifndef POKEPLATINUM_SNOWBALL_RENDERER_H
#define POKEPLATINUM_SNOWBALL_RENDERER_H

#include "struct_decls/struct_02061AB4_decl.h"

#include "overlay005/field_effect_manager.h"

#include "overworld_anim_manager.h"

void *SnowballRenderer_New(FieldEffectManager *fieldEffMan);
void SnowballRenderer_Free(void *context);
OverworldAnimManager *SnowballRenderer_Init(MapObject *mapObj);
void SnowballRenderer_SetSnowballBroken(OverworldAnimManager *animMan);

#endif // POKEPLATINUM_SNOWBALL_RENDERER_H
