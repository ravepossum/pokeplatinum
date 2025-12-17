#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_025:
    SetYNormalization Y_NORMALIZATION_NEGATIVE_SCALE
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_NEGATIVE_SIN, TRANSFORM_TYPE_SCALE_Y, 32, 98304, 0, 24
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_X, 32, 98304, 0, 24
    TransformCurveEven TRANSFORM_CALC_SET, 8, TRANSFORM_CURVE_NEGATIVE_SIN, TRANSFORM_TYPE_OFFSET_Y, 10, 32768, 0, 8
    WaitTransform
    SetYNormalization Y_NORMALIZATION_OFF
    End
