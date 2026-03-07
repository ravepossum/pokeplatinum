#ifndef POKEPLATINUM_OV5_021EB1A0_H
#define POKEPLATINUM_OV5_021EB1A0_H

#include "struct_decls/struct_02061AB4_decl.h"

#include "billboard.h"
#include "overworld_anim_manager.h"

enum ObjGfxAnimState {
    OBJ_GFX_ANIM_STATE_STOPPED = 0,
    OBJ_GFX_ANIM_STATE_WALK_32_FRAMES,
    OBJ_GFX_ANIM_STATE_WALK_16_FRAMES,
    OBJ_GFX_ANIM_STATE_WALK_8_FRAMES,
    OBJ_GFX_ANIM_STATE_WALK_4_FRAMES,
    OBJ_GFX_ANIM_STATE_WALK_2_FRAMES,
    OBJ_GFX_ANIM_STATE_WALK_6_FRAMES,
    OBJ_GFX_ANIM_STATE_WALK_3_FRAMES,
    OBJ_GFX_ANIM_STATE_WALK_7_FRAMES,
    OBJ_GFX_ANIM_STATE_STANDARD_COUNT,
};

enum NurseGfxAnimState {
    NURSE_GFX_ANIM_STATE_BOW = OBJ_GFX_ANIM_STATE_STANDARD_COUNT,
    NURSE_GFX_ANIM_STATE_COUNT,
};

enum PlayerGfxAnimState {
    PLAYER_GFX_ANIM_STATE_RUN = OBJ_GFX_ANIM_STATE_STANDARD_COUNT,
    PLAYER_GFX_ANIM_STATE_COUNT,
};

enum BikingGfxAnimState {
    BIKING_GFX_ANIM_STATE_JUMP_16_FRAMES = OBJ_GFX_ANIM_STATE_STANDARD_COUNT,
    BIKING_GFX_ANIM_STATE_JUMP_8_FRAMES,
    BIKING_GFX_ANIM_STATE_COUNT,
};

enum FishingGfxAnimState {
    FISHING_GFX_ANIM_STATE_END = 0,
    FISHING_GFX_ANIM_STATE_BEGIN,
    FISHING_GFX_ANIM_STATE_HOOKED,
    FISHING_GFX_ANIM_STATE_REEL_IN,
    FISHING_GFX_ANIM_STATE_COUNT,
};

typedef struct {
    u16 draw;
    u16 animNum;
    fx32 frameNum;
} BillboardAnimState;

typedef struct MapObjectGfx {
    s16 animSteps;
    s8 previousDir;
    s8 previousAnimState;
    Billboard *billboard;
    BillboardAnimState animState;
} MapObjectGfx;

typedef struct {
    s8 previousDir;
    s8 floppedFrames;
    s8 flopTimer;
    s8 previousAnimState;
    Billboard *billboard;
    BillboardAnimState animState;
} MagikarpMapObjectGfx;

typedef struct {
    s8 previousDir;
    s8 animSteps;
    s8 previousAnimState;
    u8 stoppedRunningOrBiking : 1;
    u8 fishingState : 7;
    Billboard *billboard;
    BillboardAnimState animState;
} PlayerMapObjectGfx;

typedef struct {
    OverworldAnimManager *unk_00;
} UnkStruct_ov5_021EC760;

typedef struct {
    s16 animSteps;
    s8 previousDir;
    s8 previousAnimState;
    Billboard *billboard;
    BillboardAnimState animState;
    u32 alpha;
} MespritDistWorldMapObjGfx;

typedef struct {
    int unk_00;
    fx32 unk_04;
    Billboard *unk_08;
    BillboardAnimState unk_0C;
} UnkStruct_ov5_021EC938;

typedef void (*ObjGfxAnimStateFunc)(MapObject *, Billboard *, MapObjectGfx *, int);
typedef void (*PlayerGfxAnimStateFunc)(MapObject *, Billboard *, PlayerMapObjectGfx *, int);

Billboard *ov5_021EB1A0(MapObject *mapObj);
void ov5_021EB2EC(MapObject *mapObj);
void ov5_021EB314(MapObject *mapObj);
void ov5_021EB328(MapObject *mapObj);
void ov5_021EB354(MapObject *mapObj);
void ov5_021EB398(MapObject *mapObj);
void ov5_021EB40C(MapObject *mapObj);
void ov5_021EB438(MapObject *mapObj);
void ov5_021EB720(MapObject *mapObj);
void ov5_021EB7F8(MapObject *mapObj);
void ov5_021EB834(MapObject *mapObj);
void ov5_021EB848(MapObject *mapObj);
void ov5_021EB874(MapObject *mapObj);
void ov5_021EB8B8(MapObject *mapObj);
void ov5_021EB944(MapObject *mapObj);
void ov5_021EBA0C(MapObject *mapObj);
void ov5_021EBA34(MapObject *mapObj);
void ov5_021EBA60(MapObject *mapObj);
void ov5_021EBA8C(MapObject *mapObj);
void ov5_021EBAD0(MapObject *mapObj);
void ov5_021EBF50(MapObject *mapObj);
void ov5_021EC15C(MapObject *mapObj);
void ov5_021EC3F0(MapObject *mapObj);
void ov5_021EC454(MapObject *mapObj);
void ov5_021EC4BC(MapObject *mapObj);
void ov5_021EC554(MapObject *mapObj);
void ov5_021EC5C0(MapObject *mapObj);
void ov5_021EC734(MapObject *mapObj);
void ov5_021EC75C(MapObject *mapObj);
void ov5_021EC760(MapObject *mapObj);
void ov5_021EC778(MapObject *mapObj);
void ov5_021EC790(MapObject *mapObj);
void ov5_021EC7B8(MapObject *mapObj);
void ov5_021EC7D0(MapObject *mapObj);
void ov5_021EC7F0(MapObject *mapObj);
void ov5_021EC824(MapObject *mapObj);
void ov5_021EC858(MapObject *mapObj);
void ov5_021EC86C(MapObject *mapObj);
void ov5_021EC898(MapObject *mapObj);
void ov5_021EC8EC(MapObject *mapObj);
void ov5_021EC938(MapObject *mapObj);
void ov5_021EC95C(MapObject *mapObj);
void ov5_021EC970(MapObject *mapObj);
void ov5_021EC9A0(MapObject *mapObj);
void ov5_021EC9E8(MapObject *mapObj);

#endif // POKEPLATINUM_OV5_021EB1A0_H
