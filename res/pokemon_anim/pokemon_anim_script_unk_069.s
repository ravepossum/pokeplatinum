#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_069:
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_Y, 60, 32768, 0, 15
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_X, 60, 32768, 0, 15
    TransformCurveEven TRANSFORM_CALC_INCREMENT, 4, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_Y, 12, 262144, 0, 7
    TransformCurveEven TRANSFORM_CALC_INCREMENT, 4, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_SCALE_X, 12, 262144, 0, 7
    WaitTransform
    End
