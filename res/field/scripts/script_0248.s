    .include "macros/scrcmd.inc"

    .data

    .long _000A-.-4
    .long _00D1-.-4
    .short 0xFD13

_000A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_020 0xAB4
    ScrCmd_01C 1, _00BC
    ScrCmd_02C 0
    ScrCmd_1B7 0x8004, 4
    ScrCmd_029 0x8008, 0x8004
    ScrCmd_011 0x8008, 0
    ScrCmd_01C 1, _005B
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _0069
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _0077
    ScrCmd_016 _0085
    ScrCmd_002

_005B:
    ScrCmd_028 0x8004, 72
    ScrCmd_016 _0093
    ScrCmd_002

_0069:
    ScrCmd_028 0x8004, 73
    ScrCmd_016 _0093
    ScrCmd_002

_0077:
    ScrCmd_028 0x8004, 74
    ScrCmd_016 _0093
    ScrCmd_002

_0085:
    ScrCmd_028 0x8004, 75
    ScrCmd_016 _0093
    ScrCmd_002

_0093:
    ScrCmd_028 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _00C7
    ScrCmd_01E 0xAB4
    ScrCmd_014 0x7E0
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00BC:
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00C7:
    ScrCmd_014 0x7E1
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00D1:
    ScrCmd_036 2, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002
