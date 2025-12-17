import io
import struct
import sys
from enum import IntEnum

class AnimCmd(IntEnum):
    END = 0
    WAITFRAME = 1
    SETORIGINALPOSITION = 2
    SETVARIF = 3
    SETVAR = 4
    COPYVAR = 5
    ADD = 6
    MULTIPLY = 7
    SUBTRACT = 8
    DIVIDE = 9
    MODULO = 10
    LOOP = 11
    LOOPEND = 12
    SETSPRITEATTRIBUTE = 13
    ADDSPRITEATTRIBUTE = 14
    UPDATESPRITEATTRIBUTE = 15
    SIN = 16
    COS = 17
    SETTRANSLATION = 18
    ADDTRANSLATION = 19
    UPDATEATTRIBUTE = 20
    APPLYTRANSLATION = 21
    APPLYSCALEANDROTATION = 22
    SETOFFSET = 23
    WAITTRANSFORM = 24
    SETYNORMALIZATION = 25
    TRANSFORMCURVE = 26
    TRANSFORMCURVEEVEN = 27
    TRANSFORMLINEAR = 28
    TRANSFORMLINEAREVEN = 29
    TRANSFORMLINEARBOUNDED = 30
    SETSTARTDELAY = 31
    FADE = 32
    WAITFADE = 33
    
cmd_names = {
    0: "End",
    1: "WaitFrame",
    2: "SetOriginalPosition",
    3: "SetVarIf",
    4: "SetVar",
    5: "CopyVar",
    6: "Add",
    7: "Multiply",
    8: "Subtract",
    9: "Divide",
    10: "Modulo",
    11: "Loop",
    12: "LoopEnd",
    13: "SetSpriteAttribute",
    14: "AddSpriteAttribute",
    15: "UpdateSpriteAttribute",
    16: "Sin",
    17: "Cos",
    18: "SetTranslation",
    19: "AddTranslation",
    20: "UpdateAttribute",
    21: "ApplyTranslation",
    22: "ApplyScaleAndRotation",
    23: "SetOffset",
    24: "WaitTransform",
    25: "SetYNormalization",
    26: "TransformCurve",
    27: "TransformCurveEven",
    28: "TransformLinear",
    29: "TransformLinearEven",
    30: "TransformLinearBounded",
    31: "SetStartDelay",
    32: "Fade",
    33: "WaitFade",
}
cmd_param_counts = {
    0: 0,
    1: 0,
    2: 0,
    3: 7,
    4: 2,
    5: 2,
    6: 4,
    7: 4,
    8: 5,
    9: 5,
    10: 5,
    11: 1,
    12: 0,
    13: 2,
    14: 2,
    15: 4,
    16: 6,
    17: 6,
    18: 2,
    19: 2,
    20: 4,
    21: 0,
    22: 0,
    23: 2,
    24: 0,
    25: 1,
    26: 8,
    27: 8,
    28: 6,
    29: 5,
    30: 6,
    31: 1,
    32: 4,
    33: 0,
}
y_norm_params = {
    27: "Y_NORMALIZATION_NEGATIVE_SCALE",
    28: "Y_NORMALIZATION_OFF",
    29: "Y_NORMALIZATION_ON",
}
transform_calc_params = {
    24: "TRANSFORM_CALC_SET",
    25: "TRANSFORM_CALC_ADD",
    26: "TRANSFORM_CALC_INCREMENT",
}
transform_curve_params = {
    30: "TRANSFORM_CURVE_SIN",
    31: "TRANSFORM_CURVE_COS",
    32: "TRANSFORM_CURVE_NEGATIVE_SIN",
    33: "TRANSFORM_CURVE_NEGATIVE_COS",
}
transform_type_params = {
    35: "TRANSFORM_TYPE_OFFSET_X",
    36: "TRANSFORM_TYPE_OFFSET_Y",
    37: "TRANSFORM_TYPE_SCALE_X",
    38: "TRANSFORM_TYPE_SCALE_Y",
    39: "TRANSFORM_TYPE_ROTATION_Z",
}
read_type_params = {
    18: "ANIM_READ_TYPE_VALUE",
    19: "ANIM_READ_TYPE_VAR",
    20: "ANIM_READ_TYPE_VALUE2",
    21: "ANIM_READ_TYPE_VAR2"
}
comparison_result_params = {
    15: "COMPARISON_RESULT_LESS_THAN",
    16: "COMPARISON_RESULT_GREATER_THAN",
    17: "COMPARISON_RESULT_EQUAL",
}
anim_attribute_params = {
    8: "ANIM_TRANSLATE_X",
    9: "ANIM_TRANSLATE_Y",
    10: "ANIM_OFFSET_X",
    11: "ANIM_OFFSET_Y",
    12: "ANIM_SCALE_X",
    13: "ANIM_SCALE_Y",
    14: "ANIM_ROTATION_Z",
}
anim_attribute_update_params = {
    22: "ANIM_ATTRIBUTE_SET",
    23: "ANIM_ATTRIBUTE_ADD",
}
sprite_attribute_params = {
    0: "MON_SPRITE_X_CENTER",
    1: "MON_SPRITE_Y_CENTER",
    10: "MON_SPRITE_X_PIVOT",
    12: "MON_SPRITE_SCALE_X",
    13: "MON_SPRITE_SCALE_Y",
    19: "MON_SPRITE_SHADOW_X",
}

def sub_param_name(param: int) -> str:
    if (curr_cmd == AnimCmd.SETYNORMALIZATION):
        return y_norm_params[param]
        
    if (curr_cmd == AnimCmd.MULTIPLY):
        if (param_count == 3):
            return read_type_params[param]

    if (curr_cmd == AnimCmd.DIVIDE or curr_cmd == AnimCmd.MODULO):
        if (param_count == 3 or param_count == 4):
            return read_type_params[param]

    if (curr_cmd == AnimCmd.ADD):
        if (param_count == 3):
            return read_type_params[param]

    if (curr_cmd == AnimCmd.SETOFFSET):
        if (param_count == 1):
            return anim_attribute_params[param]

    if (curr_cmd == AnimCmd.SUBTRACT):
        if (param_count == 3 or param_count == 4):
            return read_type_params[param]

    if (curr_cmd == AnimCmd.SIN or curr_cmd == AnimCmd.COS):
        if (param_count == 2 or param_count == 4):
            return read_type_params[param]

    if (curr_cmd == AnimCmd.UPDATEATTRIBUTE):
        if (param_count == 3):
            return read_type_params[param]
        if (param_count == 4):
            return anim_attribute_params[param]
        if (param_count == 1):
            return anim_attribute_update_params[param]

    if (curr_cmd == AnimCmd.UPDATESPRITEATTRIBUTE):
        if (param_count == 4):
            return sprite_attribute_params[param]
        if (param_count == 3):
            return read_type_params[param]
        if (param_count == 1):
            return anim_attribute_update_params[param]

    if (curr_cmd == AnimCmd.SETSPRITEATTRIBUTE or curr_cmd == AnimCmd.ADDSPRITEATTRIBUTE):
        if (param_count == 2):
            return sprite_attribute_params[param]

    if (curr_cmd == AnimCmd.SETVARIF):
        if (param_count == 7 or param_count == 3):
            return read_type_params[param]
        if (param_count == 4):
            return comparison_result_params[param]

    if (curr_cmd == AnimCmd.TRANSFORMCURVEEVEN or curr_cmd == AnimCmd.TRANSFORMCURVE):
        if (param_count == 8):
            return transform_calc_params[param]
        if (param_count == 6):
            return transform_curve_params[param]
        if (param_count == 5):
            return transform_type_params[param]
        
    if (curr_cmd == AnimCmd.TRANSFORMLINEAREVEN):
        if (param_count == 5):
            return transform_calc_params[param]
        if (param_count == 3):
            return transform_type_params[param]
        
    if (curr_cmd == AnimCmd.TRANSFORMLINEAR):
        if (param_count == 6):
            return transform_calc_params[param]
        if (param_count == 4):
            return transform_type_params[param]
        
    if (curr_cmd == AnimCmd.TRANSFORMLINEARBOUNDED):
        if (param_count == 6):
            return transform_calc_params[param]
        if (param_count == 4):
            return transform_type_params[param]
        
    return str(param)
                
# file_name = 'tmp/00003.bin'
file_name = sys.argv[1]
script_num = file_name[file_name.index('/')+3:-4]
header = "#include \"macros/pokemon_anim_cmd.inc\"\n#include \"generated/pokemon_anim_constants.h\"\n#include \"generated/pokemon_sprite_constants.h\"\n\n"
output = header
indent = 0
out_file_name = 'tmp_out/pokemon_anim_script_unk_' + script_num + '.s'

with open(file_name, mode='rb') as file:
    output += "PokemonAnimScript_unk_" + script_num + ":\n"
    indent += 1
    bytes = file.read(4)

    while len(bytes) != 0:
        param_count = 0
        curr_cmd = struct.unpack("I", bytes)[0]
        
        if (curr_cmd == AnimCmd.LOOPEND):
           indent -= 1 

        output += ("    " * indent) + cmd_names[curr_cmd]

        if (curr_cmd == AnimCmd.LOOP):
           indent += 1 

        param_count = cmd_param_counts[curr_cmd]
        while param_count > 0:
            bytes = file.read(4)
            param = struct.unpack("I", bytes)[0]

            param = sub_param_name(param)

            param_count -= 1
            output += " "
            output += str(param)

            if param_count != 0:
                output += ","
            
        output += '\n'
        bytes = file.read(4)


# print(output)
with open(out_file_name, "w") as text_file:
    text_file.write(output)