    .include "macros/scrcmd.inc"

    .data

    .long _0012-.-4
    .long _0025-.-4
    .long _0038-.-4
    .long _004C-.-4
    .short 0xFD13

_0012:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 4
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0025:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 5
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0038:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_0CD 0
    ScrCmd_02C 3
    ScrCmd_034
    ScrCmd_27A
    ScrCmd_061
    ScrCmd_002

_004C:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 0
    ScrCmd_02C 1
    ScrCmd_02C 2
    ScrCmd_034
    ScrCmd_003 15, 0x800C
    ScrCmd_1BD 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0092
    ScrCmd_011 0x800C, 2
    ScrCmd_01C 1, _00AA
    ScrCmd_011 0x800C, 3
    ScrCmd_01C 1, _00C2
    ScrCmd_002

_0092:
    ScrCmd_05E 2, _0104
    ScrCmd_05E 0xFF, _013C
    ScrCmd_05F
    ScrCmd_016 _00D2

_00AA:
    ScrCmd_05E 2, _0118
    ScrCmd_05E 0xFF, _0148
    ScrCmd_05F
    ScrCmd_016 _00D2

_00C2:
    ScrCmd_05E 2, _0118
    ScrCmd_05F
    ScrCmd_016 _00D2

_00D2:
    ScrCmd_168 0, 0, 6, 9, 77
    ScrCmd_16B 77
    ScrCmd_169 77
    ScrCmd_05E 2, _0130
    ScrCmd_05F
    ScrCmd_16C 77
    ScrCmd_169 77
    ScrCmd_16A 77
    ScrCmd_065 2
    ScrCmd_01E 0x162
    ScrCmd_061
    ScrCmd_002

    .balign 4, 0
_0104:
    MoveAction_00F 3
    MoveAction_00D 6
    MoveAction_00E 3
    MoveAction_020
    EndMovement

    .balign 4, 0
_0118:
    MoveAction_00D
    MoveAction_00F 3
    MoveAction_00D 5
    MoveAction_00E 3
    MoveAction_020
    EndMovement

    .balign 4, 0
_0130:
    MoveAction_00C
    MoveAction_045
    EndMovement

    .balign 4, 0
_013C:
    MoveAction_03F
    MoveAction_023
    EndMovement

    .balign 4, 0
_0148:
    MoveAction_03F
    MoveAction_021
    MoveAction_03F
    MoveAction_023
    EndMovement
