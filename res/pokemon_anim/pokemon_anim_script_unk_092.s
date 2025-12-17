#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_092:
    TransformLinearBounded TRANSFORM_CALC_SET, 0, TRANSFORM_TYPE_OFFSET_X, 4294967294, 0, 4294967284
    WaitTransform
    TransformLinearBounded TRANSFORM_CALC_ADD, 0, TRANSFORM_TYPE_OFFSET_X, 4, 1, 12
    WaitTransform
    UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VALUE2, 12, ANIM_ATTRIBUTE_SET
    ApplyTranslation
    SetVar 0, 4
    Loop 15
        UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VAR2, 0, ANIM_ATTRIBUTE_ADD
        ApplyTranslation
        Multiply 0, ANIM_READ_TYPE_VALUE, 0, 4294967295
        WaitFrame
    LoopEnd
    UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VALUE2, 12, ANIM_ATTRIBUTE_SET
    ApplyTranslation
    TransformLinearBounded TRANSFORM_CALC_ADD, 0, TRANSFORM_TYPE_OFFSET_X, 4294967294, 0, 0
    WaitTransform
    End
