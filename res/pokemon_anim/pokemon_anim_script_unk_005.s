#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_005:
    TransformCurve TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_OFFSET_X, 3, 30720, 0, 19
    WaitTransform
    End
