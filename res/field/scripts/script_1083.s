    .include "macros/scrcmd.inc"

    .data

    .long _000E-.-4
    .long _02BB-.-4
    .long _02CE-.-4
    .short 0xFD13

_000E:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_020 220
    ScrCmd_01C 1, _0216
    ScrCmd_020 0xAB1
    ScrCmd_01C 1, _02B0
    ScrCmd_020 216
    ScrCmd_01C 0, _004A
    ScrCmd_011 0x40E5, 0
    ScrCmd_01C 1, _0057
    ScrCmd_016 _0081

_004A:
    ScrCmd_01E 216
    ScrCmd_02C 0
    ScrCmd_016 _0060

_0057:
    ScrCmd_02C 1
    ScrCmd_016 _0060

_0060:
    ScrCmd_218 0x800C
    ScrCmd_029 0x40E5, 0x800C
    ScrCmd_219 1
    ScrCmd_0DA 0, 0x40E5, 0, 0
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0081:
    ScrCmd_02C 3
    ScrCmd_21A 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _029F
    ScrCmd_1C0 0x800C, 0x40E5
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0283
    ScrCmd_016 _00AE

_00AE:
    ScrCmd_0DA 0, 0x40E5, 0, 0
    ScrCmd_02C 4
    ScrCmd_1B7 0x800C, 12
    ScrCmd_011 0x800C, 0
    ScrCmd_01D 1, _0189
    ScrCmd_011 0x800C, 1
    ScrCmd_01D 1, _0191
    ScrCmd_011 0x800C, 2
    ScrCmd_01D 1, _0199
    ScrCmd_011 0x800C, 3
    ScrCmd_01D 1, _01A1
    ScrCmd_011 0x800C, 4
    ScrCmd_01D 1, _01A9
    ScrCmd_011 0x800C, 5
    ScrCmd_01D 1, _01B1
    ScrCmd_011 0x800C, 6
    ScrCmd_01D 1, _01B9
    ScrCmd_011 0x800C, 7
    ScrCmd_01D 1, _01C1
    ScrCmd_011 0x800C, 8
    ScrCmd_01D 1, _01C9
    ScrCmd_011 0x800C, 9
    ScrCmd_01D 1, _01D1
    ScrCmd_011 0x800C, 10
    ScrCmd_01D 1, _01D9
    ScrCmd_011 0x800C, 11
    ScrCmd_01D 1, _01E1
    ScrCmd_028 0x8005, 3
    ScrCmd_07D 93, 1, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 5, _01E9
    ScrCmd_01E 0xAB1
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _023F
    ScrCmd_002

_0189:
    ScrCmd_028 0x8004, 2
    ScrCmd_01B

_0191:
    ScrCmd_028 0x8004, 3
    ScrCmd_01B

_0199:
    ScrCmd_028 0x8004, 4
    ScrCmd_01B

_01A1:
    ScrCmd_028 0x8004, 6
    ScrCmd_01B

_01A9:
    ScrCmd_028 0x8004, 7
    ScrCmd_01B

_01B1:
    ScrCmd_028 0x8004, 8
    ScrCmd_01B

_01B9:
    ScrCmd_028 0x8004, 9
    ScrCmd_01B

_01C1:
    ScrCmd_028 0x8004, 10
    ScrCmd_01B

_01C9:
    ScrCmd_028 0x8004, 11
    ScrCmd_01B

_01D1:
    ScrCmd_028 0x8004, 13
    ScrCmd_01B

_01D9:
    ScrCmd_028 0x8004, 14
    ScrCmd_01B

_01E1:
    ScrCmd_028 0x8004, 15
    ScrCmd_01B

_01E9:
    ScrCmd_014 0x7FC
    ScrCmd_028 0x8004, 93
    ScrCmd_028 0x8005, 1
    ScrCmd_014 0x7FC
    ScrCmd_01F 220
    ScrCmd_028 0x40E5, 0
    ScrCmd_01E 0xAB1
    ScrCmd_02C 5
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0216:
    ScrCmd_07D 93, 1, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0251
    ScrCmd_029 0x8004, 0x4117
    ScrCmd_028 0x8005, 3
    ScrCmd_016 _025C
    ScrCmd_002

_023F:
    ScrCmd_029 0x4117, 0x8004
    ScrCmd_01E 220
    ScrCmd_016 _0251
    ScrCmd_002

_0251:
    ScrCmd_02C 6
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_025C:
    ScrCmd_02C 7
    ScrCmd_01F 220
    ScrCmd_028 0x40E5, 0
    ScrCmd_014 0x7FC
    ScrCmd_028 0x8004, 93
    ScrCmd_028 0x8005, 1
    ScrCmd_014 0x7E0
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0283:
    ScrCmd_0DA 0, 0x40E5, 0, 0
    ScrCmd_21A 0x800C
    ScrCmd_0D5 1, 0x800C
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_029F:
    ScrCmd_02C 9
    ScrCmd_028 0x40E5, 0
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_02B0:
    ScrCmd_02C 10
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_02BB:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 11
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_02CE:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_02C 12
    ScrCmd_02C 13
    ScrCmd_040 1, 1, 0, 1, 0x800C
    ScrCmd_29D 0x107, 0
    ScrCmd_29D 0x108, 1
    ScrCmd_29D 0x109, 2
    ScrCmd_29D 0x10B, 3
    ScrCmd_29D 0x10A, 4
    ScrCmd_043
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 0
    ScrCmd_01C 1, _0344
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _034F
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _035A
    ScrCmd_011 0x8008, 3
    ScrCmd_01C 1, _0365
    ScrCmd_016 _0370
    ScrCmd_002

_0344:
    ScrCmd_02C 14
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_034F:
    ScrCmd_02C 15
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_035A:
    ScrCmd_02C 16
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0365:
    ScrCmd_02C 17
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0370:
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

    .byte 0
    .byte 0
