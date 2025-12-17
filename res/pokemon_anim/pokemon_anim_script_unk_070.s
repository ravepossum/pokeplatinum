#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_070:
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_Y, 40, 32768, 0, 20
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_X, 40, 32768, 0, 20
    TransformCurveEven TRANSFORM_CALC_INCREMENT, 5, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_Y, 8, 262144, 0, 10
    TransformCurveEven TRANSFORM_CALC_INCREMENT, 5, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_X, 8, 262144, 0, 10
    WaitTransform
    End
