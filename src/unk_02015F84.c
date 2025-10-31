#include "unk_02015F84.h"

#include <nitro.h>
#include <string.h>

#include "constants/graphics.h"

#include "struct_defs/poke_animation_settings.h"

#include "heap.h"
#include "narc.h"
#include "pokemon_sprite.h"
#include "sys_task.h"
#include "sys_task_manager.h"

#define NUM_POKEMON_ANIMS      143
#define NUM_POKEMON_ANIM_FUNCS 34
#define MAX_ANIM_MOTION_FUNCS  4

typedef struct MotionFuncData MotionFuncData;
typedef struct PokemonAnim PokemonAnim;

typedef void (*PokemonAnimFunc)(PokemonAnim *);
typedef void (*MotionFunc)(MotionFuncData *, PokemonAnim *);

typedef struct MotionFuncData {
    BOOL active;
    int unk_04[8];
    int *unk_24;
    int *unk_28;
    u8 unk_2C;
    u8 startDelay;
    int unk_30;
    int unk_34;
    int unk_38;
    int unk_3C;
    int unk_40;
    int unk_44;
    int unk_48;
    int unk_4C;
    MotionFunc motionFunc;
} MotionFuncData;

typedef struct PokemonAnim {
    PokemonSprite *sprite;
    SysTask *task;
    void *narcData;
    u32 *scriptPtr;
    BOOL active;
    int animNum;
    int unk_18;
    int halt;
    BOOL completed;
    int unk_24[8];
    int unk_44;
    int unk_48;
    int unk_4C;
    u32 *unk_50;
    int startDelay;
    int originalX;
    int originalY;
    int unk_60;
    int unk_64;
    int unk_68;
    int unk_6C;
    int unk_70;
    int unk_74;
    int unk_78;
    MotionFuncData motionData[MAX_ANIM_MOTION_FUNCS];
    u8 reverse;
    u8 unk_1C5;
    u8 unk_1C6;
    u8 fadeActive;
} PokemonAnim;

typedef struct PokemonAnimManager {
    PokemonAnim *anims;
    int heapID;
    u8 reverse;
    u8 animCount;
} PokemonAnimManager;

typedef struct {
    MotionFunc motionFunc;
    int paramCount;
    int unk_08;
} MotionFuncParameters;

static int ReadScriptWordIndex(u32 *scriptPtr, u8 index, u8 one);
static int ReadScriptWordIndexZero(u32 *scriptPtr, u8 one);
static int ReadScriptWord(u32 *scriptPtr);
static void sub_02016D04(PokemonAnim *monAnim, const int param1);
static void sub_02016150(SysTask *task, void *monAnim);
static void sub_02016188(PokemonAnim *monAnim);
static void sub_02016530(PokemonAnim *monAnim);
static void sub_02016540(PokemonAnim *monAnim);
static void PokemonAnimFunc_SetOriginalPosition(PokemonAnim *monAnim);
static void sub_02016678(PokemonAnim *monAnim);
static void sub_02016758(PokemonAnim *monAnim);
static void sub_02016590(PokemonAnim *monAnim);
static void sub_020165B8(PokemonAnim *monAnim);
static void sub_020165DC(PokemonAnim *monAnim);
static void sub_02016604(PokemonAnim *monAnim);
static void sub_02016628(PokemonAnim *monAnim);
static void sub_02016650(PokemonAnim *monAnim);
static void sub_0201677C(PokemonAnim *monAnim);
static void sub_020167A0(PokemonAnim *monAnim);
static void sub_020167BC(PokemonAnim *monAnim);
static void sub_020167E8(PokemonAnim *monAnim);
static void sub_02016814(PokemonAnim *monAnim);
static void sub_02016894(PokemonAnim *monAnim);
static void sub_020168C8(PokemonAnim *monAnim);
static void sub_02016900(PokemonAnim *monAnim);
static void sub_02016948(PokemonAnim *monAnim);
static void sub_02016998(PokemonAnim *monAnim);
static void sub_02016A60(PokemonAnim *monAnim);
static void sub_02016AA8(PokemonAnim *monAnim);
static void sub_02016B10(PokemonAnim *monAnim);
static void sub_02016B64(PokemonAnim *monAnim);
static void sub_02016B78(PokemonAnim *monAnim);
static void sub_02016BB8(PokemonAnim *monAnim);
static void sub_02016BD4(PokemonAnim *monAnim);
static void sub_02016BE0(PokemonAnim *monAnim);
static void sub_02016C18(PokemonAnim *monAnim);
static void sub_02016C24(PokemonAnim *monAnim);
static void sub_02016C30(PokemonAnim *monAnim);
static void sub_02016C3C(PokemonAnim *monAnim);
static void sub_02016C48(PokemonAnim *monAnim);
static void sub_02016DAC(MotionFuncData *motionData, PokemonAnim *monAnim);
static void sub_02016E64(MotionFuncData *motionData, PokemonAnim *monAnim);
static void sub_02016F24(MotionFuncData *motionData, PokemonAnim *monAnim);
static void sub_02016F60(MotionFuncData *motionData, PokemonAnim *monAnim);
static void sub_02016F9C(MotionFuncData *motionData, PokemonAnim *monAnim);

static const PokemonAnimFunc sPokemonAnimFuncs[NUM_POKEMON_ANIM_FUNCS] = {
    sub_02016530,
    sub_02016540,
    PokemonAnimFunc_SetOriginalPosition,
    sub_02016678,
    sub_02016758,
    sub_02016590,
    sub_020165B8,
    sub_020165DC,
    sub_02016604,
    sub_02016628,
    sub_02016650,
    sub_0201677C,
    sub_020167A0,
    sub_020167BC,
    sub_020167E8,
    sub_02016814,
    sub_02016894,
    sub_020168C8,
    sub_02016900,
    sub_02016948,
    sub_02016998,
    sub_02016A60,
    sub_02016AA8,
    sub_02016B10,
    sub_02016BD4,
    sub_02016BE0,
    sub_02016C18,
    sub_02016C24,
    sub_02016C30,
    sub_02016C3C,
    sub_02016C48,
    sub_02016B64,
    sub_02016B78,
    sub_02016BB8
};

static const MotionFuncParameters sMotionFuncToParams[] = {
    { sub_02016DAC, 0x6, 0x1 },
    { sub_02016E64, 0x6, 0x1 },
    { sub_02016F24, 0x4, 0x0 },
    { sub_02016F60, 0x3, 0x0 },
    { sub_02016F9C, 0x4, 0x0 }
};

// PokemonAnimManager_New
PokemonAnimManager *sub_02015F84(enum HeapID heapID, const int animCount, const u8 reverse)
{
    PokemonAnimManager *monAnimMan = Heap_Alloc(heapID, sizeof(PokemonAnimManager));
    monAnimMan->reverse = reverse;
    monAnimMan->animCount = animCount;
    monAnimMan->heapID = heapID;
    monAnimMan->anims = Heap_Alloc(heapID, sizeof(PokemonAnim) * animCount);

    MI_CpuClear8(monAnimMan->anims, sizeof(PokemonAnim) * animCount);

    return monAnimMan;
}

// PokemonAnimManager_Free
void sub_02015FB8(PokemonAnimManager *monAnimMan)
{
    Heap_Free(monAnimMan->anims);
    Heap_Free(monAnimMan);
}

// PokemonAnimManager_InitAnimAtIndex
void PokeAnimation_Init(PokemonAnimManager *monAnimMan, PokemonSprite *monSprite, const PokeAnimationSettings *animTemplate, const u8 index)
{
    int animNum = animTemplate->animation;
    int startDelay = animTemplate->startDelay;

    GF_ASSERT(index < monAnimMan->animCount);
    GF_ASSERT(monAnimMan->anims[index].active == FALSE);

    MI_CpuClear8(&monAnimMan->anims[index], sizeof(PokemonAnim));

    monAnimMan->anims[index].active = TRUE;
    monAnimMan->anims[index].sprite = monSprite;

    if (animNum >= NUM_POKEMON_ANIMS) {
        animNum = 0;
        startDelay = 0;
    }

    monAnimMan->anims[index].animNum = animNum;

    if (monAnimMan->reverse) {
        monAnimMan->anims[index].reverse = animTemplate->reverse;
    } else {
        monAnimMan->anims[index].reverse = FALSE;
    }

    monAnimMan->anims[index].narcData = NARC_AllocAtEndAndReadWholeMemberByIndexPair(NARC_INDEX_POKEANIME__PL_POKE_ANM, monAnimMan->anims[index].animNum, monAnimMan->heapID);
    monAnimMan->anims[index].scriptPtr = (u32 *)monAnimMan->anims[index].narcData;
    monAnimMan->anims[index].halt = FALSE;
    monAnimMan->anims[index].completed = FALSE;
    monAnimMan->anims[index].unk_1C5 = 0;
    monAnimMan->anims[index].unk_1C6 = 28;
    monAnimMan->anims[index].fadeActive = FALSE;
    monAnimMan->anims[index].task = SysTask_Start(sub_02016150, &monAnimMan->anims[index], 0);
    monAnimMan->anims[index].startDelay = startDelay;
    monAnimMan->anims[index].originalX = PokemonSprite_GetAttribute(monSprite, MON_SPRITE_X_CENTER);
    monAnimMan->anims[index].originalY = PokemonSprite_GetAttribute(monSprite, MON_SPRITE_Y_CENTER);
    monAnimMan->anims[index].unk_60 = 0;
    monAnimMan->anims[index].unk_64 = 0;
    monAnimMan->anims[index].unk_68 = 0;
    monAnimMan->anims[index].unk_6C = 0;
    monAnimMan->anims[index].unk_70 = 0;
    monAnimMan->anims[index].unk_74 = 0;
    monAnimMan->anims[index].unk_78 = 0;
}

// PokemonAnimManager_HasAnimCompleted
BOOL sub_020160F4(PokemonAnimManager *monAnimMan, const u8 index)
{
    GF_ASSERT(index < monAnimMan->animCount);
    return monAnimMan->anims[index].completed;
}

// PokemonAnimManager_DeleteAnim
void sub_02016114(PokemonAnimManager *monAnimMan, const u8 index)
{
    if (monAnimMan->anims[index].task != NULL) {
        SysTask_Done(monAnimMan->anims[index].task);

        monAnimMan->anims[index].task = NULL;
        monAnimMan->anims[index].completed = TRUE;
        monAnimMan->anims[index].active = FALSE;

        Heap_Free(monAnimMan->anims[index].narcData);
    }
}

// Task_RunPokemonAnim
static void sub_02016150(SysTask *task, void *monAnim)
{
    PokemonAnim *anim = (PokemonAnim *)(monAnim);

    if (anim->startDelay == 0) {
        sub_02016188(anim);
    } else {
        anim->startDelay--;
    }

    if (anim->halt) {
        anim->completed = TRUE;
        anim->active = FALSE;

        SysTask_Done(task);
        anim->task = NULL;
        Heap_Free(anim->narcData);
    }
}

// TickPokemonAnim
static void sub_02016188(PokemonAnim *monAnim)
{
    monAnim->unk_18 = 0;
    monAnim->unk_44 = 0;
    u8 v2 = 0;

    for (u8 i = 0; i < MAX_ANIM_MOTION_FUNCS; i++) {
        MotionFuncData *motionData = &(monAnim->motionData[i]);

        if (motionData->active) {
            if (motionData->startDelay == 0) {
                motionData->motionFunc(motionData, monAnim);
            } else {
                motionData->startDelay--;
            }
        } else {
            v2++;
        }
    }

    if (v2 == 4) {
        monAnim->unk_1C5 = 0;
    }

    if (monAnim->unk_1C5) {
        sub_02016A60(monAnim);
        sub_02016AA8(monAnim);
        return;
    }

    if (monAnim->fadeActive) {
        if (!PokemonSprite_IsFadeActive(monAnim->sprite)) {
            monAnim->fadeActive = FALSE;
        } else {
            return;
        }
    }

    while (TRUE) {
        monAnim->unk_44++;

        GF_ASSERT(*(monAnim->scriptPtr) < NUM_POKEMON_ANIM_FUNCS);

        PokemonAnimFunc currAnimFunc = sPokemonAnimFuncs[*(monAnim->scriptPtr)];
        currAnimFunc(monAnim);

        if (monAnim->halt) {
            break;
        } else {
            monAnim->scriptPtr += 1;

            if (monAnim->unk_18) {
                break;
            } else if (monAnim->unk_1C5) {
                sub_02016A60(monAnim);
                sub_02016AA8(monAnim);
                break;
            }
        }

        if (monAnim->unk_44 >= 256) {
            GF_ASSERT(FALSE);

            monAnim->halt = TRUE;
            break;
        }
    }
}

static int ReadScriptWordIndex(u32 *scriptPtr, u8 index, u8 one)
{
    int ret = scriptPtr[index];

    if (one != 1) {
        GF_ASSERT(0);
    }

    return ret;
}

static int ReadScriptWordIndexZero(u32 *scriptPtr, u8 one)
{
    return ReadScriptWordIndex(scriptPtr, 0, one);
}

static int ReadScriptWord(u32 *scriptPtr)
{
    return ReadScriptWordIndexZero(scriptPtr, 1);
}

static MotionFuncData *MotionFuncData_New(PokemonAnim *monAnim, const u8 param1)
{
    MotionFuncData *retPtr;
    u8 i;

    for (i = 0; i < MAX_ANIM_MOTION_FUNCS; i++) {
        retPtr = &(monAnim->motionData[i]);

        if (retPtr->active == FALSE) {
            MI_CpuClear8(retPtr, sizeof(MotionFuncData));

            retPtr->active = TRUE;
            retPtr->motionFunc = sMotionFuncToParams[param1].motionFunc;

            return retPtr;
        }
    }

    GF_ASSERT(FALSE);
    return NULL;
}

static void ReadScriptInt(PokemonAnim *monAnim, int *outInt)
{
    monAnim->scriptPtr += 1;
    *outInt = (int)ReadScriptWord(monAnim->scriptPtr);
}

static void ReadScriptU8(PokemonAnim *monAnim, u8 *outU8)
{
    monAnim->scriptPtr += 1;
    *outU8 = (u8)ReadScriptWord(monAnim->scriptPtr);
}

static void sub_02016320(PokemonAnim *monAnim, u8 *outU8)
{
    monAnim->scriptPtr += 1;
    (*outU8) = (u8)ReadScriptWord(monAnim->scriptPtr);
    GF_ASSERT((*outU8) < 8);
}

static void sub_02016340(PokemonAnim *monAnim, u8 *param1, u8 *param2)
{
    sub_02016320(monAnim, param1);
    sub_02016320(monAnim, param2);
}

static void sub_02016354(PokemonAnim *monAnim, u8 *param1, int *param2, int *param3)
{
    u8 v0, v1;
    u8 v2;

    sub_02016320(monAnim, param1);
    ReadScriptU8(monAnim, &v2);

    if (v2 == 18) {
        sub_02016320(monAnim, &v0);
        (*param2) = monAnim->unk_24[v0];
        ReadScriptInt(monAnim, param3);
    } else if (v2 == 19) {
        sub_02016340(monAnim, &v0, &v1);
        (*param2) = monAnim->unk_24[v0];
        (*param3) = monAnim->unk_24[v1];
    } else {
        GF_ASSERT(0);
    }
}

static void sub_020163C8(PokemonAnim *monAnim, u8 *param1, int *param2, int *param3)
{
    u8 v0, v1;
    u8 v2, v3;

    sub_02016320(monAnim, param1);
    ReadScriptU8(monAnim, &v2);
    ReadScriptU8(monAnim, &v3);

    if (v2 == 18) {
        ReadScriptInt(monAnim, param2);
    } else if (v2 == 19) {
        sub_02016320(monAnim, &v0);
        (*param2) = monAnim->unk_24[v0];
    } else {
        GF_ASSERT(0);
    }

    if (v3 == 18) {
        ReadScriptInt(monAnim, param3);
    } else if (v3 == 19) {
        sub_02016320(monAnim, &v1);
        (*param3) = monAnim->unk_24[v1];
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016454(PokemonAnim *monAnim, u8 *param1, int *param2, int *param3)
{
    u8 v0, v1, v2;
    int v3;
    int v4;
    u8 v5;

    sub_02016340(monAnim, param1, &v0);
    v3 = monAnim->unk_24[v0];
    ReadScriptU8(monAnim, &v5);

    if (v5 == 20) {
        ReadScriptInt(monAnim, param3);
    } else if (v5 == 21) {
        sub_02016320(monAnim, &v1);
        (*param3) = monAnim->unk_24[v1];
    } else {
        GF_ASSERT(0);
    }

    ReadScriptU8(monAnim, &v5);

    if (v5 == 20) {
        ReadScriptInt(monAnim, &v4);
    } else if (v5 == 21) {
        sub_02016320(monAnim, &v2);
        v4 = monAnim->unk_24[v2];
    } else {
        GF_ASSERT(0);
    }

    (*param2) = v3 + v4;
    (*param2) %= 0x10000;
}

static u8 sub_020164FC(const int *param0, const int *param1)
{
    int v0 = (*param0) - (*param1);

    if (v0 < 0) {
        return 15;
    } else if (v0 > 0) {
        return 16;
    } else {
        return 17;
    }
}

static void sub_02016514(PokemonAnim *monAnim)
{
    int v0 = (-monAnim->unk_74) / 8;
    PokemonSprite_AddAttribute(monAnim->sprite, MON_SPRITE_Y_CENTER, v0);
}

static void sub_02016530(PokemonAnim *monAnim)
{
    PokemonAnimFunc_SetOriginalPosition(monAnim);

    monAnim->unk_18 = 1;
    monAnim->halt = 1;
}

static void sub_02016540(PokemonAnim *monAnim)
{
    monAnim->unk_18 = 1;
}

static void PokemonAnimFunc_SetOriginalPosition(PokemonAnim *monAnim)
{
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_X_CENTER, monAnim->originalX);
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_Y_CENTER, monAnim->originalY);

    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_ROTATION_Z, 0);
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_X_PIVOT, 0);

    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_SCALE_X, MON_AFFINE_SCALE(1));
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_SCALE_Y, MON_AFFINE_SCALE(1));
}

static void sub_02016590(PokemonAnim *monAnim)
{
    u8 v0, v1;

    sub_02016340(monAnim, &v0, &v1);
    monAnim->unk_24[v0] = monAnim->unk_24[v1];
}

static void sub_020165B8(PokemonAnim *monAnim)
{
    u8 v0;
    int v1, v2;

    sub_02016354(monAnim, &v0, &v1, &v2);
    monAnim->unk_24[v0] = v1 + v2;
}

static void sub_020165DC(PokemonAnim *monAnim)
{
    u8 v0;
    int v1, v2;

    sub_02016354(monAnim, &v0, &v1, &v2);
    monAnim->unk_24[v0] = v1 * v2;
}

static void sub_02016604(PokemonAnim *monAnim)
{
    u8 v0;
    int v1, v2;

    sub_020163C8(monAnim, &v0, &v1, &v2);
    monAnim->unk_24[v0] = v1 - v2;
}

static void sub_02016628(PokemonAnim *monAnim)
{
    u8 v0;
    int v1, v2;

    sub_020163C8(monAnim, &v0, &v1, &v2);
    monAnim->unk_24[v0] = v1 / v2;
}

static void sub_02016650(PokemonAnim *monAnim)
{
    u8 v0;
    int v1, v2;

    sub_020163C8(monAnim, &v0, &v1, &v2);
    monAnim->unk_24[v0] = v1 % v2;
}

static void sub_02016678(PokemonAnim *monAnim)
{
    u8 v0, v1;
    u8 v2, v3;
    u8 v4;

    int v5, v6;

    ReadScriptU8(monAnim, &v4);

    if (v4 == 20) {
        sub_02016320(monAnim, &v0);
        v5 = monAnim->unk_24[v0];
        ReadScriptInt(monAnim, &v6);
    } else if (v4 == 21) {
        sub_02016340(monAnim, &v0, &v1);
        v5 = monAnim->unk_24[v0];
        v6 = monAnim->unk_24[v1];
    } else {
        GF_ASSERT(0);
    }

    ReadScriptU8(monAnim, &v2);
    GF_ASSERT(v2 <= 17);

    v3 = sub_020164FC(&v5, &v6);

    int v7;

    ReadScriptU8(monAnim, &v4);

    if (v4 == 20) {
        sub_02016320(monAnim, &v0);
        ReadScriptInt(monAnim, &v7);
    } else if (v4 == 21) {
        sub_02016340(monAnim, &v0, &v1);
        v7 = monAnim->unk_24[v1];
    } else {
        GF_ASSERT(0);
    }

    if (v2 == v3) {
        monAnim->unk_24[v0] = v7;
    }
}

static void sub_02016758(PokemonAnim *monAnim)
{
    u8 v0;

    sub_02016320(monAnim, &v0);

    monAnim->scriptPtr += 1;
    monAnim->unk_24[v0] = (int)ReadScriptWord(monAnim->scriptPtr);
}

static void sub_0201677C(PokemonAnim *monAnim)
{
    GF_ASSERT(monAnim->unk_50 == NULL);

    monAnim->scriptPtr += 1;
    monAnim->unk_50 = monAnim->scriptPtr;
    monAnim->unk_48 = (int)ReadScriptWord(monAnim->scriptPtr);
    monAnim->unk_4C = 0;
}

static void sub_020167A0(PokemonAnim *monAnim)
{
    monAnim->unk_4C++;

    if (monAnim->unk_4C >= monAnim->unk_48) {
        monAnim->unk_50 = NULL;
        monAnim->unk_4C = 0;
        monAnim->unk_48 = 0;
    } else {
        monAnim->scriptPtr = monAnim->unk_50;
    }
}

static void sub_020167BC(PokemonAnim *monAnim)
{
    u8 v0;
    int v1;

    ReadScriptInt(monAnim, &v1);
    sub_02016320(monAnim, &v0);
    PokemonSprite_SetAttribute(monAnim->sprite, v1, monAnim->unk_24[v0]);
}

static void sub_020167E8(PokemonAnim *monAnim)
{
    u8 v0;
    int v1;

    ReadScriptInt(monAnim, &v1);
    sub_02016320(monAnim, &v0);
    PokemonSprite_AddAttribute(monAnim->sprite, v1, monAnim->unk_24[v0]);
}

static void sub_02016814(PokemonAnim *monAnim)
{
    int v0;
    int v1;

    ReadScriptInt(monAnim, &v0);

    u8 v2;
    u8 v3;

    ReadScriptU8(monAnim, &v3);

    if (v3 == 20) {
        ReadScriptInt(monAnim, &v1);
    } else if (v3 == 21) {
        sub_02016320(monAnim, &v2);
        v1 = monAnim->unk_24[v2];
    } else {
        GF_ASSERT(0);
    }

    u8 v4;

    ReadScriptU8(monAnim, &v4);

    if (v4 == 22) {
        PokemonSprite_SetAttribute(monAnim->sprite, v0, v1);
    } else if (v4 == 23) {
        PokemonSprite_AddAttribute(monAnim->sprite, v0, v1);
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016894(PokemonAnim *monAnim)
{
    u8 v0;
    int v1;
    int v2;

    sub_02016454(monAnim, &v0, &v1, &v2);
    monAnim->unk_24[v0] = FX_Whole(FX_SinIdx(v1) * v2);
}

static void sub_020168C8(PokemonAnim *monAnim)
{
    u8 v0;
    int v1;
    int v2;

    sub_02016454(monAnim, &v0, &v1, &v2);
    monAnim->unk_24[v0] = FX_Whole(FX_CosIdx(v1) * v2);
}

static void sub_02016900(PokemonAnim *monAnim)
{
    u8 v0;
    u8 v1;

    sub_02016320(monAnim, &v0);
    ReadScriptU8(monAnim, &v1);

    if (v1 == 8) {
        monAnim->unk_60 = monAnim->unk_24[v0];
    } else if (v1 == 9) {
        monAnim->unk_64 = monAnim->unk_24[v0];
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016948(PokemonAnim *monAnim)
{
    u8 v0;
    u8 v1;

    sub_02016320(monAnim, &v0);
    ReadScriptU8(monAnim, &v1);

    if (v1 == 8) {
        monAnim->unk_60 += monAnim->unk_24[v0];
    } else if (v1 == 9) {
        monAnim->unk_64 += monAnim->unk_24[v0];
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016998(PokemonAnim *monAnim)
{
    int *v0;
    int v1;
    u8 v2;

    ReadScriptU8(monAnim, &v2);

    if (v2 == 8) {
        v0 = &monAnim->unk_60;
    } else if (v2 == 9) {
        v0 = &monAnim->unk_64;
    } else if (v2 == 10) {
        v0 = &monAnim->unk_68;
    } else if (v2 == 11) {
        v0 = &monAnim->unk_6C;
    } else if (v2 == 12) {
        v0 = &monAnim->unk_70;
    } else if (v2 == 13) {
        v0 = &monAnim->unk_74;
    } else if (v2 == 14) {
        v0 = &monAnim->unk_78;
    } else {
        GF_ASSERT(0);
    }

    u8 v3;
    u8 v4;

    ReadScriptU8(monAnim, &v4);

    if (v4 == 20) {
        ReadScriptInt(monAnim, &v1);
    } else if (v4 == 21) {
        sub_02016320(monAnim, &v3);
        v1 = monAnim->unk_24[v3];
    } else {
        GF_ASSERT(0);
    }

    u8 v5;

    ReadScriptU8(monAnim, &v5);

    if (v5 == 22) {
        (*v0) = v1;
    } else if (v5 == 23) {
        (*v0) += v1;
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016A60(PokemonAnim *monAnim)
{
    if (monAnim->reverse) {
        PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_X_CENTER, monAnim->originalX - (monAnim->unk_60 + monAnim->unk_68));
    } else {
        PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_X_CENTER, monAnim->originalX + monAnim->unk_60 + monAnim->unk_68);
    }

    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_Y_CENTER, monAnim->originalY + monAnim->unk_64 + monAnim->unk_6C);
}

static void sub_02016AA8(PokemonAnim *monAnim)
{
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_SCALE_X, MON_AFFINE_SCALE(1) + monAnim->unk_70);
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_SCALE_Y, MON_AFFINE_SCALE(1) + monAnim->unk_74);
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_ROTATION_Z, (u16)monAnim->unk_78);

    int v0;

    if (monAnim->unk_1C6 == 27) {
        if (monAnim->unk_74 < 0) {
            sub_02016514(monAnim);
        }
    } else if (monAnim->unk_1C6 == 29) {
        if (monAnim->unk_74 != 0) {
            sub_02016514(monAnim);
        }
    } else if (monAnim->unk_1C6 == 28) {
        return;
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016B10(PokemonAnim *monAnim)
{
    u8 v0;
    u8 v1;

    sub_02016320(monAnim, &v0);

    monAnim->scriptPtr += 1;
    v1 = (int)ReadScriptWord(monAnim->scriptPtr);

    if ((v1 == 8) || (v1 == 10)) {
        monAnim->unk_68 = monAnim->unk_24[v0];
    } else if ((v1 == 9) || (v1 == 11)) {
        monAnim->unk_6C = monAnim->unk_24[v0];
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016B64(PokemonAnim *monAnim)
{
    ReadScriptInt(monAnim, &monAnim->startDelay);
    monAnim->unk_18 = 1;
}

static void sub_02016B78(PokemonAnim *monAnim)
{
    u8 v0, v1, v2;
    int v3;

    ReadScriptU8(monAnim, &v0);
    ReadScriptU8(monAnim, &v1);
    ReadScriptU8(monAnim, &v2);
    ReadScriptInt(monAnim, &v3);
    PokemonSprite_StartFade(monAnim->sprite, v0, v1, v2, v3);
}

static void sub_02016BB8(PokemonAnim *monAnim)
{
    u8 v0, v1, v2;

    if (PokemonSprite_IsFadeActive(monAnim->sprite)) {
        monAnim->fadeActive = TRUE;
        monAnim->unk_18 = 1;
    }
}

static void sub_02016BD4(PokemonAnim *monAnim)
{
    monAnim->unk_1C5 = 1;
}

static void sub_02016BE0(PokemonAnim *monAnim)
{
    ReadScriptU8(monAnim, &monAnim->unk_1C6);
    GF_ASSERT((monAnim->unk_1C6 == 27) || (monAnim->unk_1C6 == 29) || ((monAnim->unk_1C6 == 28) && TRUE));
}

static void sub_02016C18(PokemonAnim *monAnim)
{
    sub_02016D04(monAnim, 0);
}

static void sub_02016C24(PokemonAnim *monAnim)
{
    sub_02016D04(monAnim, 1);
}

static void sub_02016C30(PokemonAnim *monAnim)
{
    sub_02016D04(monAnim, 2);
}

static void sub_02016C3C(PokemonAnim *monAnim)
{
    sub_02016D04(monAnim, 3);
}

static void sub_02016C48(PokemonAnim *monAnim)
{
    sub_02016D04(monAnim, 4);
}

static void sub_02016C54(const u8 param0, const int *param1, const int *param2, int *param3)
{
    if (param0 == 24) {
        (*param3) = (*param2);
    } else if (param0 == 25) {
        (*param3) = (*param1) + (*param2);
    } else if (param0 == 26) {
        (*param3) += (*param2);
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016C84(const u8 param0, MotionFuncData *param1, PokemonAnim *param2)
{
    switch (param0) {
    case 35:
        param1->unk_24 = &param1->unk_3C;
        param1->unk_28 = &param2->unk_68;
        param1->unk_30 = param2->unk_68;
        break;
    case 36:
        param1->unk_24 = &param1->unk_40;
        param1->unk_28 = &param2->unk_6C;
        param1->unk_30 = param2->unk_6C;
        break;
    case 37:
        param1->unk_24 = &param1->unk_44;
        param1->unk_28 = &param2->unk_70;
        param1->unk_30 = param2->unk_70;
        break;
    case 38:
        param1->unk_24 = &param1->unk_48;
        param1->unk_28 = &param2->unk_74;
        param1->unk_30 = param2->unk_74;
        break;
    case 39:
        param1->unk_24 = &param1->unk_4C;
        param1->unk_28 = &param2->unk_78;
        param1->unk_30 = param2->unk_78;
        break;
    default:
        GF_ASSERT(FALSE);
    }
}

static void sub_02016D04(PokemonAnim *monAnim, const int param1)
{
    MotionFuncData *motionData = MotionFuncData_New(monAnim, param1);

    ReadScriptU8(monAnim, &motionData->unk_2C);
    ReadScriptU8(monAnim, &motionData->startDelay);

    for (u8 i = 0; i < sMotionFuncToParams[param1].paramCount; i++) {
        ReadScriptInt(monAnim, &motionData->unk_04[i]);
    }

    int v2 = sMotionFuncToParams[param1].unk_08;
    sub_02016C84(motionData->unk_04[v2], motionData, monAnim);

    if (motionData->startDelay == 0) {
        motionData->motionFunc(motionData, monAnim);
    } else {
        motionData->startDelay--;
    }
}

static void sub_02016DAC(MotionFuncData *motionData, PokemonAnim *monAnim)
{
    u16 v0;
    int *v1 = motionData->unk_04;
    v0 = (v1[3] * (v1[6] + 1)) + v1[4];

    switch (v1[0]) {
    case 30:
        (*motionData->unk_24) = FX_Whole(FX_SinIdx(v0) * v1[2]);
        break;
    case 31:
        (*motionData->unk_24) = FX_Whole(FX_CosIdx(v0) * v1[2]);
        break;
    case 32:
        (*motionData->unk_24) = -FX_Whole(FX_SinIdx(v0) * v1[2]);
        break;
    case 33:
        (*motionData->unk_24) = -FX_Whole(FX_CosIdx(v0) * v1[2]);
        break;
    default:
        GF_ASSERT(FALSE);
    }

    sub_02016C54(motionData->unk_2C, &(motionData->unk_30), motionData->unk_24, motionData->unk_28);

    v1[6]++;

    if (v1[6] >= v1[5]) {
        motionData->active = 0;
    }
}

static void sub_02016E64(MotionFuncData *motionData, PokemonAnim *monAnim)
{
    u16 v0;
    int *v1 = motionData->unk_04;
    v0 = ((v1[3] * (v1[6] + 1)) / v1[5]) + v1[4];

    switch (v1[0]) {
    case 30:
        (*motionData->unk_24) = FX_Whole(FX_SinIdx(v0) * v1[2]);
        break;
    case 31:
        (*motionData->unk_24) = FX_Whole(FX_CosIdx(v0) * v1[2]);
        break;
    case 32:
        (*motionData->unk_24) = -FX_Whole(FX_SinIdx(v0) * v1[2]);
        break;
    case 33:
        (*motionData->unk_24) = -FX_Whole(FX_CosIdx(v0) * v1[2]);
        break;
    default:
        GF_ASSERT(FALSE);
    }

    sub_02016C54(motionData->unk_2C, &(motionData->unk_30), motionData->unk_24, motionData->unk_28);

    v1[6]++;

    if (v1[6] >= v1[5]) {
        motionData->active = 0;
    }
}

static void sub_02016F24(MotionFuncData *motionData, PokemonAnim *monAnim)
{
    int v0;
    int *v1 = motionData->unk_04;
    v0 = v1[1] + (v1[2] * v1[4]);

    (*motionData->unk_24) += v0;

    sub_02016C54(motionData->unk_2C, &(motionData->unk_30), motionData->unk_24, motionData->unk_28);

    v1[4]++;

    if (v1[4] >= v1[3]) {
        motionData->active = 0;
    }
}

static void sub_02016F60(MotionFuncData *motionData, PokemonAnim *monAnim)
{
    int v0;
    int *v1 = motionData->unk_04;
    v0 = ((v1[3] + 1) * v1[1]) / v1[2];

    (*motionData->unk_24) = v0;

    sub_02016C54(motionData->unk_2C, &(motionData->unk_30), motionData->unk_24, motionData->unk_28);

    v1[3]++;

    if (v1[3] >= v1[2]) {
        motionData->active = 0;
    }
}

static void sub_02016F9C(MotionFuncData *motionData, PokemonAnim *monAnim)
{
    int v0;
    int *v1 = motionData->unk_04;
    v0 = v1[1] + (v1[2] * v1[4]);

    (*motionData->unk_24) += v0;

    if ((motionData->unk_2C == 24) || (motionData->unk_2C == 26)) {
        if (v0 < 0) {
            if ((*motionData->unk_24) <= v1[3]) {
                (*motionData->unk_24) = v1[3];
                motionData->active = 0;
            }
        } else {
            if ((*motionData->unk_24) >= v1[3]) {
                (*motionData->unk_24) = v1[3];
                motionData->active = 0;
            }
        }
    } else if (motionData->unk_2C == 25) {
        int v2 = motionData->unk_30 + (*motionData->unk_24);

        if (v0 < 0) {
            if (v2 <= v1[3]) {
                (*motionData->unk_24) += (v1[3] - v2);
                motionData->active = 0;
            }
        } else {
            if (v2 >= v1[3]) {
                (*motionData->unk_24) -= (v2 - v1[3]);
                motionData->active = 0;
            }
        }
    } else {
        GF_ASSERT(0);
    }

    sub_02016C54(motionData->unk_2C, &(motionData->unk_30), motionData->unk_24, motionData->unk_28);

    v1[4]++;
}
