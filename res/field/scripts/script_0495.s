    .include "macros/scrcmd.inc"

    .data

    .long _000A-.-4
    .long _008E-.-4
    .short 0xFD13

_000A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_020 218
    ScrCmd_01C 1, _0079
    ScrCmd_02C 0
    ScrCmd_028 0x8004, 92
    ScrCmd_028 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0084
    ScrCmd_014 0x7FC
    ScrCmd_01E 218
    ScrCmd_028 0x8004, 92
    ScrCmd_028 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0079
    ScrCmd_02C 1
    ScrCmd_014 0x7FC
    ScrCmd_016 _0079
    ScrCmd_002

_0079:
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0084:
    ScrCmd_014 0x7E1
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_008E:
    ScrCmd_036 3, 1, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

    .byte 0
    .byte 0
    .byte 0
