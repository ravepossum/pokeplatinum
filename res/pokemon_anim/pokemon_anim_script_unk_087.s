#include "macros/pokemon_anim_cmd.inc"
#include "generated/pokemon_anim_constants.h"
#include "generated/pokemon_sprite_constants.h"

PokemonAnimScript_unk_087:
    UpdateSpriteAttribute MON_SPRITE_X_PIVOT, ANIM_READ_TYPE_VALUE2, 4294967264, ANIM_ATTRIBUTE_SET
    TransformCurveEven TRANSFORM_CALC_SET, 0, TRANSFORM_CURVE_SIN, TRANSFORM_TYPE_ROTATION_Z, 2048, 32768, 0, 30
    WaitTransform
    End
