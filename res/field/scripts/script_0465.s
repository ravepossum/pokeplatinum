    .include "macros/scrcmd.inc"

    .data

    .long _000A-.-4
    .long _0045-.-4
    .short 0xFD13

_000A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_134 14, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 5, _003A
    ScrCmd_02C 0
    ScrCmd_028 0x8004, 14
    ScrCmd_014 0x7D9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_003A:
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0045:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_134 14, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 5, _0071
    ScrCmd_016 _0066

_0066:
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0071:
    ScrCmd_02C 3
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002
