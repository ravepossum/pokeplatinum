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

enum AnimCalcType {
    ANIM_CALC_TYPE_VALUE = 18,
    ANIM_CALC_TYPE_VAR,
    ANIM_CALC_TYPE_TRIG_VALUES,
    ANIM_CALC_TYPE_TRIG_VARS,
};

enum YNormalizationType {
    Y_NORMALIZATION_NEGATIVE_SCALE = 27,
    Y_NORMALIZATION_OFF,
    Y_NORMALIZATION_ON,
};

enum TranslationType {
    TRANSLATION_TYPE_X = 8,
    TRANSLATION_TYPE_Y,
};

#define NUM_POKEMON_ANIMS       143
#define NUM_POKEMON_ANIM_FUNCS  34
#define MAX_ANIM_TRANSFORMS     4
#define MAX_POKEMON_ANIM_VARS   8
#define MAX_TRANSFORM_DATA_VARS 8
#define MAX_ANIM_COMMANDS       256

typedef struct TransformData TransformData;
typedef struct PokemonAnim PokemonAnim;

typedef void (*PokemonAnimFunc)(PokemonAnim *);
typedef void (*TransformFunc)(TransformData *, PokemonAnim *);

typedef struct TransformData {
    BOOL active;
    int vars[MAX_TRANSFORM_DATA_VARS];
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
    TransformFunc func;
} TransformData;

typedef struct PokemonAnim {
    PokemonSprite *sprite;
    SysTask *task;
    void *scriptData;
    u32 *scriptPtr;
    BOOL active;
    int animNum;
    int waitFrame;
    int endAnim;
    BOOL completed;
    int vars[MAX_POKEMON_ANIM_VARS];
    int commandCount;
    int unk_48;
    int unk_4C;
    u32 *unk_50;
    int startDelay;
    int originalX;
    int originalY;
    int translateX;
    int translateY;
    int offsetX;
    int offsetY;
    int scaleX;
    int scaleY;
    int rotationZ;
    TransformData transforms[MAX_ANIM_TRANSFORMS];
    u8 reverse;
    u8 waitForTransform;
    u8 yNormalization;
    u8 fadeActive;
} PokemonAnim;

typedef struct PokemonAnimManager {
    PokemonAnim *anims;
    int heapID;
    u8 reverse;
    u8 animCount;
} PokemonAnimManager;

typedef struct {
    TransformFunc func;
    int paramCount;
    int unk_08;
} TransformFuncParameters;

static int ReadScript_WordIndex(u32 *scriptPtr, u8 index, u8 one);
static int ReadScript_WordIndexZero(u32 *scriptPtr, u8 one);
static int ReadScript_Word(u32 *scriptPtr);
static void sub_02016D04(PokemonAnim *monAnim, const int param1);
static void Task_RunPokemonAnim(SysTask *task, void *monAnim);
static void RunPokemonAnimScript(PokemonAnim *monAnim);
static void PokemonAnimFunc_End(PokemonAnim *monAnim);
static void PokemonAnimFunc_WaitFrame(PokemonAnim *monAnim);
static void PokemonAnimFunc_SetOriginalPosition(PokemonAnim *monAnim);
static void sub_02016678(PokemonAnim *monAnim);
static void PokemonAnimFunc_SetVar(PokemonAnim *monAnim);
static void PokemonAnimFunc_CopyVar(PokemonAnim *monAnim);
static void PokemonAnimFunc_Add(PokemonAnim *monAnim);
static void PokemonAnimFunc_Multiply(PokemonAnim *monAnim);
static void PokemonAnimFunc_Subtract(PokemonAnim *monAnim);
static void PokemonAnimFunc_Divide(PokemonAnim *monAnim);
static void PokemonAnimFunc_Modulo(PokemonAnim *monAnim);
static void sub_0201677C(PokemonAnim *monAnim);
static void sub_020167A0(PokemonAnim *monAnim);
static void PokemonAnimFunc_SetSpriteAttribute(PokemonAnim *monAnim);
static void PokemonAnimFunc_AddSpriteAttribute(PokemonAnim *monAnim);
static void sub_02016814(PokemonAnim *monAnim);
static void PokemonAnimFunc_Sin(PokemonAnim *monAnim);
static void PokemonAnimFunc_Cos(PokemonAnim *monAnim);
static void PokemonAnimFunc_SetTranslation(PokemonAnim *monAnim);
static void PokemonAnimFunc_AddTranslation(PokemonAnim *monAnim);
static void sub_02016998(PokemonAnim *monAnim);
static void PokemonAnimFunc_ApplyTranslation(PokemonAnim *monAnim);
static void PokemonAnimFunc_ApplyScaleAndRotation(PokemonAnim *monAnim);
static void sub_02016B10(PokemonAnim *monAnim);
static void PokemonAnimFunc_SetStartDelay(PokemonAnim *monAnim);
static void PokemonAnimFunc_Fade(PokemonAnim *monAnim);
static void PokemonAnimFunc_WaitFade(PokemonAnim *monAnim);
static void PokemonAnimFunc_WaitTransform(PokemonAnim *monAnim);
static void PokemonAnimFunc_SetYNormalization(PokemonAnim *monAnim);
static void sub_02016C18(PokemonAnim *monAnim);
static void sub_02016C24(PokemonAnim *monAnim);
static void sub_02016C30(PokemonAnim *monAnim);
static void sub_02016C3C(PokemonAnim *monAnim);
static void sub_02016C48(PokemonAnim *monAnim);
static void sub_02016DAC(TransformData *transform, PokemonAnim *monAnim);
static void sub_02016E64(TransformData *transform, PokemonAnim *monAnim);
static void sub_02016F24(TransformData *transform, PokemonAnim *monAnim);
static void sub_02016F60(TransformData *transform, PokemonAnim *monAnim);
static void sub_02016F9C(TransformData *transform, PokemonAnim *monAnim);

static const PokemonAnimFunc sPokemonAnimFuncs[NUM_POKEMON_ANIM_FUNCS] = {
    PokemonAnimFunc_End,
    PokemonAnimFunc_WaitFrame,
    PokemonAnimFunc_SetOriginalPosition,
    sub_02016678,
    PokemonAnimFunc_SetVar,
    PokemonAnimFunc_CopyVar,
    PokemonAnimFunc_Add,
    PokemonAnimFunc_Multiply,
    PokemonAnimFunc_Subtract,
    PokemonAnimFunc_Divide,
    PokemonAnimFunc_Modulo,
    sub_0201677C,
    sub_020167A0,
    PokemonAnimFunc_SetSpriteAttribute,
    PokemonAnimFunc_AddSpriteAttribute,
    sub_02016814,
    PokemonAnimFunc_Sin,
    PokemonAnimFunc_Cos,
    PokemonAnimFunc_SetTranslation,
    PokemonAnimFunc_AddTranslation,
    sub_02016998,
    PokemonAnimFunc_ApplyTranslation,
    PokemonAnimFunc_ApplyScaleAndRotation,
    sub_02016B10,
    PokemonAnimFunc_WaitTransform,
    PokemonAnimFunc_SetYNormalization,
    sub_02016C18,
    sub_02016C24,
    sub_02016C30,
    sub_02016C3C,
    sub_02016C48,
    PokemonAnimFunc_SetStartDelay,
    PokemonAnimFunc_Fade,
    PokemonAnimFunc_WaitFade
};

static const TransformFuncParameters sTransformFuncToParams[] = {
    { sub_02016DAC, 6, 0x1 },
    { sub_02016E64, 6, 0x1 },
    { sub_02016F24, 4, 0x0 },
    { sub_02016F60, 3, 0x0 },
    { sub_02016F9C, 4, 0x0 }
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

    monAnimMan->anims[index].scriptData = NARC_AllocAtEndAndReadWholeMemberByIndexPair(NARC_INDEX_POKEANIME__PL_POKE_ANM, monAnimMan->anims[index].animNum, monAnimMan->heapID);
    monAnimMan->anims[index].scriptPtr = (u32 *)monAnimMan->anims[index].scriptData;
    monAnimMan->anims[index].endAnim = FALSE;
    monAnimMan->anims[index].completed = FALSE;
    monAnimMan->anims[index].waitForTransform = FALSE;
    monAnimMan->anims[index].yNormalization = Y_NORMALIZATION_OFF;
    monAnimMan->anims[index].fadeActive = FALSE;
    monAnimMan->anims[index].task = SysTask_Start(Task_RunPokemonAnim, &monAnimMan->anims[index], 0);
    monAnimMan->anims[index].startDelay = startDelay;
    monAnimMan->anims[index].originalX = PokemonSprite_GetAttribute(monSprite, MON_SPRITE_X_CENTER);
    monAnimMan->anims[index].originalY = PokemonSprite_GetAttribute(monSprite, MON_SPRITE_Y_CENTER);
    monAnimMan->anims[index].translateX = 0;
    monAnimMan->anims[index].translateY = 0;
    monAnimMan->anims[index].offsetX = 0;
    monAnimMan->anims[index].offsetY = 0;
    monAnimMan->anims[index].scaleX = 0;
    monAnimMan->anims[index].scaleY = 0;
    monAnimMan->anims[index].rotationZ = 0;
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

        Heap_Free(monAnimMan->anims[index].scriptData);
    }
}

static void Task_RunPokemonAnim(SysTask *task, void *monAnim)
{
    PokemonAnim *anim = (PokemonAnim *)(monAnim);

    if (anim->startDelay == 0) {
        RunPokemonAnimScript(anim);
    } else {
        anim->startDelay--;
    }

    if (anim->endAnim) {
        anim->completed = TRUE;
        anim->active = FALSE;

        SysTask_Done(task);
        anim->task = NULL;
        Heap_Free(anim->scriptData);
    }
}

static void RunPokemonAnimScript(PokemonAnim *monAnim)
{
    monAnim->waitFrame = FALSE;
    monAnim->commandCount = 0;
    u8 inactiveTransforms = 0;

    for (u8 i = 0; i < MAX_ANIM_TRANSFORMS; i++) {
        TransformData *transform = &(monAnim->transforms[i]);

        if (transform->active) {
            if (transform->startDelay == 0) {
                transform->func(transform, monAnim);
            } else {
                transform->startDelay--;
            }
        } else {
            inactiveTransforms++;
        }
    }

    if (inactiveTransforms == MAX_ANIM_TRANSFORMS) {
        monAnim->waitForTransform = FALSE;
    }

    if (monAnim->waitForTransform) {
        PokemonAnimFunc_ApplyTranslation(monAnim);
        PokemonAnimFunc_ApplyScaleAndRotation(monAnim);
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
        monAnim->commandCount++;

        GF_ASSERT(*(monAnim->scriptPtr) < NUM_POKEMON_ANIM_FUNCS);

        PokemonAnimFunc currAnimFunc = sPokemonAnimFuncs[*(monAnim->scriptPtr)];
        currAnimFunc(monAnim);

        if (monAnim->endAnim) {
            break;
        } else {
            monAnim->scriptPtr++;

            if (monAnim->waitFrame) {
                break;
            } else if (monAnim->waitForTransform) {
                PokemonAnimFunc_ApplyTranslation(monAnim);
                PokemonAnimFunc_ApplyScaleAndRotation(monAnim);
                break;
            }
        }

        if (monAnim->commandCount >= MAX_ANIM_COMMANDS) {
            GF_ASSERT(FALSE);

            monAnim->endAnim = TRUE;
            break;
        }
    }
}

static int ReadScript_WordIndex(u32 *scriptPtr, u8 index, u8 one)
{
    int ret = scriptPtr[index];

    if (one != 1) {
        GF_ASSERT(0);
    }

    return ret;
}

static int ReadScript_WordIndexZero(u32 *scriptPtr, u8 one)
{
    return ReadScript_WordIndex(scriptPtr, 0, one);
}

static int ReadScript_Word(u32 *scriptPtr)
{
    return ReadScript_WordIndexZero(scriptPtr, 1);
}

static TransformData *TransformFuncData_New(PokemonAnim *monAnim, const u8 param1)
{
    for (u8 i = 0; i < MAX_ANIM_TRANSFORMS; i++) {
        TransformData *retPtr = &(monAnim->transforms[i]);

        if (retPtr->active == FALSE) {
            MI_CpuClear8(retPtr, sizeof(TransformData));

            retPtr->active = TRUE;
            retPtr->func = sTransformFuncToParams[param1].func;

            return retPtr;
        }
    }

    GF_ASSERT(FALSE);
    return NULL;
}

static void ReadScript_Int(PokemonAnim *monAnim, int *outInt)
{
    monAnim->scriptPtr++;
    *outInt = (int)ReadScript_Word(monAnim->scriptPtr);
}

static void ReadScript_U8(PokemonAnim *monAnim, u8 *outU8)
{
    monAnim->scriptPtr++;
    *outU8 = (u8)ReadScript_Word(monAnim->scriptPtr);
}

static void ReadScript_VarIndex(PokemonAnim *monAnim, u8 *outIndex)
{
    monAnim->scriptPtr++;
    *outIndex = (u8)ReadScript_Word(monAnim->scriptPtr);
    GF_ASSERT(*outIndex < MAX_POKEMON_ANIM_VARS);
}

static void ReadScript_TwoVarIndexes(PokemonAnim *monAnim, u8 *outIndex1, u8 *outIndex2)
{
    ReadScript_VarIndex(monAnim, outIndex1);
    ReadScript_VarIndex(monAnim, outIndex2);
}

static void ReadScript_VarAndIntOrVar(PokemonAnim *monAnim, u8 *outDestIndex, int *outOperand1, int *outOperand2)
{
    u8 index1, index2, calcType;

    ReadScript_VarIndex(monAnim, outDestIndex);
    ReadScript_U8(monAnim, &calcType);

    if (calcType == ANIM_CALC_TYPE_VALUE) {
        ReadScript_VarIndex(monAnim, &index1);
        *outOperand1 = monAnim->vars[index1];
        ReadScript_Int(monAnim, outOperand2);
    } else if (calcType == ANIM_CALC_TYPE_VAR) {
        ReadScript_TwoVarIndexes(monAnim, &index1, &index2);
        *outOperand1 = monAnim->vars[index1];
        *outOperand2 = monAnim->vars[index2];
    } else {
        GF_ASSERT(0);
    }
}

static void ReadScript_TwoIntOrVar(PokemonAnim *monAnim, u8 *outDestIndex, int *outOperand1, int *outOperand2)
{
    u8 index1, index2, calcType1, calcType2;

    ReadScript_VarIndex(monAnim, outDestIndex);
    ReadScript_U8(monAnim, &calcType1);
    ReadScript_U8(monAnim, &calcType2);

    if (calcType1 == ANIM_CALC_TYPE_VALUE) {
        ReadScript_Int(monAnim, outOperand1);
    } else if (calcType1 == ANIM_CALC_TYPE_VAR) {
        ReadScript_VarIndex(monAnim, &index1);
        *outOperand1 = monAnim->vars[index1];
    } else {
        GF_ASSERT(0);
    }

    if (calcType2 == ANIM_CALC_TYPE_VALUE) {
        ReadScript_Int(monAnim, outOperand2);
    } else if (calcType2 == ANIM_CALC_TYPE_VAR) {
        ReadScript_VarIndex(monAnim, &index2);
        *outOperand2 = monAnim->vars[index2];
    } else {
        GF_ASSERT(0);
    }
}

static void ReadScript_TrigOperands(PokemonAnim *monAnim, u8 *outDestIndex, int *outRadians, int *outAmplitude)
{
    u8 radiansIndex, amplitudeIndex, offsetIndex, calcType;
    int radians, offset;

    ReadScript_TwoVarIndexes(monAnim, outDestIndex, &radiansIndex);
    radians = monAnim->vars[radiansIndex];
    ReadScript_U8(monAnim, &calcType);

    if (calcType == ANIM_CALC_TYPE_TRIG_VALUES) {
        ReadScript_Int(monAnim, outAmplitude);
    } else if (calcType == ANIM_CALC_TYPE_TRIG_VARS) {
        ReadScript_VarIndex(monAnim, &amplitudeIndex);
        (*outAmplitude) = monAnim->vars[amplitudeIndex];
    } else {
        GF_ASSERT(0);
    }

    ReadScript_U8(monAnim, &calcType);

    if (calcType == ANIM_CALC_TYPE_TRIG_VALUES) {
        ReadScript_Int(monAnim, &offset);
    } else if (calcType == ANIM_CALC_TYPE_TRIG_VARS) {
        ReadScript_VarIndex(monAnim, &offsetIndex);
        offset = monAnim->vars[offsetIndex];
    } else {
        GF_ASSERT(0);
    }

    (*outRadians) = radians + offset;
    (*outRadians) %= 0x10000;
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

static void ApplyYNormalization(PokemonAnim *monAnim)
{
    int y = (-monAnim->scaleY) / 8;
    PokemonSprite_AddAttribute(monAnim->sprite, MON_SPRITE_Y_CENTER, y);
}

static void PokemonAnimFunc_End(PokemonAnim *monAnim)
{
    PokemonAnimFunc_SetOriginalPosition(monAnim);

    monAnim->waitFrame = TRUE;
    monAnim->endAnim = TRUE;
}

static void PokemonAnimFunc_WaitFrame(PokemonAnim *monAnim)
{
    monAnim->waitFrame = TRUE;
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

static void PokemonAnimFunc_CopyVar(PokemonAnim *monAnim)
{
    u8 destIndex, originIndex;

    ReadScript_TwoVarIndexes(monAnim, &destIndex, &originIndex);
    monAnim->vars[destIndex] = monAnim->vars[originIndex];
}

static void PokemonAnimFunc_Add(PokemonAnim *monAnim)
{
    u8 index;
    int operand1, operand2;

    ReadScript_VarAndIntOrVar(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 + operand2;
}

static void PokemonAnimFunc_Multiply(PokemonAnim *monAnim)
{
    u8 index;
    int operand1, operand2;

    ReadScript_VarAndIntOrVar(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 * operand2;
}

static void PokemonAnimFunc_Subtract(PokemonAnim *monAnim)
{
    u8 index;
    int operand1, operand2;

    ReadScript_TwoIntOrVar(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 - operand2;
}

static void PokemonAnimFunc_Divide(PokemonAnim *monAnim)
{
    u8 index;
    int operand1, operand2;

    ReadScript_TwoIntOrVar(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 / operand2;
}

static void PokemonAnimFunc_Modulo(PokemonAnim *monAnim)
{
    u8 index;
    int operand1, operand2;

    ReadScript_TwoIntOrVar(monAnim, &index, &operand1, &operand2);
    monAnim->vars[index] = operand1 % operand2;
}

static void sub_02016678(PokemonAnim *monAnim)
{
    u8 v0, v1;
    u8 v2, v3;
    u8 v4;

    int v5, v6;

    ReadScript_U8(monAnim, &v4);

    if (v4 == 20) {
        ReadScript_VarIndex(monAnim, &v0);
        v5 = monAnim->vars[v0];
        ReadScript_Int(monAnim, &v6);
    } else if (v4 == 21) {
        ReadScript_TwoVarIndexes(monAnim, &v0, &v1);
        v5 = monAnim->vars[v0];
        v6 = monAnim->vars[v1];
    } else {
        GF_ASSERT(0);
    }

    ReadScript_U8(monAnim, &v2);
    GF_ASSERT(v2 <= 17);

    v3 = sub_020164FC(&v5, &v6);

    int v7;

    ReadScript_U8(monAnim, &v4);

    if (v4 == 20) {
        ReadScript_VarIndex(monAnim, &v0);
        ReadScript_Int(monAnim, &v7);
    } else if (v4 == 21) {
        ReadScript_TwoVarIndexes(monAnim, &v0, &v1);
        v7 = monAnim->vars[v1];
    } else {
        GF_ASSERT(0);
    }

    if (v2 == v3) {
        monAnim->vars[v0] = v7;
    }
}

static void PokemonAnimFunc_SetVar(PokemonAnim *monAnim)
{
    u8 index;

    ReadScript_VarIndex(monAnim, &index);

    monAnim->scriptPtr++;
    monAnim->vars[index] = (int)ReadScript_Word(monAnim->scriptPtr);
}

static void sub_0201677C(PokemonAnim *monAnim)
{
    GF_ASSERT(monAnim->unk_50 == NULL);

    monAnim->scriptPtr++;
    monAnim->unk_50 = monAnim->scriptPtr;
    monAnim->unk_48 = (int)ReadScript_Word(monAnim->scriptPtr);
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

static void PokemonAnimFunc_SetSpriteAttribute(PokemonAnim *monAnim)
{
    u8 index;
    int attribute;

    ReadScript_Int(monAnim, &attribute);
    ReadScript_VarIndex(monAnim, &index);
    PokemonSprite_SetAttribute(monAnim->sprite, attribute, monAnim->vars[index]);
}

static void PokemonAnimFunc_AddSpriteAttribute(PokemonAnim *monAnim)
{
    u8 index;
    int attribute;

    ReadScript_Int(monAnim, &attribute);
    ReadScript_VarIndex(monAnim, &index);
    PokemonSprite_AddAttribute(monAnim->sprite, attribute, monAnim->vars[index]);
}

static void sub_02016814(PokemonAnim *monAnim)
{
    int v0;
    int v1;

    ReadScript_Int(monAnim, &v0);

    u8 v2;
    u8 v3;

    ReadScript_U8(monAnim, &v3);

    if (v3 == 20) {
        ReadScript_Int(monAnim, &v1);
    } else if (v3 == 21) {
        ReadScript_VarIndex(monAnim, &v2);
        v1 = monAnim->vars[v2];
    } else {
        GF_ASSERT(0);
    }

    u8 v4;

    ReadScript_U8(monAnim, &v4);

    if (v4 == 22) {
        PokemonSprite_SetAttribute(monAnim->sprite, v0, v1);
    } else if (v4 == 23) {
        PokemonSprite_AddAttribute(monAnim->sprite, v0, v1);
    } else {
        GF_ASSERT(0);
    }
}

static void PokemonAnimFunc_Sin(PokemonAnim *monAnim)
{
    u8 index;
    int radians, amplitude;

    ReadScript_TrigOperands(monAnim, &index, &radians, &amplitude);
    monAnim->vars[index] = FX_Whole(FX_SinIdx(radians) * amplitude);
}

static void PokemonAnimFunc_Cos(PokemonAnim *monAnim)
{
    u8 index;
    int radians, amplitude;

    ReadScript_TrigOperands(monAnim, &index, &radians, &amplitude);
    monAnim->vars[index] = FX_Whole(FX_CosIdx(radians) * amplitude);
}

static void PokemonAnimFunc_SetTranslation(PokemonAnim *monAnim)
{
    u8 index, translationType;

    ReadScript_VarIndex(monAnim, &index);
    ReadScript_U8(monAnim, &translationType);

    if (translationType == TRANSLATION_TYPE_X) {
        monAnim->translateX = monAnim->vars[index];
    } else if (translationType == TRANSLATION_TYPE_Y) {
        monAnim->translateY = monAnim->vars[index];
    } else {
        GF_ASSERT(0);
    }
}

static void PokemonAnimFunc_AddTranslation(PokemonAnim *monAnim)
{
    u8 index, translationType;

    ReadScript_VarIndex(monAnim, &index);
    ReadScript_U8(monAnim, &translationType);

    if (translationType == TRANSLATION_TYPE_X) {
        monAnim->translateX += monAnim->vars[index];
    } else if (translationType == TRANSLATION_TYPE_Y) {
        monAnim->translateY += monAnim->vars[index];
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016998(PokemonAnim *monAnim)
{
    int *v0;
    int v1;
    u8 v2;

    ReadScript_U8(monAnim, &v2);

    if (v2 == 8) {
        v0 = &monAnim->translateX;
    } else if (v2 == 9) {
        v0 = &monAnim->translateY;
    } else if (v2 == 10) {
        v0 = &monAnim->offsetX;
    } else if (v2 == 11) {
        v0 = &monAnim->offsetY;
    } else if (v2 == 12) {
        v0 = &monAnim->scaleX;
    } else if (v2 == 13) {
        v0 = &monAnim->scaleY;
    } else if (v2 == 14) {
        v0 = &monAnim->rotationZ;
    } else {
        GF_ASSERT(0);
    }

    u8 v3;
    u8 v4;

    ReadScript_U8(monAnim, &v4);

    if (v4 == 20) {
        ReadScript_Int(monAnim, &v1);
    } else if (v4 == 21) {
        ReadScript_VarIndex(monAnim, &v3);
        v1 = monAnim->vars[v3];
    } else {
        GF_ASSERT(0);
    }

    u8 v5;

    ReadScript_U8(monAnim, &v5);

    if (v5 == 22) {
        (*v0) = v1;
    } else if (v5 == 23) {
        (*v0) += v1;
    } else {
        GF_ASSERT(0);
    }
}

static void PokemonAnimFunc_ApplyTranslation(PokemonAnim *monAnim)
{
    if (monAnim->reverse) {
        PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_X_CENTER, monAnim->originalX - (monAnim->translateX + monAnim->offsetX));
    } else {
        PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_X_CENTER, monAnim->originalX + monAnim->translateX + monAnim->offsetX);
    }

    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_Y_CENTER, monAnim->originalY + monAnim->translateY + monAnim->offsetY);
}

static void PokemonAnimFunc_ApplyScaleAndRotation(PokemonAnim *monAnim)
{
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_SCALE_X, MON_AFFINE_SCALE(1) + monAnim->scaleX);
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_SCALE_Y, MON_AFFINE_SCALE(1) + monAnim->scaleY);
    PokemonSprite_SetAttribute(monAnim->sprite, MON_SPRITE_ROTATION_Z, (u16)monAnim->rotationZ);

    if (monAnim->yNormalization == Y_NORMALIZATION_NEGATIVE_SCALE) {
        if (monAnim->scaleY < 0) {
            ApplyYNormalization(monAnim);
        }
    } else if (monAnim->yNormalization == Y_NORMALIZATION_ON) {
        if (monAnim->scaleY != 0) {
            ApplyYNormalization(monAnim);
        }
    } else if (monAnim->yNormalization == Y_NORMALIZATION_OFF) {
        return;
    } else {
        GF_ASSERT(0);
    }
}

static void sub_02016B10(PokemonAnim *monAnim)
{
    u8 v0;
    u8 v1;

    ReadScript_VarIndex(monAnim, &v0);

    monAnim->scriptPtr++;
    v1 = (int)ReadScript_Word(monAnim->scriptPtr);

    if ((v1 == 8) || (v1 == 10)) {
        monAnim->offsetX = monAnim->vars[v0];
    } else if ((v1 == 9) || (v1 == 11)) {
        monAnim->offsetY = monAnim->vars[v0];
    } else {
        GF_ASSERT(0);
    }
}

static void PokemonAnimFunc_SetStartDelay(PokemonAnim *monAnim)
{
    ReadScript_Int(monAnim, &monAnim->startDelay);
    monAnim->waitFrame = TRUE;
}

static void PokemonAnimFunc_Fade(PokemonAnim *monAnim)
{
    u8 initAlpha, targetAlpha, delay;
    int color;

    ReadScript_U8(monAnim, &initAlpha);
    ReadScript_U8(monAnim, &targetAlpha);
    ReadScript_U8(monAnim, &delay);
    ReadScript_Int(monAnim, &color);
    PokemonSprite_StartFade(monAnim->sprite, initAlpha, targetAlpha, delay, color);
}

static void PokemonAnimFunc_WaitFade(PokemonAnim *monAnim)
{
    if (PokemonSprite_IsFadeActive(monAnim->sprite)) {
        monAnim->fadeActive = TRUE;
        monAnim->waitFrame = TRUE;
    }
}

static void PokemonAnimFunc_WaitTransform(PokemonAnim *monAnim)
{
    monAnim->waitForTransform = TRUE;
}

static void PokemonAnimFunc_SetYNormalization(PokemonAnim *monAnim)
{
    ReadScript_U8(monAnim, &monAnim->yNormalization);
    GF_ASSERT(
        monAnim->yNormalization == Y_NORMALIZATION_NEGATIVE_SCALE
        || monAnim->yNormalization == Y_NORMALIZATION_ON
        || (monAnim->yNormalization == Y_NORMALIZATION_OFF
            // it doesn't match without this...
            && TRUE));
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

static void sub_02016C84(const u8 param0, TransformData *transform, PokemonAnim *monAnim)
{
    switch (param0) {
    case 35:
        transform->unk_24 = &transform->unk_3C;
        transform->unk_28 = &monAnim->offsetX;
        transform->unk_30 = monAnim->offsetX;
        break;
    case 36:
        transform->unk_24 = &transform->unk_40;
        transform->unk_28 = &monAnim->offsetY;
        transform->unk_30 = monAnim->offsetY;
        break;
    case 37:
        transform->unk_24 = &transform->unk_44;
        transform->unk_28 = &monAnim->scaleX;
        transform->unk_30 = monAnim->scaleX;
        break;
    case 38:
        transform->unk_24 = &transform->unk_48;
        transform->unk_28 = &monAnim->scaleY;
        transform->unk_30 = monAnim->scaleY;
        break;
    case 39:
        transform->unk_24 = &transform->unk_4C;
        transform->unk_28 = &monAnim->rotationZ;
        transform->unk_30 = monAnim->rotationZ;
        break;
    default:
        GF_ASSERT(FALSE);
    }
}

static void sub_02016D04(PokemonAnim *monAnim, const int param1)
{
    TransformData *transform = TransformFuncData_New(monAnim, param1);

    ReadScript_U8(monAnim, &transform->unk_2C);
    ReadScript_U8(monAnim, &transform->startDelay);

    for (u8 i = 0; i < sTransformFuncToParams[param1].paramCount; i++) {
        ReadScript_Int(monAnim, &transform->vars[i]);
    }

    int v2 = sTransformFuncToParams[param1].unk_08;
    sub_02016C84(transform->vars[v2], transform, monAnim);

    if (transform->startDelay == 0) {
        transform->func(transform, monAnim);
    } else {
        transform->startDelay--;
    }
}

static void sub_02016DAC(TransformData *transform, PokemonAnim *monAnim)
{
    u16 v0;
    int *v1 = transform->vars;
    v0 = (v1[3] * (v1[6] + 1)) + v1[4];

    switch (v1[0]) {
    case 30:
        (*transform->unk_24) = FX_Whole(FX_SinIdx(v0) * v1[2]);
        break;
    case 31:
        (*transform->unk_24) = FX_Whole(FX_CosIdx(v0) * v1[2]);
        break;
    case 32:
        (*transform->unk_24) = -FX_Whole(FX_SinIdx(v0) * v1[2]);
        break;
    case 33:
        (*transform->unk_24) = -FX_Whole(FX_CosIdx(v0) * v1[2]);
        break;
    default:
        GF_ASSERT(FALSE);
    }

    sub_02016C54(transform->unk_2C, &(transform->unk_30), transform->unk_24, transform->unk_28);

    v1[6]++;

    if (v1[6] >= v1[5]) {
        transform->active = 0;
    }
}

static void sub_02016E64(TransformData *transform, PokemonAnim *monAnim)
{
    u16 v0;
    int *v1 = transform->vars;
    v0 = ((v1[3] * (v1[6] + 1)) / v1[5]) + v1[4];

    switch (v1[0]) {
    case 30:
        (*transform->unk_24) = FX_Whole(FX_SinIdx(v0) * v1[2]);
        break;
    case 31:
        (*transform->unk_24) = FX_Whole(FX_CosIdx(v0) * v1[2]);
        break;
    case 32:
        (*transform->unk_24) = -FX_Whole(FX_SinIdx(v0) * v1[2]);
        break;
    case 33:
        (*transform->unk_24) = -FX_Whole(FX_CosIdx(v0) * v1[2]);
        break;
    default:
        GF_ASSERT(FALSE);
    }

    sub_02016C54(transform->unk_2C, &(transform->unk_30), transform->unk_24, transform->unk_28);

    v1[6]++;

    if (v1[6] >= v1[5]) {
        transform->active = 0;
    }
}

static void sub_02016F24(TransformData *transform, PokemonAnim *monAnim)
{
    int v0;
    int *v1 = transform->vars;
    v0 = v1[1] + (v1[2] * v1[4]);

    (*transform->unk_24) += v0;

    sub_02016C54(transform->unk_2C, &(transform->unk_30), transform->unk_24, transform->unk_28);

    v1[4]++;

    if (v1[4] >= v1[3]) {
        transform->active = 0;
    }
}

static void sub_02016F60(TransformData *transform, PokemonAnim *monAnim)
{
    int v0;
    int *v1 = transform->vars;
    v0 = ((v1[3] + 1) * v1[1]) / v1[2];

    (*transform->unk_24) = v0;

    sub_02016C54(transform->unk_2C, &(transform->unk_30), transform->unk_24, transform->unk_28);

    v1[3]++;

    if (v1[3] >= v1[2]) {
        transform->active = 0;
    }
}

static void sub_02016F9C(TransformData *transform, PokemonAnim *monAnim)
{
    int v0;
    int *v1 = transform->vars;
    v0 = v1[1] + (v1[2] * v1[4]);

    (*transform->unk_24) += v0;

    if ((transform->unk_2C == 24) || (transform->unk_2C == 26)) {
        if (v0 < 0) {
            if ((*transform->unk_24) <= v1[3]) {
                (*transform->unk_24) = v1[3];
                transform->active = 0;
            }
        } else {
            if ((*transform->unk_24) >= v1[3]) {
                (*transform->unk_24) = v1[3];
                transform->active = 0;
            }
        }
    } else if (transform->unk_2C == 25) {
        int v2 = transform->unk_30 + (*transform->unk_24);

        if (v0 < 0) {
            if (v2 <= v1[3]) {
                (*transform->unk_24) += (v1[3] - v2);
                transform->active = 0;
            }
        } else {
            if (v2 >= v1[3]) {
                (*transform->unk_24) -= (v2 - v1[3]);
                transform->active = 0;
            }
        }
    } else {
        GF_ASSERT(0);
    }

    sub_02016C54(transform->unk_2C, &(transform->unk_30), transform->unk_24, transform->unk_28);

    v1[4]++;
}
