    .include "macros/scrcmd.inc"

    .data

    .long _0016-.-4
    .long _0022-.-4
    .long _0035-.-4
    .long _005E-.-4
    .long _007D-.-4
    .short 0xFD13

_0016:
    ScrCmd_028 0x8007, 3
    ScrCmd_014 0x7D2
    ScrCmd_002

_0022:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 0
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0035:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_020 129
    ScrCmd_01C 1, _0053
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0053:
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_005E:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04B 0x5DC
    ScrCmd_04C 0x1AB, 0
    ScrCmd_02C 3
    ScrCmd_04D
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_007D:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_011 0x4002, 1
    ScrCmd_01C 1, _00F4
    ScrCmd_134 5, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _00D9
    ScrCmd_247 0x8000
    ScrCmd_0D0 0, 0x8000
    ScrCmd_02C 6
    ScrCmd_1B9 0x800C, 0x8000
    ScrCmd_011 0x800C, 120
    ScrCmd_01C 4, _00FF
    ScrCmd_011 0x800C, 70
    ScrCmd_01C 4, _010A
    ScrCmd_016 _0115
    ScrCmd_002

_00D9:
    ScrCmd_02C 4
    ScrCmd_028 0x4002, 1
    ScrCmd_028 0x8004, 5
    ScrCmd_014 0x7D9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00F4:
    ScrCmd_02C 5
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00FF:
    ScrCmd_02C 7
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_010A:
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0115:
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002
