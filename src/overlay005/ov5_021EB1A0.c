#include "overlay005/ov5_021EB1A0.h"

#include <nitro.h>

#include "constants/map_object.h"
#include "generated/object_events_gfx.h"

#include "struct_decls/struct_02061AB4_decl.h"

#include "overlay005/const_ov5_021FB51C.h"
#include "overlay005/field_effect_manager.h"
#include "overlay005/ov5_021EB1A0.h"
#include "overlay005/ov5_021ECA70.h"
#include "overlay005/ov5_021ECC20.h"
#include "overlay005/ov5_021ECE40.h"
#include "overlay005/ov5_021F10E8.h"
#include "overlay005/snowball_renderer.h"
#include "overlay005/struct_ov5_021EC700.h"
#include "overlay005/struct_ov5_021ECD10.h"

#include "billboard.h"
#include "map_object.h"
#include "math_util.h"

static void ObjGfxAnimState_Standard_Stopped(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir);
static void ObjGfxAnimState_Standard_Walk32And16Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir);
static void ObjGfxAnimState_Standard_Walk8Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir);
static void ObjGfxAnimState_Standard_Walk4Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir);
static void ObjGfxAnimState_Standard_Walk2Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir);
static void ObjGfxAnimState_Standard_Walk6Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir);
static void ObjGfxAnimState_Standard_Walk3Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir);
static void PlayerGfxAnimState_Biking_Jump16Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir);
static void PlayerGfxAnimState_Biking_Jump8Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir);
static void SnapBillboardAnimToStep(Billboard *billboard, int framesPerStep);
static void SnapBillboardAnimToStep_8Frames(Billboard *billboard);
static void SetBillboardAnimNumAndSnapToStep(Billboard *billboard, int animNum, int framesPerStep);
static void SetBillboardAnimNumAndSnapTo8FrameStep(Billboard *billboard, int animNum);
static void UpdateBillboardDrawFlag(MapObject *mapObj, Billboard *billboard);
static void SetFishingJumpOffset(MapObject *mapObj, Billboard *billboard, int dir);

static const ObjGfxAnimStateFunc ObjGfxAnimStateFuncs_Standard[];
static const ObjGfxAnimStateFunc ObjGfxAnimStateFuncs_PokecenterNurse[];
static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Standard[];
static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Biking[];
static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Fishing[];

Billboard *MapObject_GetBillboard(MapObject *mapObj)
{
    int graphicsID = MapObject_GetGraphicsID(mapObj);

    switch (graphicsID) {
    case OBJ_EVENT_GFX_PLAYER_M:
    case OBJ_EVENT_GFX_PLAYER_F:
    case OBJ_EVENT_GFX_PLAYER_M_BIKE:
    case OBJ_EVENT_GFX_PLAYER_F_BIKE:
    case OBJ_EVENT_GFX_PLAYER_M_HOLDING_POKE_BALL:
    case OBJ_EVENT_GFX_PLAYER_F_HOLDING_POKE_BALL:
    case OBJ_EVENT_GFX_PLAYER_M_SURF:
    case OBJ_EVENT_GFX_PLAYER_F_SURF:
    case OBJ_EVENT_GFX_PLAYER_M_SPRAYDUCK:
    case OBJ_EVENT_GFX_PLAYER_F_SPRAYDUCK:
    case OBJ_EVENT_GFX_PLAYER_M_SAVE:
    case OBJ_EVENT_GFX_PLAYER_F_SAVE:
    case OBJ_EVENT_GFX_PLAYER_M_CONTEST:
    case OBJ_EVENT_GFX_PLAYER_F_CONTEST:
    case OBJ_EVENT_GFX_PLAYER_M_FISHING:
    case OBJ_EVENT_GFX_PLAYER_F_FISHING:
    case OBJ_EVENT_GFX_PLAYER_M_POKETCH:
    case OBJ_EVENT_GFX_PLAYER_F_POKETCH:
    case OBJ_EVENT_GFX_UNK_200:
    case OBJ_EVENT_GFX_UNK_201:
    case OBJ_EVENT_GFX_UNK_210:
    case OBJ_EVENT_GFX_UNK_211:
    case OBJ_EVENT_GFX_UNK_212:
    case OBJ_EVENT_GFX_UNK_263:
    case OBJ_EVENT_GFX_UNK_268:
    case OBJ_EVENT_GFX_UNK_269:
    case OBJ_EVENT_GFX_UNK_270:
    case OBJ_EVENT_GFX_UNK_271:
    case OBJ_EVENT_GFX_UNK_272:
    case OBJ_EVENT_GFX_UNK_273:
    case OBJ_EVENT_GFX_UNK_274:
    case OBJ_EVENT_GFX_UNK_275:
        PlayerMapObjectGfx *playerObjGfx = sub_02062AF0(mapObj);
        return playerObjGfx->billboard;
    case OBJ_EVENT_GFX_MAGIKARP:
        MagikarpMapObjectGfx *magikarpObjGfx = sub_02062AF0(mapObj);
        return magikarpObjGfx->billboard;
    case OBJ_EVENT_GFX_UXIE:
    case OBJ_EVENT_GFX_MESPRIT:
    case OBJ_EVENT_GFX_AZELF:
        return ov5_021ECB80(mapObj);
    case OBJ_EVENT_GFX_MESPRIT_DISTORTION_WORLD:
        MespritDistWorldMapObjGfx *mespritObjGfx = sub_02062AF0(mapObj);
        return mespritObjGfx->billboard;
    case OBJ_EVENT_GFX_UNK_100:
        return NULL;
    default:
        const UnkStruct_ov5_021ECD10 *v4 = ov5_021ECD04(mapObj);

        if (v4->unk_04_0 == 1) {
            MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);
            return mapObjGfx->billboard;
        }
    }

    return NULL;
}

void MapObjGfxFunc_Standard_Init(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062ACC(mapObj, sizeof(MapObjectGfx));
    mapObjGfx->previousDir = DIR_NONE;

    ov5_021ECF04(mapObj, &mapObjGfx->billboard);

    if (mapObjGfx->billboard != NULL) {
        sub_02062B68(mapObj);
    }
}

void MapObjGfxFunc_Standard_Delete(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);
    MapObject_DeleteGfx(mapObj, &mapObjGfx->billboard);
}

void MapObjGfxFunc_Standard_Pause(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);

    if (mapObjGfx->billboard != NULL) {
        Billboard_StoreAnimState(mapObjGfx->billboard, &mapObjGfx->animState);
    }

    MapObject_DeleteGfx(mapObj, &mapObjGfx->billboard);
    MapObject_SetStatusFlagOn(mapObj, MAP_OBJ_STATUS_21);
}

void MapObjGfxFunc_Standard_Resume(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (mapObjGfx->billboard == NULL) {
        ov5_021ECF04(mapObj, &mapObjGfx->billboard);
    }

    if (mapObjGfx->billboard != NULL) {
        Billboard_RestoreAnimState(mapObjGfx->billboard, &mapObjGfx->animState);
        MapObject_UpdateBillboardPos(mapObj, mapObjGfx->billboard);

        MapObject_SetStatusFlagOff(mapObj, MAP_OBJ_STATUS_21);
    }
}

void MapObjGfxFunc_Standard_Update(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);
    Billboard *billboard = mapObjGfx->billboard;

    mapObjGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int currentDir = MapObject_GetFacingDir(mapObj);
    int animState = sub_02062A14(mapObj);

    GF_ASSERT(animState < OBJ_GFX_ANIM_STATE_STANDARD_COUNT);
    ObjGfxAnimStateFuncs_Standard[animState](mapObj, billboard, mapObjGfx, currentDir);

    mapObjGfx->previousDir = currentDir;
    mapObjGfx->previousAnimState = sub_02062A14(mapObj);

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

// MapObjGfxFunc_NoAnim_Update
void ov5_021EB40C(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    Billboard *billboard = mapObjGfx->billboard;

    if (billboard == NULL) {
        return;
    }

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

// MapObjGfxFunc_SingleAnim_Update
void ov5_021EB438(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    Billboard *billboard = mapObjGfx->billboard;

    if (billboard == NULL) {
        return;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE);
    }

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

static void ObjGfxAnimState_Standard_Stopped(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else {
        SnapBillboardAnimToStep_8Frames(billboard);
    }

    Billboard_AdvanceAnim(billboard, 0);
}

static void ObjGfxAnimState_Standard_Walk32And16Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE / 2);
    }
}

static void ObjGfxAnimState_Standard_Walk8Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE);
    }
}

static void ObjGfxAnimState_Standard_Walk4Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE * 2);
    }
}

static void ObjGfxAnimState_Standard_Walk2Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE * 4);
    }
}

static void ObjGfxAnimState_Standard_Walk6Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);

        mapObjGfx->animSteps = 0;
    }

    if (mapObjGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_6_FRAMES) {
        mapObjGfx->animSteps = 0;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        fx32 stepsToFrames[] = {
            FX32_ONE,
            FX32_ONE,
            FX32_ONE * 2,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE * 2,
            0,
        };

        Billboard_AdvanceAnim(billboard, stepsToFrames[mapObjGfx->animSteps]);
        mapObjGfx->animSteps++;

        if (stepsToFrames[mapObjGfx->animSteps] == 0) {
            mapObjGfx->animSteps = 0;
        }
    }
}

static void ObjGfxAnimState_Standard_Walk3Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);

        mapObjGfx->animSteps = 0;
    }

    if (mapObjGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_3_FRAMES) {
        mapObjGfx->animSteps = 0;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == 0) {
        fx32 stepsToFrames[] = {
            FX32_ONE * 3,
            FX32_ONE * 2,
            FX32_ONE * 3,
            0,
        };

        Billboard_AdvanceAnim(billboard, stepsToFrames[mapObjGfx->animSteps]);
        mapObjGfx->animSteps++;

        if (stepsToFrames[mapObjGfx->animSteps] == 0) {
            mapObjGfx->animSteps = 0;
        }
    }
}

static void ObjGfxAnimState_Standard_Walk7Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);

        mapObjGfx->animSteps = 0;
    }

    if (mapObjGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_7_FRAMES) {
        mapObjGfx->animSteps = 0;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == 0) {
        fx32 stepsToFrames[] = {
            FX32_ONE * 2,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE,
            0,
        };

        Billboard_AdvanceAnim(billboard, stepsToFrames[mapObjGfx->animSteps]);

        mapObjGfx->animSteps++;
        if (stepsToFrames[mapObjGfx->animSteps] == 0) {
            mapObjGfx->animSteps = 0;
        }
    }
}

static const ObjGfxAnimStateFunc ObjGfxAnimStateFuncs_Standard[] = {
    ObjGfxAnimState_Standard_Stopped,
    ObjGfxAnimState_Standard_Walk32And16Frames,
    ObjGfxAnimState_Standard_Walk32And16Frames,
    ObjGfxAnimState_Standard_Walk8Frames,
    ObjGfxAnimState_Standard_Walk4Frames,
    ObjGfxAnimState_Standard_Walk2Frames,
    ObjGfxAnimState_Standard_Walk6Frames,
    ObjGfxAnimState_Standard_Walk3Frames,
    ObjGfxAnimState_Standard_Walk7Frames,
};

void MapObjGfxFunc_PokecenterNurse_Update(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);
    Billboard *billboard = mapObjGfx->billboard;

    mapObjGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);
    int animState = sub_02062A14(mapObj);

    GF_ASSERT(animState < NURSE_GFX_ANIM_STATE_COUNT);

    ObjGfxAnimStateFuncs_PokecenterNurse[animState](mapObj, billboard, mapObjGfx, dir);

    mapObjGfx->previousDir = dir;
    mapObjGfx->previousAnimState = sub_02062A14(mapObj);

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

static void ObjGfxAnimState_PokecenterNurse_Stopped(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir || mapObjGfx->previousAnimState == NURSE_GFX_ANIM_STATE_BOW) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else {
        SnapBillboardAnimToStep_8Frames(billboard);
    }

    Billboard_AdvanceAnim(billboard, 0);
}

static void ObjGfxAnimState_PokecenterNurse_Bow(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (mapObjGfx->previousAnimState != NURSE_GFX_ANIM_STATE_BOW) {
        Billboard_SetAnimNum(billboard, 4);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    Billboard_AdvanceAnim(billboard, FX32_ONE);
}

static const ObjGfxAnimStateFunc ObjGfxAnimStateFuncs_PokecenterNurse[] = {
    ObjGfxAnimState_PokecenterNurse_Stopped,
    ObjGfxAnimState_Standard_Walk32And16Frames,
    ObjGfxAnimState_Standard_Walk32And16Frames,
    ObjGfxAnimState_Standard_Walk8Frames,
    ObjGfxAnimState_Standard_Walk4Frames,
    ObjGfxAnimState_Standard_Walk2Frames,
    ObjGfxAnimState_Standard_Walk6Frames,
    ObjGfxAnimState_Standard_Walk3Frames,
    ObjGfxAnimState_Standard_Walk7Frames,
    ObjGfxAnimState_PokecenterNurse_Bow,
};

void MapObjGfxFunc_Magikarp_Init(MapObject *mapObj)
{
    MagikarpMapObjectGfx *magikarpGfx = sub_02062ACC(mapObj, sizeof(MagikarpMapObjectGfx));

    magikarpGfx->previousDir = DIR_NONE;
    magikarpGfx->flopTimer = LCRNG_Next() % 16;

    ov5_021ECF04(mapObj, &magikarpGfx->billboard);

    if (magikarpGfx->billboard != NULL) {
        sub_02062B68(mapObj);
    }
}

void MapObjGfxFunc_Magikarp_Delete(MapObject *mapObj)
{
    MagikarpMapObjectGfx *magikarpGfx = sub_02062AF0(mapObj);
    MapObject_DeleteGfx(mapObj, &magikarpGfx->billboard);
}

void MapObjGfxFunc_Magikarp_Pause(MapObject *mapObj)
{
    MagikarpMapObjectGfx *magikarpGfx = sub_02062AF0(mapObj);

    if (magikarpGfx->billboard != NULL) {
        Billboard_StoreAnimState(magikarpGfx->billboard, &magikarpGfx->animState);
    }

    MapObject_DeleteGfx(mapObj, &magikarpGfx->billboard);
    MapObject_SetStatusFlagOn(mapObj, MAP_OBJ_STATUS_21);
}

void MapObjGfxFunc_Magikarp_Resume(MapObject *mapObj)
{
    MagikarpMapObjectGfx *magikarpGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (magikarpGfx->billboard == NULL) {
        ov5_021ECF04(mapObj, &magikarpGfx->billboard);
    }

    if (magikarpGfx->billboard != NULL) {
        Billboard_RestoreAnimState(magikarpGfx->billboard, &magikarpGfx->animState);
        MapObject_UpdateBillboardPos(mapObj, magikarpGfx->billboard);
        MapObject_SetStatusFlagOff(mapObj, MAP_OBJ_STATUS_21);
    }
}

void MapObjGfxFunc_Magikarp_Update(MapObject *mapObj)
{
    MagikarpMapObjectGfx *magikarpGfx = sub_02062AF0(mapObj);
    Billboard *billboard = magikarpGfx->billboard;

    magikarpGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);
    int dummy = sub_02062A14(mapObj);

    if (magikarpGfx->flopTimer) {
        magikarpGfx->flopTimer--;
        Billboard_AdvanceAnim(billboard, 0);
    } else {
        Billboard_AdvanceAnim(billboard, FX32_ONE);
        magikarpGfx->floppedFrames++;

        if (magikarpGfx->floppedFrames >= 32) {
            magikarpGfx->floppedFrames = 0;
            magikarpGfx->flopTimer = LCRNG_Next() % 16;
        }
    }

    magikarpGfx->previousDir = dir;
    magikarpGfx->previousAnimState = sub_02062A14(mapObj);

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

// MapObjGfxFunc_Starly_Update
void ov5_021EB944(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);
    Billboard *billboard = mapObjGfx->billboard;

    mapObjGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);
    int animState = sub_02062A14(mapObj);

    switch (animState) {
    case 0:
        int animNum;

        if (dir == DIR_NORTH || dir == DIR_SOUTH) {
            animNum = 0;
        } else {
            animNum = 1;
        }

        if (Billboard_GetAnimNum(billboard) != animNum || mapObjGfx->previousAnimState == 0) {
            Billboard_SetAnimNum(billboard, animNum);
            Billboard_SetAnimFrameNum(billboard, 0);
            Billboard_AdvanceAnim(billboard, 0);
        }

        Billboard_AdvanceAnim(billboard, 0);
        break;
    default:
        int animNum2;

        if (dir == DIR_NORTH || dir == DIR_SOUTH) {
            animNum2 = 0;
        } else {
            animNum2 = 1;
        }

        if (Billboard_GetAnimNum(billboard) != animNum2) {
            Billboard_SetAnimNum(billboard, animNum2);
            Billboard_SetAnimFrameNum(billboard, 0);
        }

        Billboard_AdvanceAnim(billboard, FX32_ONE);
        break;
    }

    mapObjGfx->previousDir = dir;
    mapObjGfx->previousAnimState = sub_02062A14(mapObj);

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

void MapObjGfxFunc_Player_Init(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062ACC(mapObj, sizeof(PlayerMapObjectGfx));
    playerGfx->previousDir = DIR_NONE;

    ov5_021ECF04(mapObj, &playerGfx->billboard);

    if (playerGfx->billboard != NULL) {
        sub_02062B68(mapObj);
    }
}

void MapObjGfxFunc_Player_Delete(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx;
    VecFx32 zeroOffset = { 0, 0, 0 };

    playerGfx = sub_02062AF0(mapObj);

    MapObject_DeleteGfx(mapObj, &playerGfx->billboard);
    MapObject_SetSpriteJumpOffset(mapObj, &zeroOffset);
}

void MapObjGfxFunc_Player_Pause(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);

    if (playerGfx->billboard != NULL) {
        Billboard_StoreAnimState(playerGfx->billboard, &playerGfx->animState);
    }

    MapObject_DeleteGfx(mapObj, &playerGfx->billboard);
    MapObject_SetStatusFlagOn(mapObj, MAP_OBJ_STATUS_21);
}

void MapObjGfxFunc_Player_Resume(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (playerGfx->billboard == NULL) {
        ov5_021ECF04(mapObj, &playerGfx->billboard);
    }

    if (playerGfx->billboard != NULL) {
        Billboard_RestoreAnimState(playerGfx->billboard, &playerGfx->animState);
        MapObject_UpdateBillboardPos(mapObj, playerGfx->billboard);
        MapObject_SetStatusFlagOff(mapObj, MAP_OBJ_STATUS_21);
    }
}

void MapObjGfxFunc_Player_Update(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);
    Billboard *billboard = playerGfx->billboard;

    playerGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);
    int animState = sub_02062A14(mapObj);

    GF_ASSERT(animState < ((0x8 + 1) + 1));
    PlayerGfxAnimStateFuncs_Standard[animState](mapObj, billboard, playerGfx, dir);

    playerGfx->previousDir = dir;
    playerGfx->previousAnimState = sub_02062A14(mapObj);

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

static void PlayerGfxAnimState_Standard_Stopped(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->stoppedRunningOrBiking = FALSE;
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        playerGfx->stoppedRunningOrBiking = TRUE;
    } else if (playerGfx->stoppedRunningOrBiking == TRUE) {
        SetBillboardAnimNumAndSnapTo8FrameStep(billboard, Billboard_WalkDirToAnimNum(dir));
        playerGfx->stoppedRunningOrBiking = FALSE;
    } else {
        SnapBillboardAnimToStep_8Frames(billboard);
    }
}

static void PlayerGfxAnimState_Standard_Walk32And16Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        playerGfx->stoppedRunningOrBiking = TRUE;
        return;
    } else if (playerGfx->stoppedRunningOrBiking == TRUE) {
        SetBillboardAnimNumAndSnapTo8FrameStep(billboard, Billboard_WalkDirToAnimNum(dir));
        playerGfx->stoppedRunningOrBiking = FALSE;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE / 2);
    }
}

static void PlayerGfxAnimState_Standard_Walk8Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        SetBillboardAnimNumAndSnapTo8FrameStep(billboard, Billboard_WalkDirToAnimNum(dir));
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE);
    }
}

static void PlayerGfxAnimState_Standard_Walk4Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        SetBillboardAnimNumAndSnapTo8FrameStep(billboard, Billboard_WalkDirToAnimNum(dir));
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE * 2);
    }
}

static void PlayerGfxAnimState_Standard_Walk2Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        SetBillboardAnimNumAndSnapTo8FrameStep(billboard, Billboard_WalkDirToAnimNum(dir));
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE * 4);
    }
}

static void PlayerGfxAnimState_Standard_Walk6Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->animSteps = 0;
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        SetBillboardAnimNumAndSnapTo8FrameStep(billboard, Billboard_WalkDirToAnimNum(dir));
    }

    if (playerGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_6_FRAMES) {
        playerGfx->animSteps = 0;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        fx32 stepsToFrames[] = {
            FX32_ONE,
            FX32_ONE,
            FX32_ONE * 2,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE * 2,
            0,
        };

        Billboard_AdvanceAnim(billboard, stepsToFrames[playerGfx->animSteps]);
        playerGfx->animSteps++;

        if (stepsToFrames[playerGfx->animSteps] == 0) {
            playerGfx->animSteps = 0;
        }
    }
}

static void PlayerGfxAnimState_Standard_Walk3Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->animSteps = 0;
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        SetBillboardAnimNumAndSnapTo8FrameStep(billboard, Billboard_WalkDirToAnimNum(dir));
    }

    if (playerGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_3_FRAMES) {
        playerGfx->animSteps = 0;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        fx32 stepsToFrames[] = {
            FX32_ONE * 3,
            FX32_ONE * 2,
            FX32_ONE * 3,
            0,
        };

        Billboard_AdvanceAnim(billboard, stepsToFrames[playerGfx->animSteps]);
        playerGfx->animSteps++;

        if (stepsToFrames[playerGfx->animSteps] == 0) {
            playerGfx->animSteps = 0;
        }
    }
}

static void PlayerGfxAnimState_Standard_Walk7Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->animSteps = 0;
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        SetBillboardAnimNumAndSnapTo8FrameStep(billboard, Billboard_WalkDirToAnimNum(dir));
    }

    if (playerGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_7_FRAMES) {
        playerGfx->animSteps = 0;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        fx32 stepsToFrames[] = {
            FX32_ONE * 2,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE,
            FX32_ONE,
            0,
        };

        Billboard_AdvanceAnim(billboard, stepsToFrames[playerGfx->animSteps]);
        playerGfx->animSteps++;

        if (stepsToFrames[playerGfx->animSteps] == 0) {
            playerGfx->animSteps = 0;
        }
    }
}

static void PlayerGfxAnimState_Standard_Run(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_RunDirToAnimNum(dir);
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState != PLAYER_GFX_ANIM_STATE_RUN) {
        SetBillboardAnimNumAndSnapToStep(billboard, Billboard_RunDirToAnimNum(dir), 4);
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE);
    }
}

static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Standard[] = {
    PlayerGfxAnimState_Standard_Stopped,
    PlayerGfxAnimState_Standard_Walk32And16Frames,
    PlayerGfxAnimState_Standard_Walk32And16Frames,
    PlayerGfxAnimState_Standard_Walk8Frames,
    PlayerGfxAnimState_Standard_Walk4Frames,
    PlayerGfxAnimState_Standard_Walk2Frames,
    PlayerGfxAnimState_Standard_Walk6Frames,
    PlayerGfxAnimState_Standard_Walk3Frames,
    PlayerGfxAnimState_Standard_Walk7Frames,
    PlayerGfxAnimState_Standard_Run,
};

void MapObjGfxFunc_PlayerBiking_Update(MapObject *mapObj)
{
    // does not match with C99-style declarations
    int dir, animState;
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);
    Billboard *billboard = playerGfx->billboard;

    playerGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    dir = MapObject_GetFacingDir(mapObj);
    animState = sub_02062A14(mapObj);

    GF_ASSERT(animState < BIKING_GFX_ANIM_STATE_COUNT);

    if (playerGfx->previousAnimState == OBJ_GFX_ANIM_STATE_STOPPED
        && animState != playerGfx->previousAnimState
        && playerGfx->stoppedRunningOrBiking == FALSE) {
        playerGfx->previousDir = DIR_NONE;
    }

    PlayerGfxAnimStateFuncs_Biking[animState](mapObj, billboard, playerGfx, dir);

    playerGfx->previousDir = dir;
    playerGfx->previousAnimState = animState;

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

static void PlayerGfxAnimState_Biking_Stopped(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    int bikeDirToAnimNum[MAX_DIR] = { 6, 7, 8, 9 };
    int animNum = bikeDirToAnimNum[dir];

    if (dir != playerGfx->previousDir) {
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->stoppedRunningOrBiking = FALSE;
    } else if (playerGfx->previousAnimState != OBJ_GFX_ANIM_STATE_STOPPED) {
        playerGfx->stoppedRunningOrBiking = TRUE;
    } else if (playerGfx->stoppedRunningOrBiking == TRUE) {
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->stoppedRunningOrBiking = FALSE;
    } else {
        Billboard_AdvanceAnim(billboard, FX32_ONE);
    }
}

static void PlayerGfxAnimState_Biking_Jump16Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    int animNum = Billboard_BikeJumpDirToAnimNum(dir);

    if (dir != playerGfx->previousDir || playerGfx->previousAnimState != OBJ_GFX_ANIM_STATE_STOPPED) {
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState != BIKING_GFX_ANIM_STATE_JUMP_16_FRAMES) {
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (Billboard_GetAnimNum(billboard) != animNum) {
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (MapObject_ShouldPauseAnim(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE / 2);
    }
}

static void PlayerGfxAnimState_Biking_Jump8Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    int animNum = Billboard_BikeJumpDirToAnimNum(dir);

    if (dir != playerGfx->previousDir) {
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState != BIKING_GFX_ANIM_STATE_JUMP_8_FRAMES) {
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (Billboard_GetAnimNum(billboard) != animNum) {
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (MapObject_ShouldPauseAnim(mapObj) == 0) {
        Billboard_AdvanceAnim(billboard, FX32_ONE );
    }
}

static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Biking[] = {
    PlayerGfxAnimState_Biking_Stopped,
    PlayerGfxAnimState_Standard_Walk32And16Frames,
    PlayerGfxAnimState_Standard_Walk32And16Frames,
    PlayerGfxAnimState_Standard_Walk8Frames,
    PlayerGfxAnimState_Standard_Walk4Frames,
    PlayerGfxAnimState_Standard_Walk2Frames,
    PlayerGfxAnimState_Standard_Walk6Frames,
    PlayerGfxAnimState_Standard_Walk3Frames,
    PlayerGfxAnimState_Standard_Walk7Frames,
    PlayerGfxAnimState_Biking_Jump16Frames,
    PlayerGfxAnimState_Biking_Jump8Frames,
};

static const int BillboardFishingDirAnimTable[MAX_DIR] = {
    [DIR_NORTH] = 0,
    [DIR_SOUTH] = 1,
    [DIR_WEST] = 2,
    [DIR_EAST] = 3,
};

void MapObjGfxFunc_PlayerFishing_Update(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);
    Billboard *billboard = playerGfx->billboard;

    playerGfx = sub_02062AF0(mapObj);

    if (billboard == NULL || ov5_021EDD94(mapObj) == 1) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);
    int animState = sub_02062A14(mapObj);

    GF_ASSERT(animState < FISHING_GFX_ANIM_STATE_COUNT);

    PlayerGfxAnimStateFuncs_Fishing[animState](mapObj, billboard, playerGfx, dir);
    SetFishingJumpOffset(mapObj, billboard, dir);

    playerGfx->previousDir = dir;
    playerGfx->previousAnimState = sub_02062A14(mapObj);

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

static void PlayerGfxAnimState_Fishing_End(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir || playerGfx->previousAnimState != FISHING_GFX_ANIM_STATE_END) {
        Billboard_SetAnimNum(billboard, BillboardFishingDirAnimTable[dir]);
        Billboard_SetAnimFrameNum(billboard, FX32_ONE * 15);
    }

    fx32 frameNum = Billboard_GetAnimFrameNum(billboard);
    frameNum -= FX32_ONE;

    if (frameNum < 0) {
        frameNum = 0;
    }

    Billboard_SetAnimFrameNum(billboard, frameNum);
    Billboard_AdvanceAnim(billboard, 0);
}

static void PlayerGfxAnimState_Fishing_Begin(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir || playerGfx->previousAnimState != FISHING_GFX_ANIM_STATE_BEGIN) {
        Billboard_SetAnimNum(billboard, BillboardFishingDirAnimTable[dir]);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    Billboard_AdvanceAnim(billboard, FX32_ONE );
}

static void PlayerGfxAnimState_Fishing_Hooked(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir || playerGfx->previousAnimState != FISHING_GFX_ANIM_STATE_HOOKED) {
        Billboard_SetAnimNum(billboard, BillboardFishingDirAnimTable[dir]);
        Billboard_SetAnimFrameNum(billboard, FX32_ONE * 15);
        playerGfx->fishingState = 0;
    }

    fx32 frameNum = Billboard_GetAnimFrameNum(billboard);

    switch (playerGfx->fishingState) {
    case 0:
        frameNum -= FX32_ONE;

        if (frameNum <= FX32_ONE * 10) {
            playerGfx->fishingState++;
        }
        break;
    case 1:
        frameNum += FX32_ONE;

        if (frameNum >= FX32_ONE * 15) {
            playerGfx->fishingState++;
            playerGfx->animSteps = 0;
        }
        break;
    case 2:
        playerGfx->animSteps++;

        if (playerGfx->animSteps >= 30) {
            playerGfx->animSteps = 0;
            playerGfx->fishingState = 0;
        }
    }

    Billboard_SetAnimFrameNum(billboard, frameNum);
    Billboard_AdvanceAnim(billboard, 0);
}

static void PlayerGfxAnimState_Fishing_ReelIn(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir || playerGfx->previousAnimState != FISHING_GFX_ANIM_STATE_REEL_IN) {
        Billboard_SetAnimNum(billboard, BillboardFishingDirAnimTable[dir]);
        Billboard_SetAnimFrameNum(billboard, FX32_ONE * 15);
    }

    fx32 frameNum = Billboard_GetAnimFrameNum(billboard) - FX32_ONE;

    if (frameNum < 0) {
        frameNum = 0;
    }

    Billboard_SetAnimFrameNum(billboard, frameNum);
    Billboard_AdvanceAnim(billboard, 0);
}

static void SetFishingJumpOffset(MapObject *mapObj, Billboard *billboard, int dir)
{
    VecFx32 offsetVec = { 0, 0, 0 };
    int frameNum = Billboard_GetFrameNum(billboard) / FX32_ONE;

    switch (dir) {
    case DIR_NORTH:
        break;
    case DIR_SOUTH:
        if (frameNum >= 24) {
            offsetVec.z = FX32_ONE * 6;
        }
        break;
    case DIR_WEST:
        if (frameNum < 40) {
            offsetVec.x = FX32_ONE * 1;
        } else {
            offsetVec.x = FX32_ONE * -5;
        }
        break;
    case DIR_EAST:
        if (frameNum < 56) {
            offsetVec.x = FX32_ONE * -1;
        } else {
            offsetVec.x = FX32_ONE * 5;
        }
        break;
    }

    MapObject_SetSpriteJumpOffset(mapObj, &offsetVec);
}

static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Fishing[] = {
    PlayerGfxAnimState_Fishing_End,
    PlayerGfxAnimState_Fishing_Begin,
    PlayerGfxAnimState_Fishing_Hooked,
    PlayerGfxAnimState_Fishing_ReelIn,
};

// MapObjGfxFunc_PlayerSurfing_Update ?
void ov5_021EC3F0(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);
    Billboard *billboard = playerGfx->billboard;

    playerGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);

    playerGfx->previousDir = dir;
    playerGfx->previousAnimState = sub_02062A14(mapObj);

    int animNum = Billboard_WalkDirToAnimNum(dir);

    Billboard_SetAnimNum(billboard, animNum);
    Billboard_SetAnimFrameNum(billboard, 0);
    Billboard_AdvanceAnim(billboard, 0);

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

void ov5_021EC454(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);
    Billboard *billboard = playerGfx->billboard;

    playerGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);

    playerGfx->previousDir = dir;
    playerGfx->previousAnimState = sub_02062A14(mapObj);

    if (Billboard_GetAnimNum(billboard) != 0) {
        Billboard_SetAnimNum(billboard, 0);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    Billboard_AdvanceAnim(billboard, FX32_ONE);

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

void ov5_021EC4BC(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);
    Billboard *billboard = playerGfx->billboard;

    playerGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);
    int animState = sub_02062A14(mapObj);

    switch (animState) {
    case 0:
        if (Billboard_GetAnimNum(billboard) != 1) {
            Billboard_SetAnimNum(billboard, 1);
            Billboard_SetAnimFrameNum(billboard, 0);
        }

        Billboard_AdvanceAnim(billboard, FX32_ONE);
        break;
    default:
        if (Billboard_GetAnimNum(billboard) != 0) {
            Billboard_SetAnimNum(billboard, 0);
            Billboard_SetAnimFrameNum(billboard, 0);
        }

        Billboard_AdvanceAnim(billboard, FX32_ONE);
    }

    playerGfx->previousDir = dir;
    playerGfx->previousAnimState = sub_02062A14(mapObj);

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

void ov5_021EC554(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);
    Billboard *billboard = playerGfx->billboard;

    playerGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);

    if (dir != playerGfx->previousDir) {
        int animNum = Billboard_WalkDirToAnimNum(dir);
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    Billboard_AdvanceAnim(billboard, FX32_ONE);
    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);

    playerGfx->previousDir = dir;
    playerGfx->previousAnimState = sub_02062A14(mapObj);
}

void ov5_021EC5C0(MapObject *mapObj)
{
    PlayerMapObjectGfx *playerGfx = sub_02062AF0(mapObj);
    Billboard *billboard = playerGfx->billboard;

    playerGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (billboard == NULL) {
        return;
    }

    int dir = MapObject_GetFacingDir(mapObj);
    int animState = sub_02062A14(mapObj);

    if (animState != playerGfx->previousAnimState) {
        if (animState == 0) {
            Billboard_SetAnimNum(billboard, 0);
        } else {
            Billboard_SetAnimNum(billboard, 1);
        }

        Billboard_SetAnimFrameNum(billboard, 0);
    }

    Billboard_AdvanceAnim(billboard, FX32_ONE);

    playerGfx->previousDir = dir;
    playerGfx->previousAnimState = animState;

    MapObject_UpdateBillboardPos(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

static void SnapBillboardAnimToStep(Billboard *billboard, int framesPerStep)
{
    fx32 frame = Billboard_GetAnimFrameNum(billboard);
    frame /= FX32_ONE;
    fx32 offset = frame % framesPerStep;
    frame -= offset;
    frame *= FX32_ONE;

    Billboard_SetAnimFrameNum(billboard, frame);
    Billboard_AdvanceAnim(billboard, 0);
}

static void SnapBillboardAnimToStep_8Frames(Billboard *billboard)
{
    SnapBillboardAnimToStep(billboard, 8);
}

static void SetBillboardAnimNumAndSnapToStep(Billboard *billboard, int animNum, int framesPerStep)
{
    SnapBillboardAnimToStep(billboard, framesPerStep);
    fx32 frameNum = Billboard_GetAnimFrameNum(billboard);

    Billboard_SetAnimNum(billboard, animNum);
    Billboard_SetAnimFrameNum(billboard, frameNum);
}

static void SetBillboardAnimNumAndSnapTo8FrameStep(Billboard *billboard, int animNum)
{
    SnapBillboardAnimToStep_8Frames(billboard);
    fx32 frameNum = Billboard_GetAnimFrameNum(billboard);

    Billboard_SetAnimNum(billboard, animNum);
    Billboard_SetAnimFrameNum(billboard, frameNum);
}

static void UpdateBillboardDrawFlag(MapObject *mapObj, Billboard *billboard)
{
    int draw = TRUE;

    if (MapObject_CheckStatusFlag(mapObj, MAP_OBJ_STATUS_HIDE) == TRUE) {
        draw = FALSE;
    }

    if (MapObject_CheckStatusFlag(mapObj, MAP_OBJ_STATUS_12) == TRUE
        && !MapObject_CheckStatusFlag(mapObj, MAP_OBJ_STATUS_13)) {
        draw = FALSE;
    }

    Billboard_SetDrawFlag(billboard, draw);
}

static void TryApplyGlobalOffset(u32 graphicsID, VecFx32 *outOffset)
{
    const MapObjGfxGlobalOffset *globalOffsets = Unk_ov5_021FB51C;

    while (globalOffsets->graphicsID != OBJ_EVENT_GFX_NONE) {
        if (globalOffsets->graphicsID == graphicsID) {
            *outOffset = globalOffsets->offset;
            return;
        }

        globalOffsets++;
    }

    GF_ASSERT(FALSE);
}

// MapObjGfxFunc_FieldEffect_Init
void ov5_021EC734(MapObject *mapObj)
{
    VecFx32 pos;
    FieldEffectMapObjGfx *fieldEffectGfx = sub_02062ACC(mapObj, sizeof(FieldEffectMapObjGfx));
    TryApplyGlobalOffset(MapObject_GetEffectiveGraphicsID(mapObj), &pos);
    fieldEffectGfx->animManager = FieldEffectMapObject_InitAnim(mapObj, &pos);
}

// MapObjGfxFunc_FieldEffect_Update
void ov5_021EC75C(MapObject *mapObj)
{
    return;
}

// MapObjGfxFunc_FieldEffect_Delete
void ov5_021EC760(MapObject *mapObj)
{
    FieldEffectMapObjGfx *fieldEffectGfx = sub_02062AF0(mapObj);

    if (fieldEffectGfx->animManager != NULL) {
        FieldEffectManager_FinishAnimManager(fieldEffectGfx->animManager);
        fieldEffectGfx->animManager = NULL;
    }
}

// MapObjGfxFunc_FieldEffect_Pause
void ov5_021EC778(MapObject *mapObj)
{
    FieldEffectMapObjGfx *fieldEffectGfx = sub_02062AF0(mapObj);

    if (fieldEffectGfx->animManager != NULL) {
        FieldEffectManager_FinishAnimManager(fieldEffectGfx->animManager);
        fieldEffectGfx->animManager = NULL;
    }
}

// MapObjGfxFunc_FieldEffect_Resume
void ov5_021EC790(MapObject *mapObj)
{
    VecFx32 pos;
    FieldEffectMapObjGfx *fieldEffectGfx = sub_02062AF0(mapObj);
    TryApplyGlobalOffset(MapObject_GetEffectiveGraphicsID(mapObj), &pos);
    fieldEffectGfx->animManager = FieldEffectMapObject_InitAnim(mapObj, &pos);
}

// MapObjGfxFunc_Snowball_Init
void ov5_021EC7B8(MapObject *mapObj)
{
    FieldEffectMapObjGfx *fieldEffectGfx = sub_02062ACC(mapObj, (sizeof(FieldEffectMapObjGfx)));
    fieldEffectGfx->animManager = SnowballRenderer_Init(mapObj);
}

// MapObjGfxFunc_Snowball_Resume
void ov5_021EC7D0(MapObject *mapObj)
{
    FieldEffectMapObjGfx *fieldEffectGfx = sub_02062AF0(mapObj);
    GF_ASSERT(fieldEffectGfx->animManager == NULL);
    fieldEffectGfx->animManager = SnowballRenderer_Init(mapObj);
}

// MapObject_BreakSnowball ?
void ov5_021EC7F0(MapObject *mapObj)
{
    FieldEffectMapObjGfx *fieldEffectGfx = sub_02062AF0(mapObj);
    SnowballRenderer_SetSnowballBroken(fieldEffectGfx->animManager);
    fieldEffectGfx->animManager = NULL;
}

static void BillboardCB_UpdateMespritAlpha(Billboard *billboard, void *mespritGfxData)
{
    MespritDistWorldMapObjGfx *mespritGfx = mespritGfxData;
    NNSG3dResMdl *model = Billboard_GetModel(billboard);

    NNS_G3dMdlUseMdlAlpha(model);
    NNS_G3dMdlSetMdlAlphaAll(model, mespritGfx->alpha);
}

void ov5_021EC824(MapObject *mapObj)
{
    MespritDistWorldMapObjGfx *mespritGfx = sub_02062ACC(mapObj, sizeof(MespritDistWorldMapObjGfx));
    mespritGfx->previousDir = DIR_NONE;

    ov5_021ECF04(mapObj, &mespritGfx->billboard);

    if (mespritGfx->billboard != NULL) {
        Billboard_SetCallback(mespritGfx->billboard, BillboardCB_UpdateMespritAlpha, mespritGfx);
        sub_02062B68(mapObj);
    }
}

void ov5_021EC858(MapObject *mapObj)
{
    MespritDistWorldMapObjGfx *mespritGfx = sub_02062AF0(mapObj);
    MapObject_DeleteGfx(mapObj, &mespritGfx->billboard);
}

void ov5_021EC86C(MapObject *mapObj)
{
    MespritDistWorldMapObjGfx *mespritGfx = sub_02062AF0(mapObj);

    if (mespritGfx->billboard != NULL) {
        Billboard_StoreAnimState(mespritGfx->billboard, &mespritGfx->animState);
    }

    MapObject_DeleteGfx(mapObj, &mespritGfx->billboard);
    MapObject_SetStatusFlagOn(mapObj, MAP_OBJ_STATUS_21);
}

void ov5_021EC898(MapObject *mapObj)
{
    MapObjectGfx *mespritGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (mespritGfx->billboard == NULL) {
        ov5_021ECF04(mapObj, &mespritGfx->billboard);
    }

    if (mespritGfx->billboard != NULL) {
        Billboard_RestoreAnimState(mespritGfx->billboard, &mespritGfx->animState);
        MapObject_UpdateBillboardPos(mapObj, mespritGfx->billboard);
        MapObject_SetStatusFlagOff(mapObj, MAP_OBJ_STATUS_21);
        Billboard_SetCallback(mespritGfx->billboard, BillboardCB_UpdateMespritAlpha, mespritGfx);
    }
}

void ov5_021EC8EC(MapObject *mapObj)
{
    MespritDistWorldMapObjGfx *mespritGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    Billboard *v3 = mespritGfx->billboard;

    if (v3 == NULL) {
        return;
    }

    if (mespritGfx->alpha < 31) {
        mespritGfx->alpha++;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == 0) {
        Billboard_AdvanceAnim(v3, FX32_ONE);
    }

    MapObject_UpdateBillboardPos(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);
}

void ov5_021EC938(MapObject *mapObj)
{
    UnkStruct_ov5_021EC938 *v0 = sub_02062ACC(mapObj, (sizeof(UnkStruct_ov5_021EC938)));
    ov5_021ECF04(mapObj, &v0->unk_08);

    if (v0->unk_08 != NULL) {
        sub_02062B68(mapObj);
    }
}

void ov5_021EC95C(MapObject *mapObj)
{
    UnkStruct_ov5_021EC938 *v0 = sub_02062AF0(mapObj);
    MapObject_DeleteGfx(mapObj, &v0->unk_08);
}

void ov5_021EC970(MapObject *mapObj)
{
    UnkStruct_ov5_021EC938 *v0 = sub_02062AF0(mapObj);

    if (v0->unk_08 != NULL) {
        Billboard_StoreAnimState(v0->unk_08, &v0->unk_0C);
    }

    MapObject_DeleteGfx(mapObj, &v0->unk_08);
    MapObject_SetStatusFlagOn(mapObj, MAP_OBJ_STATUS_21);
}

void ov5_021EC9A0(MapObject *mapObj)
{
    int v0;
    UnkStruct_ov5_021EC938 *v1 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v1->unk_08 == NULL) {
        ov5_021ECF04(mapObj, &v1->unk_08);
    }

    if (v1->unk_08 != NULL) {
        Billboard_RestoreAnimState(v1->unk_08, &v1->unk_0C);
        MapObject_UpdateBillboardPos(mapObj, v1->unk_08);
        MapObject_SetStatusFlagOff(mapObj, MAP_OBJ_STATUS_21);
    }
}

void ov5_021EC9E8(MapObject *mapObj)
{
    int v0, v1;
    UnkStruct_ov5_021EC938 *v2;
    Billboard *v3;

    v2 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    v3 = v2->unk_08;

    if (v3 == NULL) {
        return;
    }

    if (MapObject_ShouldPauseAnim(mapObj) == 0) {
        Billboard_AdvanceAnim(v3, (FX32_ONE));
    }

    {
        VecFx32 v4;
        fx32 v5[16] = {
            0x0,
            0x800,
            0x1000,
            0x2000,
            0x4000,
            0x5800,
            0x7000,
            0x8000,
            0x7800,
            0x6800,
            0x5000,
            0x3000,
            0x2000,
            0x1000,
            0x800,
            0x0
        };

        MapObject_GetSpriteJumpOffset(mapObj, &v4);

        if (v4.y == v2->unk_04) {
            v4.y = v5[v2->unk_00 >> 1];
            v2->unk_00 = (v2->unk_00 + 1) & 0x1f;
            v2->unk_04 = v4.y;

            MapObject_SetSpriteJumpOffset(mapObj, &v4);
        }
    }

    MapObject_UpdateBillboardPos(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);
}
