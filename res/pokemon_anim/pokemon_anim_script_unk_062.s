#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_062:
    Loop 2
        TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_Y, 70, 32768, 0, 10
        TransformCurveEven TRANSFORM_CALC_INCREMENT, 2, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_Y, 14, 262144, 0, 6
        WaitTransform
    LoopEnd
    End
