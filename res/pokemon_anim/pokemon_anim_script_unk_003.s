#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_003:
    SetVar 0, 0
    Loop 40
        Multiply 2, ANIM_READ_TYPE_VALUE, 0, 65536
        Divide 2, ANIM_READ_TYPE_VAR, ANIM_READ_TYPE_VALUE, 2, 40
        Modulo 1, ANIM_READ_TYPE_VAR, ANIM_READ_TYPE_VALUE, 0, 2
        SetVarIf ANIM_READ_TYPE_VALUE2, 1, 0, COMPARISON_RESULT_EQUAL, ANIM_READ_TYPE_VALUE2, 3, 32
        SetVarIf ANIM_READ_TYPE_VALUE2, 1, 1, COMPARISON_RESULT_EQUAL, ANIM_READ_TYPE_VALUE2, 3, 8
        Sin 4, 2, ANIM_READ_TYPE_VAR2, 3, ANIM_READ_TYPE_VALUE2, 0
        Subtract 5, ANIM_READ_TYPE_VALUE, ANIM_READ_TYPE_VAR, 0, 4
        UpdateAttribute ANIM_SCALE_X, ANIM_READ_TYPE_VAR2, 5, ANIM_ATTRIBUTE_SET
        UpdateAttribute ANIM_SCALE_Y, ANIM_READ_TYPE_VAR2, 5, ANIM_ATTRIBUTE_SET
        ApplyScaleAndRotation
        Add 0, ANIM_READ_TYPE_VALUE, 0, 1
        WaitFrame
    LoopEnd
    End
