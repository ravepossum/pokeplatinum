    .include "macros/scrcmd.inc"

    .data

    .long _004E-.-4
    .long _005A-.-4
    .long _0066-.-4
    .long _0072-.-4
    .long _007E-.-4
    .long _008A-.-4
    .long _0096-.-4
    .long _00A2-.-4
    .long _00AE-.-4
    .long _00BA-.-4
    .long _00C6-.-4
    .long _00D2-.-4
    .long _00DE-.-4
    .long _00EA-.-4
    .long _00F6-.-4
    .long _0129-.-4
    .long _0135-.-4
    .long _0141-.-4
    .long _014D-.-4
    .short 0xFD13

_004E:
    ScrCmd_028 0x8000, 1
    ScrCmd_016 _0159

_005A:
    ScrCmd_028 0x8000, 2
    ScrCmd_016 _0159

_0066:
    ScrCmd_028 0x8000, 3
    ScrCmd_016 _0159

_0072:
    ScrCmd_028 0x8000, 4
    ScrCmd_016 _0159

_007E:
    ScrCmd_028 0x8000, 5
    ScrCmd_016 _0159

_008A:
    ScrCmd_028 0x8000, 6
    ScrCmd_016 _0159

_0096:
    ScrCmd_028 0x8000, 7
    ScrCmd_016 _0159

_00A2:
    ScrCmd_028 0x8000, 8
    ScrCmd_016 _0159

_00AE:
    ScrCmd_028 0x8000, 9
    ScrCmd_016 _0159

_00BA:
    ScrCmd_028 0x8000, 10
    ScrCmd_016 _0159

_00C6:
    ScrCmd_028 0x8000, 11
    ScrCmd_016 _0159

_00D2:
    ScrCmd_028 0x8000, 12
    ScrCmd_016 _0159

_00DE:
    ScrCmd_028 0x8000, 13
    ScrCmd_016 _0159

_00EA:
    ScrCmd_028 0x8000, 14
    ScrCmd_016 _0159

_00F6:
    ScrCmd_011 0x4032, 1
    ScrCmd_01C 1, _0129
    ScrCmd_011 0x4032, 2
    ScrCmd_01C 1, _0135
    ScrCmd_011 0x4032, 3
    ScrCmd_01C 1, _0141
    ScrCmd_028 0x8000, 15
    ScrCmd_016 _0159

_0129:
    ScrCmd_028 0x8000, 16
    ScrCmd_016 _0159

_0135:
    ScrCmd_028 0x8000, 17
    ScrCmd_016 _0159

_0141:
    ScrCmd_028 0x8000, 18
    ScrCmd_016 _0159

_014D:
    ScrCmd_028 0x8000, 19
    ScrCmd_016 _0159

_0159:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_238 0x8000, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _02A1
    ScrCmd_02C 0
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0190
    ScrCmd_016 _02AE
    ScrCmd_002

_0190:
    ScrCmd_237 0, 0x8000, 0x8004, 0x8005
    ScrCmd_1FB 0x8004, 0x8005
    ScrCmd_011 0x8000, 10
    ScrCmd_01C 1, _01F0
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_034
    ScrCmd_243 0, 0x800C, 0x8004
    ScrCmd_0A1
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _02AE
    ScrCmd_237 1, 0x8000, 0x8004, 0x8005
    ScrCmd_016 _02A1
    ScrCmd_002

_01F0:
    ScrCmd_045 1, 1, 0, 1, 0x8004
    ScrCmd_046 22, 0xFF, 1
    ScrCmd_046 23, 0xFF, 2
    ScrCmd_046 24, 0xFF, 3
    ScrCmd_046 25, 0xFF, 4
    ScrCmd_046 26, 0xFF, 5
    ScrCmd_046 27, 0xFF, 6
    ScrCmd_046 28, 0xFF, 7
    ScrCmd_046 29, 0xFF, 8
    ScrCmd_046 30, 0xFF, 9
    ScrCmd_046 31, 0xFF, 10
    ScrCmd_046 32, 0xFF, 11
    ScrCmd_046 33, 0xFF, 12
    ScrCmd_046 34, 0xFF, 13
    ScrCmd_046 35, 0xFF, 14
    ScrCmd_046 36, 0xFF, 15
    ScrCmd_046 37, 0xFF, 16
    ScrCmd_046 38, 0xFF, 17
    ScrCmd_047
    ScrCmd_011 0x8004, 0xFFFE
    ScrCmd_01C 1, _02AE
    ScrCmd_237 1, 0x8000, 0x8004, 0x8005
    ScrCmd_016 _02A1
    ScrCmd_002

_02A1:
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_016 _02BB
    ScrCmd_002

_02AE:
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_016 _02BB
    ScrCmd_002

_02BB:
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

    .byte 0
    .byte 0
    .byte 0
