#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_02061830_decl.h"
#include "struct_decls/struct_02061AB4_decl.h"
#include "overlay101/struct_ov101_021D5D90_decl.h"

#include "field/field_system.h"
#include "struct_defs/struct_0205EC34.h"

#include "heap.h"
#include "unk_0205E7D0.h"
#include "unk_02061804.h"
#include "unk_02063400.h"
#include "overlay005/ov5_021F25C0.h"
#include "overlay005/ov5_021F61BC.h"

typedef struct UnkStruct_0205E884_t {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    int unk_0C;
    u32 unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    int unk_2C;
    LocalMapObject * unk_30;
    UnkStruct_ov101_021D5D90 * unk_34;
    UnkStruct_0205EC34 * unk_38;
    const UnkStruct_0205EC34 * unk_3C;
} PlayerAvatar;

static PlayerAvatar * sub_0205E8FC(void);
static void sub_0205E91C(PlayerAvatar * param0, int param1, int param2, UnkStruct_0205EC34 * param3);
static void sub_0205E988(PlayerAvatar * param0, const UnkStruct_02061830 * param1, int param2, int param3, int param4, int param5);
static LocalMapObject * sub_0205EA64(const UnkStruct_02061830 * param0);
static void sub_0205EB9C(PlayerAvatar * param0, u32 param1);
static void sub_0205EBA4(PlayerAvatar * param0, u32 param1);
static u32 sub_0205EBB0(PlayerAvatar * param0, u32 param1);
static void sub_0205EC08(PlayerAvatar * param0, UnkStruct_0205EC34 * param1);

PlayerAvatar * sub_0205E7D0 (const UnkStruct_02061830 * param0, int param1, int param2, int param3, int param4, int param5, int param6, UnkStruct_0205EC34 * param7)
{
    PlayerAvatar * v0;

    v0 = sub_0205E8FC();
    sub_0205E91C(v0, param4, param5, param7);

    {
        int v1;

        if (param6 == 0) {
            v1 = sub_0205ED6C(param4, param5);
        } else {
            if (param5 == 0) {
                v1 = 0xfc;
            } else {
                v1 = 0xfd;
            }
        }

        sub_0205E988(v0, param0, v1, param3, param1, param2);
    }

    return v0;
}

PlayerAvatar * sub_0205E820 (const UnkStruct_02061830 * param0, UnkStruct_0205EC34 * param1, int param2)
{
    int v0;
    PlayerAvatar * v1;
    LocalMapObject * v2;

    v1 = sub_0205E8FC();
    v0 = sub_0205EC94(param1);

    sub_0205E91C(v1, v0, param2, param1);
    v2 = sub_0205EA64(param0);

    sub_0206291C(v2, sub_0205ED6C(v0, param2));
    sub_020628BC(v2, ((1 << 10) | (1 << 13)));
    sub_020628C4(v2, ((1 << 7) | (1 << 8)));
    sub_02062F90(v2, 1);
    sub_0205EB38(v1, v2);

    return v1;
}

void sub_0205E884 (PlayerAvatar * param0, int param1)
{
    int v0, v1;
    LocalMapObject * v2;
    const UnkStruct_02061830 * v3;

    v2 = Player_LocalMapObject(param0);
    GF_ASSERT(v2 != NULL);
    v3 = sub_02062A40(v2);

    ov5_021F6218(param0);

    if (sub_0205EB74(param0) == 0x2) {
        if (param1 != 9) {
            int v4 = Player_XPos(param0);
            int v5 = Player_ZPos(param0);
            int v6 = Player_Dir(param0);
            UnkStruct_ov101_021D5D90 * v7 = ov5_021F261C(v2, v4, v5, v6, 1);

            sub_0205EC00(param0, v7);
        }
    }
}

void sub_0205E8E0 (PlayerAvatar * param0)
{
    Heap_FreeToHeap(param0);
}

void Player_Delete (PlayerAvatar * param0)
{
    LocalMapObject * v0 = Player_LocalMapObject(param0);

    sub_02061AF4(v0);
    sub_0205E8E0(param0);
}

static PlayerAvatar * sub_0205E8FC (void)
{
    PlayerAvatar * v0;

    v0 = Heap_AllocFromHeap(11, (sizeof(PlayerAvatar)));
    GF_ASSERT(v0 != NULL);
    memset(v0, 0, (sizeof(PlayerAvatar)));

    return v0;
}

static void sub_0205E91C (PlayerAvatar * param0, int param1, int param2, UnkStruct_0205EC34 * param3)
{
    sub_0205EC08(param0, param3);

    sub_0205EB08(param0, 0);
    sub_0205EB10(param0, 0);
    sub_0205EB58(param0, param1);
    sub_0205EB94(param0, param2);
    sub_0205EB8C(param0, 0);
    sub_0205EBC0(param0);
    sub_0205EBDC(param0, -1);
    sub_0205EBE4(param0, -1);
    sub_0205EC20(param0, 0xff, 0);

    sub_0205EF6C(param0, 1);
    sub_0205EFF0(param0, 1);
}

static void sub_0205E988 (PlayerAvatar * param0, const UnkStruct_02061830 * param1, int param2, int param3, int param4, int param5)
{
    LocalMapObject * v0;

    v0 = sub_020619DC(param1, param4, param5, param3, param2, 0x1, 1);
    GF_ASSERT(v0 != NULL);

    sub_0206290C(v0, 0xff);
    sub_0206294C(v0, 0);
    sub_02062954(v0, 0);
    sub_0206295C(v0, 0);
    sub_020629B4(v0, 0, 0);
    sub_020629B4(v0, 0, 1);
    sub_020629B4(v0, 0, 2);
    sub_020629FC(v0, -1);
    sub_02062A04(v0, -1);
    sub_020628BC(v0, ((1 << 10) | (1 << 13)));
    sub_020628C4(v0, ((1 << 7) | (1 << 8)));
    sub_02062F90(v0, 1);

    sub_0205EB38(param0, v0);
}

LocalMapObject * sub_0205EA24 (const UnkStruct_02061830 * param0)
{
    int v0 = 0;
    LocalMapObject * v1 = NULL;

    while (sub_020625B0(param0, &v1, &v0, (1 << 0))) {
        if (sub_02062948(v1) == 0x1) {
            break;
        }
    }

    return v1;
}

static LocalMapObject * sub_0205EA64 (const UnkStruct_02061830 * param0)
{
    LocalMapObject * v0 = sub_0205EA24(param0);

    GF_ASSERT(v0 != NULL);
    return v0;
}

int Player_Dir (PlayerAvatar * const param0)
{
    return sub_0206298C(Player_LocalMapObject(param0));
}

void Player_SetDir (PlayerAvatar * param0, int param1)
{
    sub_02062974(Player_LocalMapObject(param0), param1);
}

int sub_0205EA94 (PlayerAvatar * const param0)
{
    return sub_0206299C(Player_LocalMapObject(param0));
}

int sub_0205EAA0 (PlayerAvatar * const param0)
{
    if (sub_0205F16C(param0) == 1) {
        return Player_Dir(param0);
    }

    return sub_0205EA94(param0);
}

int Player_XPos (PlayerAvatar * const param0)
{
    return sub_02063020(Player_LocalMapObject(param0));
}

int Player_ZPos (PlayerAvatar * const param0)
{
    return sub_02063040(Player_LocalMapObject(param0));
}

int sub_0205EAD4 (PlayerAvatar * const param0)
{
    return sub_02063008(Player_LocalMapObject(param0));
}

int sub_0205EAE0 (PlayerAvatar * const param0)
{
    return sub_02063018(Player_LocalMapObject(param0));
}

void sub_0205EAEC (PlayerAvatar * const param0, VecFx32 * param1)
{
    sub_02063050(Player_LocalMapObject(param0), param1);
}

const VecFx32 * sub_0205EAFC (PlayerAvatar * const param0)
{
    return sub_02063070(sub_0205EB54(param0));
}

void sub_0205EB08 (PlayerAvatar * param0, int param1)
{
    param0->unk_14 = param1;
}

int sub_0205EB0C (const PlayerAvatar * param0)
{
    return param0->unk_14;
}

void sub_0205EB10 (PlayerAvatar * param0, int param1)
{
    param0->unk_18 = param1;
}

int Player_MoveState (const PlayerAvatar * param0)
{
    return param0->unk_18;
}

void sub_0205EB18 (PlayerAvatar * param0, int param1)
{
    LocalMapObject * v0;

    v0 = Player_LocalMapObject(param0);

    if (param1 == 1) {
        sub_020628C4(v0, (1 << 9));
    } else {
        sub_020628BC(v0, (1 << 9));
    }
}

void sub_0205EB38 (PlayerAvatar * param0, LocalMapObject * param1)
{
    param0->unk_30 = param1;
}

LocalMapObject * Player_LocalMapObject (PlayerAvatar * param0)
{
    GF_ASSERT(param0 != NULL);
    GF_ASSERT(param0->unk_30 != NULL);

    return param0->unk_30;
}

const LocalMapObject * sub_0205EB54 (const PlayerAvatar * param0)
{
    return param0->unk_30;
}

void sub_0205EB58 (PlayerAvatar * param0, int param1)
{
    GF_ASSERT(param1 < 0x6);
    param0->unk_1C = param1;

    sub_0205ECA8(param0, param1);
}

int sub_0205EB74 (PlayerAvatar * param0)
{
    GF_ASSERT(param0 != NULL);
    return param0->unk_1C;
}

void sub_0205EB84 (PlayerAvatar * param0, u32 param1)
{
    param0->unk_04 |= param1;
}

void sub_0205EB8C (PlayerAvatar * param0, u32 param1)
{
    param0->unk_04 = param1;
}

u32 sub_0205EB90 (PlayerAvatar * param0)
{
    return param0->unk_04;
}

void sub_0205EB94 (PlayerAvatar * param0, int param1)
{
    param0->unk_20 = param1;
}

int sub_0205EB98 (PlayerAvatar * param0)
{
    return param0->unk_20;
}

static void sub_0205EB9C (PlayerAvatar * param0, u32 param1)
{
    param0->unk_00 |= param1;
}

static void sub_0205EBA4 (PlayerAvatar * param0, u32 param1)
{
    param0->unk_00 &= ~param1;
}

static u32 sub_0205EBB0 (PlayerAvatar * param0, u32 param1)
{
    return param0->unk_00 & param1;
}

int sub_0205EBB8 (PlayerAvatar * param0)
{
    return param0->unk_24;
}

void sub_0205EBBC (PlayerAvatar * param0, int param1)
{
    param0->unk_24 = param1;
}

void sub_0205EBC0 (PlayerAvatar * param0)
{
    param0->unk_24 = 0;
    sub_0205EF98(param0, 0);
}

int sub_0205EBCC (PlayerAvatar * param0, int param1, int param2)
{
    param0->unk_24 += param1;

    if (param0->unk_24 > param2) {
        param0->unk_24 = param2;
    }

    return param0->unk_24;
}

void sub_0205EBDC (PlayerAvatar * param0, int param1)
{
    param0->unk_28 = param1;
}

int sub_0205EBE0 (PlayerAvatar * param0)
{
    return param0->unk_28;
}

void sub_0205EBE4 (PlayerAvatar * param0, int param1)
{
    param0->unk_2C = param1;
}

int sub_0205EBE8 (PlayerAvatar * param0)
{
    return param0->unk_2C;
}

void sub_0205EBEC (PlayerAvatar * param0, int param1, int param2)
{
    sub_0205EBDC(param0, param1);
    sub_0205EBE4(param0, param2);
}

void sub_0205EC00 (PlayerAvatar * param0, UnkStruct_ov101_021D5D90 * param1)
{
    param0->unk_34 = param1;
}

UnkStruct_ov101_021D5D90 * sub_0205EC04 (PlayerAvatar * param0)
{
    return param0->unk_34;
}

static void sub_0205EC08 (PlayerAvatar * param0, UnkStruct_0205EC34 * param1)
{
    param0->unk_38 = param1;
}

UnkStruct_0205EC34 * sub_0205EC0C (PlayerAvatar * param0)
{
    return param0->unk_38;
}

void sub_0205EC10 (PlayerAvatar * param0, u32 param1)
{
    param0->unk_08 = param1;
}

u32 sub_0205EC14 (PlayerAvatar * param0)
{
    return param0->unk_08;
}

void sub_0205EC18 (PlayerAvatar * param0, int param1)
{
    param0->unk_0C = param1;
}

int sub_0205EC1C (PlayerAvatar * param0)
{
    return param0->unk_0C;
}

void sub_0205EC20 (PlayerAvatar * param0, u32 param1, int param2)
{
    sub_0205EC10(param0, param1);
    sub_0205EC18(param0, param2);
}

void sub_0205EC34 (UnkStruct_0205EC34 * param0)
{
    param0->unk_00 = 0;
    param0->unk_02 = 0;
    param0->unk_04 = 0x0;
}

int sub_0205EC40 (UnkStruct_0205EC34 * param0)
{
    if (param0 != NULL) {
        if (param0->unk_02 == 1) {
            return 1;
        }
    }

    return 0;
}

void sub_0205EC54 (UnkStruct_0205EC34 * param0, int param1)
{
    if (param1 == 1) {
        param0->unk_02 = 1;
    } else {
        param0->unk_02 = 0;
    }
}

int sub_0205EC64 (UnkStruct_0205EC34 * param0)
{
    if (param0 == NULL) {
        return 0;
    }

    return param0->unk_00;
}

void sub_0205EC70 (UnkStruct_0205EC34 * param0, int param1)
{
    if (param0 != NULL) {
        param0->unk_00 = param1;
    }
}

void sub_0205EC78 (PlayerAvatar * param0, int param1)
{
    UnkStruct_0205EC34 * v0 = sub_0205EC0C(param0);
    sub_0205EC70(v0, param1);
}

int sub_0205EC88 (PlayerAvatar * param0)
{
    UnkStruct_0205EC34 * v0 = sub_0205EC0C(param0);
    return sub_0205EC64(v0);
}

u32 sub_0205EC94 (UnkStruct_0205EC34 * param0)
{
    if (param0 != NULL) {
        return param0->unk_04;
    }

    return 0x0;
}

void sub_0205ECA0 (UnkStruct_0205EC34 * param0, u32 param1)
{
    if (param0 != NULL) {
        param0->unk_04 = param1;
    }
}

void sub_0205ECA8 (PlayerAvatar * param0, u32 param1)
{
    UnkStruct_0205EC34 * v0 = sub_0205EC0C(param0);
    sub_0205ECA0(v0, param1);
}

void sub_0205ECB8 (PlayerAvatar * param0, const VecFx32 * param1, int param2)
{
    LocalMapObject * v0;

    v0 = Player_LocalMapObject(param0);

    sub_020632D4(v0, param1, param2);
    sub_0205EB08(param0, 0);
    sub_0205EB10(param0, 0);
}

void sub_0205ECE0 (PlayerAvatar * param0, int param1, int param2, int param3)
{
    LocalMapObject * v0;

    v0 = Player_LocalMapObject(param0);

    LocalMapObj_SetPosDir(v0, param1, 0, param2, param3);
    sub_0205EB08(param0, 0);
    sub_0205EB10(param0, 0);
}

void sub_0205ED0C (PlayerAvatar * param0, fx32 param1)
{
    VecFx32 v0;
    LocalMapObject * v1 = Player_LocalMapObject(param0);

    sub_02063050(v1, &v0);
    v0.y = param1;
    sub_02063060(v1, &v0);
}

void sub_0205ED2C (PlayerAvatar * param0, int param1)
{
    LocalMapObject * v0 = Player_LocalMapObject(param0);

    if (param1 == 1) {
        sub_02062E28(v0, 0);
    } else {
        sub_02062E28(v0, 1);
    }
}

void sub_0205ED48 (PlayerAvatar * param0, int param1)
{
    LocalMapObject * v0 = Player_LocalMapObject(param0);

    if (param1 == 1) {
        sub_02062E28(v0, 0);
        sub_020642F8(v0);
    } else {
        sub_02062E28(v0, 1);
    }
}

int sub_0205ED6C (int param0, int param1)
{
    if (param1 == 0) {
        switch (param0) {
        case 0x0:
            return 0x0;
        case 0x1:
            return 0x15;
        case 0x2:
            return 0xb2;
        case 0x10:
            return 0xb0;
        case 0x11:
            return 0xb4;
        case 0x12:
            return 0xba;
        case 0x13:
            return 0xbc;
        case 0x14:
            return 0xc4;
        case 0x15:
            return 0xc6;
        case 0x16:
            return 0xc8;
        case 0x17:
            return 0x100;
        case 0x18:
            return 0xd4;
        case 0x19:
            return 0x102;
        case 0x1a:
            return 0x10c;
        case 0x1b:
            return 0x10e;
        case 0x1c:
            return 0xd2;
        case 0x1d:
            return 0x110;
        case 0x1e:
            return 0x112;
        }
    } else {
        switch (param0) {
        case 0x0:
            return 0x61;
        case 0x1:
            return 0x62;
        case 0x2:
            return 0xb3;
        case 0x10:
            return 0xb1;
        case 0x11:
            return 0xb5;
        case 0x12:
            return 0xbb;
        case 0x13:
            return 0xbd;
        case 0x14:
            return 0xc5;
        case 0x15:
            return 0xc7;
        case 0x16:
            return 0xc9;
        case 0x17:
            return 0x101;
        case 0x18:
            return 0x107;
        case 0x19:
            return 0x103;
        case 0x1a:
            return 0x10d;
        case 0x1b:
            return 0x10f;
        case 0x1c:
            return 0xd3;
        case 0x1d:
            return 0x111;
        case 0x1e:
            return 0x113;
        }
    }

    GF_ASSERT(FALSE);
    return 0x0;
}

u32 sub_0205EED8 (int param0)
{
    switch (param0) {
    case 0x0:
        return 1 << 0;
    case 0x1:
        return 1 << 1;
    case 0x2:
        return 1 << 2;
    case 0x11:
        return 1 << 4;
    case 0x13:
        return 1 << 5;
    case 0x14:
        return 1 << 6;
    case 0x15:
        return 1 << 7;
    case 0x16:
        return 1 << 8;
    }

    GF_ASSERT(0);
    return 1 << 0;
}

PlayerAvatar * sub_0205EF3C (FieldSystem * param0)
{
    return param0->playerAvatar;
}

void sub_0205EF40 (PlayerAvatar * param0, int param1)
{
    if (param1 == 1) {
        sub_0205EB9C(param0, (1 << 0));
    } else {
        sub_0205EBA4(param0, (1 << 0));
    }
}

int sub_0205EF58 (PlayerAvatar * param0)
{
    if (sub_0205EBB0(param0, (1 << 0))) {
        return 1;
    }

    return 0;
}

void sub_0205EF6C (PlayerAvatar * param0, int param1)
{
    if (param1 == 1) {
        sub_0205EB9C(param0, (1 << 1));
    } else {
        sub_0205EBA4(param0, (1 << 1));
    }
}

int sub_0205EF84 (PlayerAvatar * param0)
{
    if (sub_0205EBB0(param0, (1 << 1))) {
        return 1;
    }

    return 0;
}

void sub_0205EF98 (PlayerAvatar * param0, int param1)
{
    if (param1 == 1) {
        sub_0205EB9C(param0, (1 << 2));
    } else {
        sub_0205EBA4(param0, (1 << 2));
    }
}

int sub_0205EFB0 (PlayerAvatar * param0)
{
    if (sub_0205EBB0(param0, (1 << 2))) {
        return 1;
    }

    return 0;
}

void sub_0205EFC4 (PlayerAvatar * param0, int param1)
{
    if (param1 == 1) {
        sub_0205EB9C(param0, (1 << 3));
    } else {
        sub_0205EBA4(param0, (1 << 3));
    }
}

int sub_0205EFDC (PlayerAvatar * param0)
{
    if (sub_0205EBB0(param0, (1 << 3))) {
        return 1;
    }

    return 0;
}

void sub_0205EFF0 (PlayerAvatar * param0, int param1)
{
    if (param1 == 1) {
        sub_0205EB9C(param0, (1 << 4));
    } else {
        sub_0205EBA4(param0, (1 << 4));
    }
}

int sub_0205F008 (PlayerAvatar * param0)
{
    if (sub_0205EBB0(param0, (1 << 4))) {
        return 1;
    }

    return 0;
}

void sub_0205F01C (PlayerAvatar * param0, int param1)
{
    if (param1 == 1) {
        sub_0205EB9C(param0, (1 << 5));
    } else {
        sub_0205EBA4(param0, (1 << 5));
    }
}

int sub_0205F034 (PlayerAvatar * param0)
{
    if (sub_0205EBB0(param0, (1 << 5))) {
        return 1;
    }

    return 0;
}

void sub_0205F048 (PlayerAvatar * param0)
{
    sub_0205EB9C(param0, (1 << 6));
}

void sub_0205F054 (PlayerAvatar * param0)
{
    sub_0205EBA4(param0, (1 << 6));
}

int sub_0205F060 (PlayerAvatar * param0)
{
    if (sub_0205EBB0(param0, (1 << 6))) {
        return 1;
    }

    return 0;
}

void sub_0205F074 (PlayerAvatar * param0, int param1)
{
    if (param1 == 1) {
        sub_0205EB9C(param0, (1 << 7));
    } else {
        sub_0205EBA4(param0, (1 << 7));
    }
}

int sub_0205F08C (PlayerAvatar * param0)
{
    return sub_0205EBB0(param0, (1 << 7));
}

void sub_0205F098 (PlayerAvatar * param0, int param1)
{
    sub_0205EBA4(param0, ((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11) | (1 << 12)));

    switch (param1) {
    case 0:
        return;
    case 1:
        sub_0205EB9C(param0, (1 << 8));
        return;
    case 2:
        sub_0205EB9C(param0, (1 << 9));
        return;
    case 3:
        sub_0205EB9C(param0, (1 << 10));
        return;
    case 4:
        sub_0205EB9C(param0, (1 << 11));
        return;
    case 5:
        sub_0205EB9C(param0, (1 << 12));
        return;
    }

    GF_ASSERT(0);
}

int sub_0205F108 (PlayerAvatar * const param0)
{
    int v0 = 0;
    u32 v1 = sub_0205EBB0(param0, ((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11) | (1 << 12)));

    switch (v1) {
    case (1 << 8):
        v0 = 1;
        break;
    case (1 << 9):
        v0 = 2;
        break;
    case (1 << 10):
        v0 = 3;
        break;
    case (1 << 11):
        v0 = 4;
        break;
    case (1 << 12):
        v0 = 5;
        break;
    }

    return v0;
}

BOOL sub_0205F158 (PlayerAvatar * const param0)
{
    int v0 = sub_0205F108(param0);

    if ((v0 == 0) || (v0 == 1)) {
        return 0;
    }

    return 1;
}

BOOL sub_0205F16C (PlayerAvatar * const param0)
{
    int v0 = sub_0205F108(param0);

    if ((v0 == 0) || (v0 == 1) || (v0 == 2)) {
        return 1;
    }

    return 0;
}
