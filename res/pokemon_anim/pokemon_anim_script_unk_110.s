#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_110:
    UpdateAttribute ANIM_OFFSET_Y, ANIM_READ_TYPE_VALUE2, 8, ANIM_ATTRIBUTE_SET
    ApplyTranslation
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_NEGATIVE_SIN, TRANSFORM_TYPE_OFFSET_X, 12, 262144, 0, 24
    TransformCurveEven TRANSFORM_CALC_ADD, 0, TRANSFORM_CURVE_NEGATIVE_COS, TRANSFORM_TYPE_OFFSET_Y, 8, 262144, 0, 24
    WaitTransform
    End
