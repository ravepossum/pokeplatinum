#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_108:
    TransformLinearEven TRANSFORM_CALC_SET, 0, TRANSFORM_TYPE_OFFSET_X, 4294967280, 6
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_NEGATIVE_SIN, TRANSFORM_TYPE_OFFSET_Y, 2, 98304, 0, 6
    WaitTransform
    TransformLinearEven TRANSFORM_CALC_ADD, 0, TRANSFORM_TYPE_OFFSET_X, 32, 12
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_NEGATIVE_SIN, TRANSFORM_TYPE_OFFSET_Y, 2, 196608, 0, 12
    WaitTransform
    TransformLinearEven TRANSFORM_CALC_ADD, 0, TRANSFORM_TYPE_OFFSET_X, 4294967280, 6
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_NEGATIVE_SIN, TRANSFORM_TYPE_OFFSET_Y, 2, 98304, 0, 6
    WaitTransform
    End
