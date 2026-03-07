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
#include "overlay005/ov5_021F23D0.h"
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
static void PlayerGfxAnimState_Biking_Jump16Frames(MapObject *mapObj, Billboard *param1, PlayerMapObjectGfx *param2, int param3);
static void PlayerGfxAnimState_Biking_Jump8Frames(MapObject *mapObj, Billboard *param1, PlayerMapObjectGfx *param2, int param3);
static void ov5_021EC638(Billboard *param0, int param1);
static void ov5_021EC668(Billboard *param0);
static void ov5_021EC674(Billboard *param0, int param1, int param2);
static void ov5_021EC69C(Billboard *param0, int param1);
static void UpdateBillboardDrawFlag(MapObject *mapObj, Billboard *billboard);
static void ov5_021EC374(MapObject *mapObj, Billboard *param1, int param2);

static const ObjGfxAnimStateFunc ObjGfxAnimStateFuncs_Standard[];
static const ObjGfxAnimStateFunc ObjGfxAnimStateFuncs_PokecenterNurse[];
static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Standard[];
static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Biking[];
static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Fishing[];

// MapObject_GetBillboard
Billboard *ov5_021EB1A0(MapObject *mapObj)
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

// MapObjGfxFunc_Standard_Init
void ov5_021EB2EC(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062ACC(mapObj, sizeof(MapObjectGfx));
    mapObjGfx->previousDir = DIR_NONE;

    // initialize billboard or something
    ov5_021ECF04(mapObj, &mapObjGfx->billboard);

    if (mapObjGfx->billboard != NULL) {
        sub_02062B68(mapObj);
    }
}

// MapObjGfxFunc_Standard_Delete
void ov5_021EB314(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);
    ov5_021ECFA4(mapObj, &mapObjGfx->billboard);
}

// MapObjGfxFunc_Standard_Pause
void ov5_021EB328(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);

    if (mapObjGfx->billboard != NULL) {
        ov5_021ED01C(mapObjGfx->billboard, &mapObjGfx->animState);
    }

    ov5_021ECFA4(mapObj, &mapObjGfx->billboard);
    MapObject_SetStatusFlagOn(mapObj, MAP_OBJ_STATUS_21);
}

// MapObjGfxFunc_Standard_Resume
void ov5_021EB354(MapObject *mapObj)
{
    MapObjectGfx *mapObjGfx = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (mapObjGfx->billboard == NULL) {
        ov5_021ECF04(mapObj, &mapObjGfx->billboard);
    }

    if (mapObjGfx->billboard != NULL) {
        ov5_021ED03C(mapObjGfx->billboard, &mapObjGfx->animState);
        ov5_021EDEB4(mapObj, mapObjGfx->billboard);

        MapObject_SetStatusFlagOff(mapObj, MAP_OBJ_STATUS_21);
    }
}

// MapObjGfxFunc_Standard_Update
void ov5_021EB398(MapObject *mapObj)
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

    ov5_021EDEB4(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

void ov5_021EB40C(MapObject *mapObj)
{
    int v0, v1;
    MapObjectGfx *v2;
    Billboard *v3;

    v2 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    v3 = v2->billboard;

    if (v3 == NULL) {
        return;
    }

    ov5_021EDEB4(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);
}

void ov5_021EB438(MapObject *mapObj)
{
    int v0, v1;
    MapObjectGfx *v2;
    Billboard *v3;

    v2 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    v3 = v2->billboard;

    if (v3 == NULL) {
        return;
    }

    if (ov5_021ECD38(mapObj) == 0) {
        Billboard_AdvanceAnim(v3, (FX32_ONE));
    }

    ov5_021EDEB4(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);
}

static void ObjGfxAnimState_Standard_Stopped(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else {
        ov5_021EC668(billboard);
    }

    Billboard_AdvanceAnim(billboard, 0);
}

static void ObjGfxAnimState_Standard_Walk32And16Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE / 2);
    }
}

static void ObjGfxAnimState_Standard_Walk8Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE);
    }
}

static void ObjGfxAnimState_Standard_Walk4Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE * 2);
    }
}

static void ObjGfxAnimState_Standard_Walk2Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE * 4);
    }
}

static void ObjGfxAnimState_Standard_Walk6Frames(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);

        mapObjGfx->animSteps = 0;
    }

    if (mapObjGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_6_FRAMES) {
        mapObjGfx->animSteps = 0;
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
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
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);

        mapObjGfx->animSteps = 0;
    }

    if (mapObjGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_3_FRAMES) {
        mapObjGfx->animSteps = 0;
    }

    if (ov5_021ECD38(mapObj) == 0) {
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
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);

        mapObjGfx->animSteps = 0;
    }

    if (mapObjGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_7_FRAMES) {
        mapObjGfx->animSteps = 0;
    }

    if (ov5_021ECD38(mapObj) == 0) {
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

// MapObjGfxFunc_PokecenterNurse_Update
void ov5_021EB720(MapObject *mapObj)
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

    ov5_021EDEB4(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

static void ObjGfxAnimState_PokecenterNurse_Stopped(MapObject *mapObj, Billboard *billboard, MapObjectGfx *mapObjGfx, int dir)
{
    if (dir != mapObjGfx->previousDir || mapObjGfx->previousAnimState == NURSE_GFX_ANIM_STATE_BOW) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else {
        ov5_021EC668(billboard);
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

void ov5_021EB7F8(MapObject *mapObj)
{
    MagikarpMapObjectGfx *v0 = sub_02062ACC(mapObj, sizeof(MagikarpMapObjectGfx));

    v0->unk_00 = -1;
    v0->unk_02 = LCRNG_Next() % 16;

    ov5_021ECF04(mapObj, &v0->billboard);

    if (v0->billboard != NULL) {
        sub_02062B68(mapObj);
    }
}

void ov5_021EB834(MapObject *mapObj)
{
    MagikarpMapObjectGfx *v0 = sub_02062AF0(mapObj);
    ov5_021ECFA4(mapObj, &v0->billboard);
}

void ov5_021EB848(MapObject *mapObj)
{
    MagikarpMapObjectGfx *v0 = sub_02062AF0(mapObj);

    if (v0->billboard != NULL) {
        ov5_021ED01C(v0->billboard, &v0->animState);
    }

    ov5_021ECFA4(mapObj, &v0->billboard);
    MapObject_SetStatusFlagOn(mapObj, MAP_OBJ_STATUS_21);
}

void ov5_021EB874(MapObject *mapObj)
{
    int v0;
    MagikarpMapObjectGfx *v1 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v1->billboard == NULL) {
        ov5_021ECF04(mapObj, &v1->billboard);
    }

    if (v1->billboard != NULL) {
        ov5_021ED03C(v1->billboard, &v1->animState);
        ov5_021EDEB4(mapObj, v1->billboard);
        MapObject_SetStatusFlagOff(mapObj, MAP_OBJ_STATUS_21);
    }
}

void ov5_021EB8B8(MapObject *mapObj)
{
    int v0, v1;
    MagikarpMapObjectGfx *v2 = sub_02062AF0(mapObj);
    Billboard *v3 = v2->billboard;

    v2 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v3 == NULL) {
        return;
    }

    v0 = MapObject_GetFacingDir(mapObj);
    v1 = sub_02062A14(mapObj);

    if (v2->unk_02) {
        v2->unk_02--;
        Billboard_AdvanceAnim(v3, 0);
    } else {
        Billboard_AdvanceAnim(v3, (FX32_ONE));
        v2->unk_01++;

        if (v2->unk_01 >= 32) {
            v2->unk_01 = 0;
            v2->unk_02 = LCRNG_Next() % 16;
        }
    }

    v2->unk_00 = v0;
    v2->unk_03 = sub_02062A14(mapObj);

    ov5_021EDEB4(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);
}

void ov5_021EB944(MapObject *mapObj)
{
    int v0, v1;
    MapObjectGfx *v2 = sub_02062AF0(mapObj);
    Billboard *v3 = v2->billboard;

    v2 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v3 == NULL) {
        return;
    }

    v0 = MapObject_GetFacingDir(mapObj);
    v1 = sub_02062A14(mapObj);

    switch (v1) {
    case 0x0: {
        int v4;

        if ((v0 == 0) || (v0 == 1)) {
            v4 = 0;
        } else {
            v4 = 1;
        }

        if ((Billboard_GetAnimNum(v3) != v4) || (v2->previousAnimState == 0x0)) {
            Billboard_SetAnimNum(v3, v4);
            Billboard_SetAnimFrameNum(v3, 0);
            Billboard_AdvanceAnim(v3, 0);
        }

        Billboard_AdvanceAnim(v3, 0);
    } break;
    default: {
        int v5;
        fx32 v6 = 0;

        if ((v0 == 0) || (v0 == 1)) {
            v5 = 0;
        } else {
            v5 = 1;
        }

        if (Billboard_GetAnimNum(v3) != v5) {
            Billboard_SetAnimNum(v3, v5);
            Billboard_SetAnimFrameNum(v3, 0);
        }

        Billboard_AdvanceAnim(v3, (FX32_ONE));
    } break;
    }

    v2->previousDir = v0;
    v2->previousAnimState = sub_02062A14(mapObj);

    ov5_021EDEB4(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);
}

void ov5_021EBA0C(MapObject *mapObj)
{
    PlayerMapObjectGfx *v0 = sub_02062ACC(mapObj, sizeof(PlayerMapObjectGfx));
    v0->previousDir = -1;

    ov5_021ECF04(mapObj, &v0->billboard);

    if (v0->billboard != NULL) {
        sub_02062B68(mapObj);
    }
}

void ov5_021EBA34(MapObject *mapObj)
{
    PlayerMapObjectGfx *v0;
    VecFx32 v1 = { 0, 0, 0 };

    v0 = sub_02062AF0(mapObj);

    ov5_021ECFA4(mapObj, &v0->billboard);
    MapObject_SetSpriteJumpOffset(mapObj, &v1);
}

void ov5_021EBA60(MapObject *mapObj)
{
    PlayerMapObjectGfx *v0 = sub_02062AF0(mapObj);

    if (v0->billboard != NULL) {
        ov5_021ED01C(v0->billboard, &v0->animState);
    }

    ov5_021ECFA4(mapObj, &v0->billboard);
    MapObject_SetStatusFlagOn(mapObj, MAP_OBJ_STATUS_21);
}

void ov5_021EBA8C(MapObject *mapObj)
{
    int v0;
    PlayerMapObjectGfx *v1 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v1->billboard == NULL) {
        ov5_021ECF04(mapObj, &v1->billboard);
    }

    if (v1->billboard != NULL) {
        ov5_021ED03C(v1->billboard, &v1->animState);
        ov5_021EDEB4(mapObj, v1->billboard);
        MapObject_SetStatusFlagOff(mapObj, MAP_OBJ_STATUS_21);
    }
}

// MapObjGfxFunc_Player_Update
void ov5_021EBAD0(MapObject *mapObj)
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

    ov5_021EDEB4(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

static void PlayerGfxAnimState_Standard_Stopped(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->stoppedRunningOrBiking = FALSE;
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        playerGfx->stoppedRunningOrBiking = TRUE;
    } else if (playerGfx->stoppedRunningOrBiking == TRUE) {
        ov5_021EC69C(billboard, ov5_021EDF18(dir));
        playerGfx->stoppedRunningOrBiking = FALSE;
    } else {
        ov5_021EC668(billboard);
    }
}

static void PlayerGfxAnimState_Standard_Walk32And16Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        playerGfx->stoppedRunningOrBiking = TRUE;
        return;
    } else if (playerGfx->stoppedRunningOrBiking == TRUE) {
        ov5_021EC69C(billboard, ov5_021EDF18(dir));
        playerGfx->stoppedRunningOrBiking = FALSE;
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE / 2);
    }
}

static void PlayerGfxAnimState_Standard_Walk8Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        ov5_021EC69C(billboard, ov5_021EDF18(dir));
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE);
    }
}

static void PlayerGfxAnimState_Standard_Walk4Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        ov5_021EC69C(billboard, ov5_021EDF18(dir));
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE * 2);
    }
}

static void PlayerGfxAnimState_Standard_Walk2Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        ov5_021EC69C(billboard, ov5_021EDF18(dir));
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE * 4);
    }
}

static void PlayerGfxAnimState_Standard_Walk6Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir) {
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->animSteps = 0;
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        ov5_021EC69C(billboard, ov5_021EDF18(dir));
    }

    if (playerGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_6_FRAMES) {
        playerGfx->animSteps = 0;
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
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
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->animSteps = 0;
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        ov5_021EC69C(billboard, ov5_021EDF18(dir));
    }

    if (playerGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_3_FRAMES) {
        playerGfx->animSteps = 0;
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
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
        int animNum = ov5_021EDF18(dir);

        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
        playerGfx->animSteps = 0;
    } else if (playerGfx->previousAnimState == PLAYER_GFX_ANIM_STATE_RUN) {
        ov5_021EC69C(billboard, ov5_021EDF18(dir));
    }

    if (playerGfx->previousAnimState != OBJ_GFX_ANIM_STATE_WALK_7_FRAMES) {
        playerGfx->animSteps = 0;
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
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
        int animNum = ov5_021EDF24(dir);
        Billboard_SetAnimNum(billboard, animNum);
        Billboard_SetAnimFrameNum(billboard, 0);
    } else if (playerGfx->previousAnimState != PLAYER_GFX_ANIM_STATE_RUN) {
        ov5_021EC674(
            billboard, ov5_021EDF24(dir), 4);
    }

    if (ov5_021ECD38(mapObj) == FALSE) {
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

// MapObjGfxFunc_PlayerBiking_Update
void ov5_021EBF50(MapObject *mapObj)
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

    ov5_021EDEB4(mapObj, billboard);
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
    int animNum = ov5_021EDF30(dir);

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

    if (ov5_021ECD38(mapObj) == FALSE) {
        Billboard_AdvanceAnim(billboard, FX32_ONE / 2);
    }
}

static void PlayerGfxAnimState_Biking_Jump8Frames(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    int animNum = ov5_021EDF30(dir);

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

    if (ov5_021ECD38(mapObj) == 0) {
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
    0,
    1,
    2,
    3,
};

// MapObjGfxFunc_PlayerFishing_Update
void ov5_021EC15C(MapObject *mapObj)
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
    ov5_021EC374(mapObj, billboard, dir);

    playerGfx->previousDir = dir;
    playerGfx->previousAnimState = sub_02062A14(mapObj);

    ov5_021EDEB4(mapObj, billboard);
    UpdateBillboardDrawFlag(mapObj, billboard);
}

static void ov5_021EC1D8(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
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

static void ov5_021EC228(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir || playerGfx->previousAnimState != FISHING_GFX_ANIM_STATE_BEGIN) {
        Billboard_SetAnimNum(billboard, BillboardFishingDirAnimTable[dir]);
        Billboard_SetAnimFrameNum(billboard, 0);
    }

    Billboard_AdvanceAnim(billboard, FX32_ONE );
}

static void ov5_021EC260(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
{
    if (dir != playerGfx->previousDir || playerGfx->previousAnimState != FISHING_GFX_ANIM_STATE_HOOKED) {
        Billboard_SetAnimNum(billboard, BillboardFishingDirAnimTable[dir]);
        Billboard_SetAnimFrameNum(billboard, FX32_ONE * 15);
        playerGfx->fishingState = 0;
    }

    fx32 frameNum = Billboard_GetAnimFrameNum(billboard);

    switch (playerGfx->fishingState) {
    case 0:
        // EmulatorLog("we in state 0");
        frameNum -= FX32_ONE;

        if (frameNum <= FX32_ONE * 10) {
            playerGfx->fishingState++;
        }
        break;
    case 1:
        // EmulatorLog("we in state 1");
        frameNum += FX32_ONE;

        if (frameNum >= FX32_ONE * 15) {
            playerGfx->fishingState++;
            playerGfx->animSteps = 0;
        }
        break;
    case 2:
        // EmulatorLog("we in state 2");
        playerGfx->animSteps++;

        if (playerGfx->animSteps >= 30) {
            playerGfx->animSteps = 0;
            playerGfx->fishingState = 0;
        }
    }

    Billboard_SetAnimFrameNum(billboard, frameNum);
    Billboard_AdvanceAnim(billboard, 0);
}

static void ov5_021EC324(MapObject *mapObj, Billboard *billboard, PlayerMapObjectGfx *playerGfx, int dir)
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

static void ov5_021EC374(MapObject *mapObj, Billboard *param1, int param2)
{
    VecFx32 v0 = { 0, 0, 0 };
    int v1 = (Billboard_GetFrameNum(param1) / FX32_ONE);

    switch (param2) {
    case 0:
        break;
    case 1:
        if (v1 >= 24) {
            v0.z = (FX32_ONE * 6);
        }
        break;
    case 2:
        if (v1 < 40) {
            v0.x = (FX32_ONE * 1);
        } else {
            v0.x = (FX32_ONE * -5);
        }
        break;
    case 3:
        if (v1 < 56) {
            v0.x = (FX32_ONE * -1);
        } else {
            v0.x = (FX32_ONE * 5);
        }
        break;
    }

    MapObject_SetSpriteJumpOffset(mapObj, &v0);
}

static const PlayerGfxAnimStateFunc PlayerGfxAnimStateFuncs_Fishing[] = {
    ov5_021EC1D8,
    ov5_021EC228,
    ov5_021EC260,
    ov5_021EC324,
};

void ov5_021EC3F0(MapObject *mapObj)
{
    int v0;
    PlayerMapObjectGfx *v1 = sub_02062AF0(mapObj);
    Billboard *v2 = v1->billboard;

    v1 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v2 == NULL) {
        return;
    }

    v0 = MapObject_GetFacingDir(mapObj);

    v1->previousDir = v0;
    v1->previousAnimState = sub_02062A14(mapObj);

    {
        int v3 = ov5_021EDF18(v0);

        Billboard_SetAnimNum(v2, v3);
        Billboard_SetAnimFrameNum(v2, 0);
        Billboard_AdvanceAnim(v2, 0);
    }

    ov5_021EDEB4(mapObj, v2);
    UpdateBillboardDrawFlag(mapObj, v2);
}

void ov5_021EC454(MapObject *mapObj)
{
    int v0;
    PlayerMapObjectGfx *v1 = sub_02062AF0(mapObj);
    Billboard *v2 = v1->billboard;

    v1 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v2 == NULL) {
        return;
    }

    v0 = MapObject_GetFacingDir(mapObj);

    v1->previousDir = v0;
    v1->previousAnimState = sub_02062A14(mapObj);

    if (Billboard_GetAnimNum(v2) != 0) {
        Billboard_SetAnimNum(v2, 0);
        Billboard_SetAnimFrameNum(v2, 0);
    }

    Billboard_AdvanceAnim(v2, (FX32_ONE));

    ov5_021EDEB4(mapObj, v2);
    UpdateBillboardDrawFlag(mapObj, v2);
}

void ov5_021EC4BC(MapObject *mapObj)
{
    int v0, v1;
    PlayerMapObjectGfx *v2 = sub_02062AF0(mapObj);
    Billboard *v3 = v2->billboard;

    v2 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v3 == NULL) {
        return;
    }

    v0 = MapObject_GetFacingDir(mapObj);
    v1 = sub_02062A14(mapObj);

    switch (v1) {
    case 0x0:
        if (Billboard_GetAnimNum(v3) != 1) {
            Billboard_SetAnimNum(v3, 1);
            Billboard_SetAnimFrameNum(v3, 0);
        }

        Billboard_AdvanceAnim(v3, (FX32_ONE));
        break;
    default:
        if (Billboard_GetAnimNum(v3) != 0) {
            Billboard_SetAnimNum(v3, 0);
            Billboard_SetAnimFrameNum(v3, 0);
        }

        Billboard_AdvanceAnim(v3, (FX32_ONE));
    }

    v2->previousDir = v0;
    v2->previousAnimState = sub_02062A14(mapObj);

    ov5_021EDEB4(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);
}

void ov5_021EC554(MapObject *mapObj)
{
    int v0, v1;
    PlayerMapObjectGfx *v2 = sub_02062AF0(mapObj);
    Billboard *v3 = v2->billboard;

    v2 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v3 == NULL) {
        return;
    }

    v0 = MapObject_GetFacingDir(mapObj);

    if (v0 != v2->previousDir) {
        v1 = ov5_021EDF18(v0);
        Billboard_SetAnimNum(v3, v1);
        Billboard_SetAnimFrameNum(v3, 0);
    }

    Billboard_AdvanceAnim(v3, (FX32_ONE));
    ov5_021EDEB4(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);

    v2->previousDir = v0;
    v2->previousAnimState = sub_02062A14(mapObj);
}

void ov5_021EC5C0(MapObject *mapObj)
{
    int v0, v1;
    PlayerMapObjectGfx *v2 = sub_02062AF0(mapObj);
    Billboard *v3 = v2->billboard;

    v2 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v3 == NULL) {
        return;
    }

    v0 = MapObject_GetFacingDir(mapObj);
    v1 = sub_02062A14(mapObj);

    if (v1 != v2->previousAnimState) {
        if (v1 == 0x0) {
            Billboard_SetAnimNum(v3, 0);
        } else {
            Billboard_SetAnimNum(v3, 1);
        }

        Billboard_SetAnimFrameNum(v3, 0);
    }

    Billboard_AdvanceAnim(v3, (FX32_ONE));

    v2->previousDir = v0;
    v2->previousAnimState = v1;

    ov5_021EDEB4(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);
}

// SnapBillboardAnimToStep
static void ov5_021EC638(Billboard *billboard, int framesPerStep)
{
    fx32 frame = Billboard_GetAnimFrameNum(billboard);
    frame /= FX32_ONE;
    fx32 offset = frame % framesPerStep;
    frame -= offset;
    frame *= FX32_ONE;

    Billboard_SetAnimFrameNum(billboard, frame);
    Billboard_AdvanceAnim(billboard, 0);
}

// SnapBillboardAnimToStep_8Frames
static void ov5_021EC668(Billboard *billboard)
{
    ov5_021EC638(billboard, 8);
}

static void ov5_021EC674(Billboard *billboard, int animNum, int framesPerStep)
{
    ov5_021EC638(billboard, framesPerStep);
    fx32 frameNum = Billboard_GetAnimFrameNum(billboard);

    Billboard_SetAnimNum(billboard, animNum);
    Billboard_SetAnimFrameNum(billboard, frameNum);
}

static void ov5_021EC69C(Billboard *billboard, int animNum)
{
    ov5_021EC668(billboard);
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

static void ov5_021EC700(u32 param0, VecFx32 *param1)
{
    const UnkStruct_ov5_021EC700 *v0 = Unk_ov5_021FB51C;

    while (v0->unk_00 != 0xffff) {
        if (v0->unk_00 == param0) {
            *param1 = v0->unk_04;
            return;
        }

        v0++;
    }

    GF_ASSERT(0);
}

void ov5_021EC734(MapObject *mapObj)
{
    VecFx32 v0;
    UnkStruct_ov5_021EC760 *v1 = sub_02062ACC(mapObj, (sizeof(UnkStruct_ov5_021EC760)));
    ov5_021EC700(MapObject_GetEffectiveGraphicsID(mapObj), &v0);
    v1->unk_00 = ov5_021F121C(mapObj, &v0);
}

void ov5_021EC75C(MapObject *mapObj)
{
    return;
}

void ov5_021EC760(MapObject *mapObj)
{
    UnkStruct_ov5_021EC760 *v0 = sub_02062AF0(mapObj);

    if (v0->unk_00 != NULL) {
        FieldEffectManager_FinishAnimManager(v0->unk_00);
        v0->unk_00 = NULL;
    }
}

void ov5_021EC778(MapObject *mapObj)
{
    UnkStruct_ov5_021EC760 *v0 = sub_02062AF0(mapObj);

    if (v0->unk_00 != NULL) {
        FieldEffectManager_FinishAnimManager(v0->unk_00);
        v0->unk_00 = NULL;
    }
}

void ov5_021EC790(MapObject *mapObj)
{
    VecFx32 v0;
    UnkStruct_ov5_021EC760 *v1 = sub_02062AF0(mapObj);
    ov5_021EC700(MapObject_GetEffectiveGraphicsID(mapObj), &v0);
    v1->unk_00 = ov5_021F121C(mapObj, &v0);
}

void ov5_021EC7B8(MapObject *mapObj)
{
    UnkStruct_ov5_021EC760 *v0 = sub_02062ACC(mapObj, (sizeof(UnkStruct_ov5_021EC760)));
    v0->unk_00 = ov5_021F2438(mapObj);
}

void ov5_021EC7D0(MapObject *mapObj)
{
    UnkStruct_ov5_021EC760 *v0 = sub_02062AF0(mapObj);
    GF_ASSERT(v0->unk_00 == NULL);
    v0->unk_00 = ov5_021F2438(mapObj);
}

void ov5_021EC7F0(MapObject *mapObj)
{
    UnkStruct_ov5_021EC760 *v0 = sub_02062AF0(mapObj);
    ov5_021F247C(v0->unk_00);
    v0->unk_00 = NULL;
}

static void ov5_021EC804(Billboard *param0, void *param1)
{
    MespritDistWorldMapObjGfx *v0 = param1;
    NNSG3dResMdl *v1 = Billboard_GetModel(param0);

    NNS_G3dMdlUseMdlAlpha(v1);
    NNS_G3dMdlSetMdlAlphaAll(v1, v0->unk_10);
}

void ov5_021EC824(MapObject *mapObj)
{
    MespritDistWorldMapObjGfx *v0 = sub_02062ACC(mapObj, sizeof(MespritDistWorldMapObjGfx));
    v0->unk_02 = -1;

    ov5_021ECF04(mapObj, &v0->billboard);

    if (v0->billboard != NULL) {
        Billboard_SetCallback(v0->billboard, ov5_021EC804, v0);
        sub_02062B68(mapObj);
    }
}

void ov5_021EC858(MapObject *mapObj)
{
    MespritDistWorldMapObjGfx *v0 = sub_02062AF0(mapObj);
    ov5_021ECFA4(mapObj, &v0->billboard);
}

void ov5_021EC86C(MapObject *mapObj)
{
    MespritDistWorldMapObjGfx *v0 = sub_02062AF0(mapObj);

    if (v0->billboard != NULL) {
        ov5_021ED01C(v0->billboard, &v0->animState);
    }

    ov5_021ECFA4(mapObj, &v0->billboard);
    MapObject_SetStatusFlagOn(mapObj, MAP_OBJ_STATUS_21);
}

void ov5_021EC898(MapObject *mapObj)
{
    int v0;
    MapObjectGfx *v1 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    if (v1->billboard == NULL) {
        ov5_021ECF04(mapObj, &v1->billboard);
    }

    if (v1->billboard != NULL) {
        ov5_021ED03C(v1->billboard, &v1->animState);
        ov5_021EDEB4(mapObj, v1->billboard);
        MapObject_SetStatusFlagOff(mapObj, MAP_OBJ_STATUS_21);
        Billboard_SetCallback(v1->billboard, ov5_021EC804, v1);
    }
}

void ov5_021EC8EC(MapObject *mapObj)
{
    int v0, v1;
    MespritDistWorldMapObjGfx *v2;
    Billboard *v3;

    v2 = sub_02062AF0(mapObj);

    if (ov5_021EDD94(mapObj) == 1) {
        return;
    }

    v3 = v2->billboard;

    if (v3 == NULL) {
        return;
    }

    if (v2->unk_10 < 31) {
        v2->unk_10++;
    }

    if (ov5_021ECD38(mapObj) == 0) {
        Billboard_AdvanceAnim(v3, (FX32_ONE));
    }

    ov5_021EDEB4(mapObj, v3);
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
    ov5_021ECFA4(mapObj, &v0->unk_08);
}

void ov5_021EC970(MapObject *mapObj)
{
    UnkStruct_ov5_021EC938 *v0 = sub_02062AF0(mapObj);

    if (v0->unk_08 != NULL) {
        ov5_021ED01C(v0->unk_08, &v0->unk_0C);
    }

    ov5_021ECFA4(mapObj, &v0->unk_08);
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
        ov5_021ED03C(v1->unk_08, &v1->unk_0C);
        ov5_021EDEB4(mapObj, v1->unk_08);
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

    if (ov5_021ECD38(mapObj) == 0) {
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

    ov5_021EDEB4(mapObj, v3);
    UpdateBillboardDrawFlag(mapObj, v3);
}
