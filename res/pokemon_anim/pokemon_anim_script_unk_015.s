#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_015:
    SetVar 1, 0
    SetVar 2, 1
    Loop 32
        Add 1, ANIM_READ_TYPE_VALUE, 1, 1024
        Sin 0, 1, ANIM_READ_TYPE_VALUE2, 6, ANIM_READ_TYPE_VALUE2, 0
        Multiply 0, ANIM_READ_TYPE_VAR, 0, 2
        SetOffset 0, ANIM_OFFSET_X
        Multiply 2, ANIM_READ_TYPE_VALUE, 2, 4294967295
        ApplyTranslation
        WaitFrame
    LoopEnd
    End
