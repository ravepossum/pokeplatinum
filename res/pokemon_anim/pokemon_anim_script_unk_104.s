#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_104:
    SetVar 0, 2
    UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VAR2, 0, ANIM_ATTRIBUTE_SET
    ApplyTranslation
    Multiply 0, ANIM_READ_TYPE_VALUE, 0, 4294967295
    Fade 16, 16, 0, 1023
    WaitFrame
    Fade 0, 0, 0, 1023
    WaitFrame
    Loop 7
        UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VAR2, 0, ANIM_ATTRIBUTE_SET
        ApplyTranslation
        Multiply 0, ANIM_READ_TYPE_VALUE, 0, 4294967295
        WaitFrame
    LoopEnd
    UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VAR2, 0, ANIM_ATTRIBUTE_SET
    ApplyTranslation
    Multiply 0, ANIM_READ_TYPE_VALUE, 0, 4294967295
    Fade 16, 16, 0, 1023
    WaitFrame
    Fade 0, 0, 0, 1023
    WaitFrame
    Loop 7
        UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VAR2, 0, ANIM_ATTRIBUTE_SET
        ApplyTranslation
        Multiply 0, ANIM_READ_TYPE_VALUE, 0, 4294967295
        WaitFrame
    LoopEnd
    UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VAR2, 0, ANIM_ATTRIBUTE_SET
    ApplyTranslation
    Multiply 0, ANIM_READ_TYPE_VALUE, 0, 4294967295
    Fade 16, 16, 0, 1023
    WaitFrame
    Fade 0, 0, 0, 1023
    WaitFrame
    Loop 7
        UpdateAttribute ANIM_OFFSET_X, ANIM_READ_TYPE_VAR2, 0, ANIM_ATTRIBUTE_SET
        ApplyTranslation
        Multiply 0, ANIM_READ_TYPE_VALUE, 0, 4294967295
        Fade 16, 16, 0, 1023
        WaitFrame
        Fade 0, 0, 0, 1023
        WaitFrame
    LoopEnd
    End
