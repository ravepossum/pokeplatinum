#include "overlay005/ov5_021F23D0.h"

#include <nitro.h>
#include <string.h>

#include "constants/field/field_effect_renderer.h"

#include "struct_decls/struct_02061AB4_decl.h"

#include "overlay005/field_effect_manager.h"

#include "map_object.h"
#include "overworld_anim_manager.h"
#include "simple3d.h"

typedef struct SnowballResources {
    FieldEffectManager *fieldEffMan;
    Simple3DModel model;
    Simple3DAnimation anim;
} SnowballResources;

typedef struct SnowballUserData {
    FieldEffectManager *fieldEffMan;
    SnowballResources *resources;
    MapObject *mapObj;
} SnowballUserData;

typedef struct {
    int unused;
    int localID;
    int mapID;
    BOOL snowballBroken;
    SnowballUserData userData;
    Simple3DAnimation anim;
    Simple3DRenderObj renderObj;
} SnowballRenderer;

static void SnowballResources_Init(SnowballResources *param0);
static void SnowballResources_Free(SnowballResources *param0);

static const OverworldAnimManagerFuncs sSnowballRendererAnimFuncs;

void *SnowballRenderer_New(FieldEffectManager *fieldEffMan)
{
    SnowballResources *resources = FieldEffectManager_HeapAllocInit(fieldEffMan, (sizeof(SnowballResources)), FALSE, 0);
    resources->fieldEffMan = fieldEffMan;

    SnowballResources_Init(resources);
    return resources;
}

void SnowballRenderer_Free(void *context)
{
    SnowballResources *resources = context;

    SnowballResources_Free(resources);
    FieldEffectManager_HeapFree(resources);
}

static void SnowballResources_Init(SnowballResources *resources)
{
    FieldEffectManager_LoadModel(resources->fieldEffMan, &resources->model, 0, 78, FALSE);
    FieldEffectManager_LoadAnimation(resources->fieldEffMan, &resources->anim, 0, 165, FALSE);
}

static void SnowballResources_Free(SnowballResources *resources)
{
    Simple3D_FreeModel(&resources->model);
    Simple3D_FreeAnimation(&resources->anim);
}

OverworldAnimManager *SnowballRenderer_Init(MapObject *mapObj)
{
    FieldEffectManager *fieldEffMan = MapObject_GetFieldEffectManager(mapObj);
    SnowballResources *resources = FieldEffectManager_GetRendererContext(fieldEffMan, FIELD_EFFECT_RENDERER_SNOWBALL);
    SnowballUserData userData;

    userData.fieldEffMan = fieldEffMan;
    userData.resources = resources;
    userData.mapObj = mapObj;

    VecFx32 zeroPos = { 0, 0, 0 };
    int taskPriority = MapObject_CalculateTaskPriority(mapObj, 2);

    return FieldEffectManager_InitAnimManager(fieldEffMan, &sSnowballRendererAnimFuncs, &zeroPos, 0, &userData, taskPriority);
}

void SnowballRenderer_SetSnowballBroken(OverworldAnimManager *animMan)
{
    SnowballRenderer *renderer = OverworldAnimManager_GetFuncsContext(animMan);

    renderer->snowballBroken = TRUE;
}

static int SnowballRenderer_AnimInit(OverworldAnimManager *animMan, void *context)
{
    SnowballRenderer *renderer = context;
    const SnowballUserData *userData = OverworldAnimManager_GetUserData(animMan);

    renderer->userData = *userData;

    FieldEffectManager_ApplyAnimCopyToModel(renderer->userData.fieldEffMan, &renderer->anim, &renderer->userData.resources->model, &renderer->userData.resources->anim, 0);
    Simple3D_CreateRenderObjectWithAnim(&renderer->renderObj, &renderer->userData.resources->model, &renderer->anim);

    renderer->localID = MapObject_GetLocalID(renderer->userData.mapObj);
    renderer->mapID = MapObject_GetMapID(renderer->userData.mapObj);

    VecFx32 pos, offset;
    MapObject *mapObj = renderer->userData.mapObj;

    MapObject_GetPosPtr(mapObj, &pos);
    MapObject_GetSpriteJumpOffset(mapObj, &offset);

    pos.x += offset.x + 0;
    pos.y += offset.y + 0;
    pos.z += offset.z + FX32_ONE * 6 - FX32_ONE * 6;

    OverworldAnimManager_SetPosition(animMan, &pos);

    return TRUE;
}

static void SnowballRenderer_AnimExit(OverworldAnimManager *animMan, void *context)
{
    SnowballRenderer *renderer = context;
    Simple3D_FreeAnimation(&renderer->anim);
}

static void SnowballRenderer_AnimTick(OverworldAnimManager *animMan, void *context)
{
    SnowballRenderer *renderer = context;
    MapObject *mapObj = renderer->userData.mapObj;

    if (renderer->snowballBroken == FALSE) {
        if (sub_02062764(mapObj, renderer->localID, renderer->mapID) == FALSE) {
            FieldEffectManager_FinishAnimManager(animMan);
            return;
        }
    } else {
        if (Simple3D_HasAnimationReachedEnd(&renderer->anim) == TRUE) {
            FieldEffectManager_FinishAnimManager(animMan);
            return;
        }

        Simple3D_UpdateAnim(&renderer->anim, FX32_ONE, FALSE);
    }

    if (renderer->snowballBroken == FALSE) {
        VecFx32 pos, offset;

        MapObject_GetPosPtr(mapObj, &pos);
        MapObject_GetSpriteJumpOffset(mapObj, &offset);

        pos.x += offset.x + 0;
        pos.y += offset.y + 0;
        pos.z += offset.z + FX32_ONE * 6 - FX32_ONE * 6;

        OverworldAnimManager_SetPosition(animMan, &pos);
    }
}

static void SnowballRenderer_AnimRender(OverworldAnimManager *animMan, void *context)
{
    VecFx32 pos;
    SnowballRenderer *renderer = context;

    OverworldAnimManager_GetPosition(animMan, &pos);
    Simple3D_DrawRenderObjWithPos(&renderer->renderObj, &pos);
}

static const OverworldAnimManagerFuncs sSnowballRendererAnimFuncs = {
    sizeof(SnowballRenderer),
    SnowballRenderer_AnimInit,
    SnowballRenderer_AnimExit,
    SnowballRenderer_AnimTick,
    SnowballRenderer_AnimRender,
};
