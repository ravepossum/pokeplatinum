#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_119:
    Loop 2
        TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_OFFSET_X, 16, 32768, 0, 12
        TransformCurveEven TRANSFORM_CALC_ADD, 0, TRANSFORM_CURVE_NEGATIVE_SIN, TRANSFORM_TYPE_SCALE_X, 64, 32768, 0, 12
        WaitTransform
        TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_OFFSET_X, 16, 32768, 32768, 12
        TransformCurveEven TRANSFORM_CALC_ADD, 0, TRANSFORM_CURVE_NEGATIVE_SIN, TRANSFORM_TYPE_SCALE_X, 64, 32768, 0, 12
        WaitTransform
    LoopEnd
    End
