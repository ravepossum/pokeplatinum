#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_073:
    SetVar 0, 0
    SetVar 2, 0
    Loop 15
        Add 0, ANIM_READ_TYPE_VALUE, 0, 1
        Add 2, ANIM_READ_TYPE_VALUE, 2, 12288
        Subtract 1, ANIM_READ_TYPE_VALUE, ANIM_READ_TYPE_VAR, 15, 0
        Multiply 1, ANIM_READ_TYPE_VALUE, 1, 6
        Divide 1, ANIM_READ_TYPE_VAR, ANIM_READ_TYPE_VALUE, 1, 15
        Sin 3, 2, ANIM_READ_TYPE_VAR2, 1, ANIM_READ_TYPE_VALUE2, 0
        UpdateAttribute ANIM_OFFSET_Y, ANIM_READ_TYPE_VAR2, 3, ANIM_ATTRIBUTE_SET
        ApplyTranslation
        WaitFrame
    LoopEnd
    SetStartDelay 7
    SetVar 0, 0
    SetVar 2, 0
    Loop 15
        Add 0, ANIM_READ_TYPE_VALUE, 0, 1
        Add 2, ANIM_READ_TYPE_VALUE, 2, 12288
        Subtract 1, ANIM_READ_TYPE_VALUE, ANIM_READ_TYPE_VAR, 15, 0
        Multiply 1, ANIM_READ_TYPE_VALUE, 1, 6
        Divide 1, ANIM_READ_TYPE_VAR, ANIM_READ_TYPE_VALUE, 1, 15
        Sin 3, 2, ANIM_READ_TYPE_VAR2, 1, ANIM_READ_TYPE_VALUE2, 0
        UpdateAttribute ANIM_OFFSET_Y, ANIM_READ_TYPE_VAR2, 3, ANIM_ATTRIBUTE_SET
        ApplyTranslation
        WaitFrame
    LoopEnd
    End
