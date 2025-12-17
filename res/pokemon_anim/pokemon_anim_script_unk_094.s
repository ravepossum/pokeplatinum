#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_094:
    TransformLinearBounded TRANSFORM_CALC_SET, 0, TRANSFORM_TYPE_OFFSET_X, 4294967295, 0, 4294967290
    WaitTransform
    TransformLinearBounded TRANSFORM_CALC_ADD, 0, TRANSFORM_TYPE_OFFSET_X, 0, 1, 6
    WaitTransform
    UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VALUE2, 6, ANIM_ATTRIBUTE_SET
    ApplyTranslation
    SetVar 0, 2
    Loop 6
        UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VAR2, 0, ANIM_ATTRIBUTE_ADD
        ApplyTranslation
        Multiply 0, ANIM_READ_TYPE_VALUE, 0, 4294967295
        WaitFrame
    LoopEnd
    UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VALUE2, 6, ANIM_ATTRIBUTE_SET
    ApplyTranslation
    TransformLinearBounded TRANSFORM_CALC_ADD, 0, TRANSFORM_TYPE_OFFSET_X, 4294967295, 0, 0
    WaitTransform
    End
