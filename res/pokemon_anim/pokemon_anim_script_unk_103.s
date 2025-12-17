#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_103:
    Fade 0, 12, 0, 992
    WaitFade
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_NEGATIVE_SIN, TRANSFORM_TYPE_OFFSET_X, 6, 65536, 0, 16
    WaitTransform
    Fade 12, 0, 0, 992
    WaitFade
    End
