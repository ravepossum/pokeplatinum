    .include "macros/scrcmd.inc"

    .data

    .long _000A-.-4
    .long _011C-.-4
    .short 0xFD13

_000A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_020 151
    ScrCmd_01C 1, _0109
    ScrCmd_020 181
    ScrCmd_01C 1, _0037
    ScrCmd_01E 181
    ScrCmd_02C 0
    ScrCmd_016 _0042
    ScrCmd_002

_0037:
    ScrCmd_02C 1
    ScrCmd_016 _0042
    ScrCmd_002

_0042:
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _005B
    ScrCmd_016 _00FE
    ScrCmd_002

_005B:
    ScrCmd_177 0x800C
    ScrCmd_011 0x800C, 6
    ScrCmd_01C 1, _00F3
    ScrCmd_02C 2
    ScrCmd_04E 0x486
    ScrCmd_0CD 0
    ScrCmd_02C 3
    ScrCmd_04F
    ScrCmd_096 137, 25, 0, 0x800C
    ScrCmd_01E 151
    ScrCmd_02C 4
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _00AC
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _00ED
    ScrCmd_002

_00AC:
    ScrCmd_034
    ScrCmd_177 0x4000
    ScrCmd_027 0x4000, 1
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_0BB 0x4000, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01D 5, _00E7
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_061
    ScrCmd_002

_00E7:
    ScrCmd_1E5 49
    ScrCmd_01B

_00ED:
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00F3:
    ScrCmd_02C 5
    ScrCmd_016 _0114
    ScrCmd_002

_00FE:
    ScrCmd_02C 6
    ScrCmd_016 _0114
    ScrCmd_002

_0109:
    ScrCmd_02C 7
    ScrCmd_016 _0114
    ScrCmd_002

_0114:
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_011C:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

    .byte 0
