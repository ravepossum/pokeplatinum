#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_0205E884_decl.h"
#include "struct_decls/struct_02061830_decl.h"
#include "struct_decls/struct_02061AB4_decl.h"
#include "overlay101/struct_ov101_021D5D90_decl.h"

#include "field/field_system.h"

#include "unk_02054D00.h"
#include "unk_0205DAC8.h"
#include "unk_0205E7D0.h"
#include "unk_02061804.h"
#include "unk_02063400.h"
#include "unk_020655F4.h"
#include "unk_02067A84.h"
#include "unk_02069BE0.h"
#include "overlay005/ov5_021DF440.h"
#include "overlay005/ov5_021F3D00.h"

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    s16 unk_04;
    u16 unk_06;
} UnkStruct_02069CA8;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    s16 unk_04;
    u16 unk_06;
    UnkStruct_02061AB4 * unk_08;
} UnkStruct_02069F48;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    UnkStruct_ov101_021D5D90 * unk_04;
} UnkStruct_0206A0BC;

typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_0206A23C;

typedef struct {
    u32 unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_0206A47C;

static int sub_02069CA8(UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1);
static void sub_02069CD4(UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1);
static int sub_02069CFC(UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1);
static void sub_02069D30(UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1);
static u32 sub_02069D50(UnkStruct_02061AB4 * param0);
static int sub_02069D8C(UnkStruct_02061AB4 * param0);
static int sub_02069F48(UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1);
static void sub_02069FC4(UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1, UnkStruct_02061AB4 * param2);
static int sub_02069FE8(UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1);
static int sub_0206A034(UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1);
static int sub_0206A630(UnkStruct_02061AB4 * param0, UnkStruct_0206A47C * param1, int param2);

int(*const Unk_020EF8D4[])(UnkStruct_02061AB4 *, UnkStruct_02069CA8 *);
int(*const Unk_020EF8EC[])(UnkStruct_02061AB4 *, UnkStruct_02069F48 *);
int(*const Unk_020EF8DC[])(UnkStruct_02061AB4 *, UnkStruct_0206A0BC *);

void sub_02069BE0 (UnkStruct_02061AB4 * param0)
{
    UnkStruct_02069CA8 * v0 = sub_02062A54(param0, (sizeof(UnkStruct_02069CA8)));

    sub_02069CA8(param0, v0);
    sub_02062A0C(param0, 0x0);
    sub_02062D10(param0);
    sub_02062D80(param0, 0);
}

void sub_02069C0C (UnkStruct_02061AB4 * param0)
{
    UnkStruct_02069CA8 * v0 = sub_02062A78(param0);

    if (sub_02069CA8(param0, v0) == 0) {
        return;
    }

    sub_02062D80(param0, 0);

    while (Unk_020EF8D4[v0->unk_00](param0, v0) == 1) {
        (void)0;
    }
}

void sub_02069C44 (UnkStruct_02061AB4 * param0)
{
    return;
}

static int sub_02069C48 (UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1)
{
    sub_02062D10(param0);
    sub_02062D34(param0);

    if (sub_02069CFC(param0, param1) == 1) {
        sub_02069D30(param0, param1);

        if (sub_02069D8C(param0) == 1) {
            sub_02062D04(param0);
            param1->unk_00++;
            return 1;
        }
    }

    return 0;
}

static int sub_02069C8C (UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1)
{
    if (sub_020658DC(param0) == 1) {
        sub_02062D10(param0);
        param1->unk_00 = 0;
    }

    return 0;
}

static int(*const Unk_020EF8D4[])(UnkStruct_02061AB4 *, UnkStruct_02069CA8 *) = {
    sub_02069C48,
    sub_02069C8C
};

static int sub_02069CA8 (UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1)
{
    const UnkStruct_02061830 * v0 = sub_02062A40(param0);
    UnkStruct_02061AB4 * v1 = sub_0205EA24(v0);

    if (v1 == NULL) {
        param1->unk_01 = 0;
        return 0;
    }

    if (param1->unk_01 == 0) {
        sub_02069CD4(param0, param1);
    }

    return 1;
}

static void sub_02069CD4 (UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1)
{
    FieldSystem * v0 = sub_02062C00(param0);
    UnkStruct_0205E884 * v1 = sub_0205EF3C(v0);

    param1->unk_01 = 1;
    param1->unk_02 = sub_0205EABC(v1);
    param1->unk_04 = sub_0205EAC8(v1);
    param1->unk_06 = 0xff;
}

static int sub_02069CFC (UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1)
{
    FieldSystem * v0 = sub_02062C00(param0);
    UnkStruct_0205E884 * v1 = sub_0205EF3C(v0);

    if (v1 != NULL) {
        int v2 = sub_0205EABC(v1);
        int v3 = sub_0205EAC8(v1);

        if ((v2 != param1->unk_02) || (v3 != param1->unk_04)) {
            return 1;
        }
    }

    return 0;
}

static void sub_02069D30 (UnkStruct_02061AB4 * param0, UnkStruct_02069CA8 * param1)
{
    FieldSystem * v0 = sub_02062C00(param0);
    UnkStruct_0205E884 * v1 = sub_0205EF3C(v0);

    param1->unk_02 = sub_0205EABC(v1);
    param1->unk_04 = sub_0205EAC8(v1);
}

static u32 sub_02069D50 (UnkStruct_02061AB4 * param0)
{
    u32 v0;
    FieldSystem * v1 = sub_02062C00(param0);
    UnkStruct_0205E884 * v2 = sub_0205EF3C(v1);

    v0 = sub_0205EC14(v2);

    switch (v0) {
    case 0x58:
        v0 = 0x10;
        break;
    case 0x59:
        v0 = 0x11;
        break;
    case 0x5a:
        v0 = 0x12;
        break;
    case 0x5b:
        v0 = 0x13;
        break;
    }

    return v0;
}

static int sub_02069D8C (UnkStruct_02061AB4 * param0)
{
    FieldSystem * v0 = sub_02062C00(param0);
    UnkStruct_0205E884 * v1 = sub_0205EF3C(v0);
    int v2 = sub_02063020(param0);
    int v3 = sub_02063040(param0);
    int v4 = sub_0205EAD4(v1);
    int v5 = sub_0205EAE0(v1);

    if ((v2 != v4) || (v3 != v5)) {
        u32 v6 = sub_02069D50(param0);
        int v7 = sub_02064488(v2, v3, v4, v5);

        v6 = sub_02065838(v7, v6);
        sub_02065668(param0, v6);

        return 1;
    }

    return 0;
}

void sub_02069DF4 (UnkStruct_02061AB4 * param0)
{
    UnkStruct_02069F48 * v0 = sub_02062A54(param0, (sizeof(UnkStruct_02069F48)));

    sub_02069F48(param0, v0);
    sub_02062A0C(param0, 0x0);
    sub_02062D10(param0);

    v0->unk_01 = 0;
}

void sub_02069E1C (UnkStruct_02061AB4 * param0)
{
    UnkStruct_02069F48 * v0 = sub_02062A78(param0);

    if (sub_02069F48(param0, v0) == 0) {
        return;
    }

    while (Unk_020EF8EC[v0->unk_00](param0, v0) == 1) {
        (void)0;
    }
}

void sub_02069E4C (UnkStruct_02061AB4 * param0)
{
    return;
}

void sub_02069E50 (UnkStruct_02061AB4 * param0)
{
    UnkStruct_02069F48 * v0 = sub_02062A78(param0);
    v0->unk_01 = 0;
}

static int sub_02069E5C (UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1)
{
    sub_02062D10(param0);
    sub_02062D34(param0);

    if (sub_02069FE8(param0, param1) == 1) {
        if (sub_0206A034(param0, param1) == 1) {
            sub_02062D04(param0);
            param1->unk_00++;
            return 1;
        }
    }

    return 0;
}

static int sub_02069E98 (UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1)
{
    if (sub_020658DC(param0) == 0) {
        return 0;
    }

    sub_02062D10(param0);
    param1->unk_00 = 0;
    return 0;
}

static int(*const Unk_020EF8EC[])(UnkStruct_02061AB4 *, UnkStruct_02069F48 *) = {
    sub_02069E5C,
    sub_02069E98
};

UnkStruct_02061AB4 * sub_02069EB8 (UnkStruct_02061AB4 * param0)
{
    int v0 = 0;
    int v1 = sub_02062950(param0);
    int v2 = sub_02062918(param0);
    u32 v3 = sub_02067F24(param0);
    const UnkStruct_02061830 * v4 = sub_02062A40(param0);
    UnkStruct_02061AB4 * v5;

    switch (v1) {
    case 0x1:
    case 0x2:
    case 0x3:
    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:
    case 0x8:
        while (sub_020625B0(v4, &v5, &v0, (1 << 0)) == 1) {
            if ((param0 != v5) && (sub_02062918(v5) == v2)) {
                if (sub_02067F24(v5) == v3) {
                    return v5;
                }
            }
        }
    }

    return NULL;
}

static int sub_02069F48 (UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1)
{
    int v0, v1;
    u32 v2;
    UnkStruct_02061AB4 * v3;
    const UnkStruct_02061830 * v4 = sub_02062A40(param0);

    v0 = 0;
    v1 = sub_02062918(param0);
    v2 = sub_02067F24(param0);

    while (sub_020625B0(v4, &v3, &v0, (1 << 0)) == 1) {
        if ((param0 != v3) && (sub_02062918(v3) == v1) && (sub_02067F24(v3) == v2)) {
            if (param1->unk_01 == 0) {
                sub_02069FC4(param0, param1, v3);
            }

            return 1;
        }
    }

    param1->unk_01 = 0;
    return 0;
}

static void sub_02069FC4 (UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1, UnkStruct_02061AB4 * param2)
{
    param1->unk_01 = 1;
    param1->unk_02 = sub_02063020(param2);
    param1->unk_04 = sub_02063040(param2);
    param1->unk_06 = 0xff;
    param1->unk_08 = param2;
}

static int sub_02069FE8 (UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1)
{
    UnkStruct_02061AB4 * v0 = param1->unk_08;
    int v1 = sub_02063020(param0);
    int v2 = sub_02063040(param0);
    int v3 = sub_02063008(v0);
    int v4 = sub_02063018(v0);

    if (((v1 != v3) || (v2 != v4)) && ((sub_02062D1C(v0) == 1) || (sub_020628D0(v0, ((1 << 11) | (1 << 12) | (1 << 6))) == 0))) {
        return 1;
    }

    return 0;
}

static int sub_0206A034 (UnkStruct_02061AB4 * param0, UnkStruct_02069F48 * param1)
{
    int v0 = sub_02063020(param0);
    int v1 = sub_02063040(param0);
    int v2 = sub_02063020(param1->unk_08);
    int v3 = sub_02063040(param1->unk_08);
    int v4 = sub_02063008(param1->unk_08);
    int v5 = sub_02063018(param1->unk_08);
    int v6;

    if ((v0 == v2) && (v1 == v3)) {
        return 0;
    }

    v6 = sub_02064488(v0, v1, v4, v5);
    v0 += sub_0206419C(v6);
    v1 += sub_020641A8(v6);

    if ((v0 != v2) || (v1 != v3)) {
        u32 v7 = 0xc;

        v7 = sub_02065838(v6, v7);
        sub_02065668(param0, v7);
        return 1;
    }

    return 0;
}

static void sub_0206A0BC (UnkStruct_02061AB4 * param0, int param1)
{
    UnkStruct_0206A0BC * v0 = sub_02062A54(param0, (sizeof(UnkStruct_0206A0BC)));

    v0->unk_01 = param1;

    sub_02062A0C(param0, 0x0);
    sub_02062D10(param0);
    sub_020628BC(param0, (1 << 20));

    {
        VecFx32 v1 = {0, (FX32_ONE * -32), 0};

        sub_02063088(param0, &v1);
    }
}

void sub_0206A104 (UnkStruct_02061AB4 * param0)
{
    sub_0206A0BC(param0, 0);
}

void sub_0206A110 (UnkStruct_02061AB4 * param0)
{
    sub_0206A0BC(param0, 1);
}

void sub_0206A11C (UnkStruct_02061AB4 * param0)
{
    sub_0206A0BC(param0, 2);
}

void sub_0206A128 (UnkStruct_02061AB4 * param0)
{
    sub_0206A0BC(param0, 3);
}

void sub_0206A134 (UnkStruct_02061AB4 * param0)
{
    UnkStruct_0206A0BC * v0 = sub_02062A78(param0);

    while (Unk_020EF8DC[v0->unk_00](param0, v0) == 1) {
        (void)0;
    }
}

void sub_0206A158 (UnkStruct_02061AB4 * param0)
{
    UnkStruct_ov101_021D5D90 * v0 = sub_0206A224(param0);

    if (v0 != NULL) {
        ov5_021DF74C(v0);
    }
}

void sub_0206A168 (UnkStruct_02061AB4 * param0)
{
    UnkStruct_0206A0BC * v0 = sub_02062A78(param0);

    v0->unk_00 = 0;
    sub_0206A218(param0, NULL);

    if (v0->unk_02 == 0) {
        VecFx32 v1 = {0, (FX32_ONE * -32), 0};

        sub_02063088(param0, &v1);
        sub_020628BC(param0, (1 << 20));
    }
}

static int sub_0206A1AC (UnkStruct_02061AB4 * param0, UnkStruct_0206A0BC * param1)
{
    if (param1->unk_02 == 0) {
        UnkStruct_ov101_021D5D90 * v0 = ov5_021F3D90(param0, param1->unk_01);

        sub_0206A218(param0, v0);
    }

    sub_02062D10(param0);
    sub_02062D34(param0);
    param1->unk_00++;

    return 0;
}

static int sub_0206A1DC (UnkStruct_02061AB4 * param0, UnkStruct_0206A0BC * param1)
{
    if (param1->unk_02 == 0) {
        UnkStruct_ov101_021D5D90 * v0 = sub_0206A224(param0);

        if (v0 == NULL) {
            if (sub_02062DFC(param0) == 1) {
                v0 = ov5_021F3D90(param0, param1->unk_01);
                sub_0206A218(param0, v0);
            }
        }

        sub_020628BC(param0, (1 << 20));
    }

    return 0;
}

static int(*const Unk_020EF8DC[])(UnkStruct_02061AB4 *, UnkStruct_0206A0BC *) = {
    sub_0206A1AC,
    sub_0206A1DC
};

void sub_0206A218 (UnkStruct_02061AB4 * param0, UnkStruct_ov101_021D5D90 * param1)
{
    UnkStruct_0206A0BC * v0 = sub_02062A78(param0);
    v0->unk_04 = param1;
}

UnkStruct_ov101_021D5D90 * sub_0206A224 (UnkStruct_02061AB4 * param0)
{
    UnkStruct_0206A0BC * v0 = sub_02062A78(param0);
    return v0->unk_04;
}

void sub_0206A230 (UnkStruct_02061AB4 * param0)
{
    UnkStruct_0206A0BC * v0 = sub_02062A78(param0);
    v0->unk_02 = 1;
}

static int(*const Unk_020EF918[5])(UnkStruct_02061AB4 *, UnkStruct_0206A23C *);

static void sub_0206A23C (UnkStruct_02061AB4 * param0, int param1, u32 param2)
{
    UnkStruct_0206A23C * v0 = sub_02062A54(param0, (sizeof(UnkStruct_0206A23C)));

    v0->unk_01 = -1;
    v0->unk_02 = param2;

    sub_02062974(param0, 0);
}

void sub_0206A25C (UnkStruct_02061AB4 * param0)
{
    sub_0206A23C(param0, 0, 0);
}

void sub_0206A268 (UnkStruct_02061AB4 * param0)
{
    sub_0206A23C(param0, 1, 0);
}

void sub_0206A274 (UnkStruct_02061AB4 * param0)
{
    sub_0206A23C(param0, 2, 0);
}

void sub_0206A280 (UnkStruct_02061AB4 * param0)
{
    sub_0206A23C(param0, 3, 0);
}

void sub_0206A28C (UnkStruct_02061AB4 * param0)
{
    sub_0206A23C(param0, 0, 1);
}

void sub_0206A298 (UnkStruct_02061AB4 * param0)
{
    sub_0206A23C(param0, 1, 1);
}

void sub_0206A2A4 (UnkStruct_02061AB4 * param0)
{
    sub_0206A23C(param0, 2, 1);
}

void sub_0206A2B0 (UnkStruct_02061AB4 * param0)
{
    sub_0206A23C(param0, 3, 1);
}

void sub_0206A2BC (UnkStruct_02061AB4 * param0)
{
    UnkStruct_0206A23C * v0 = sub_02062A78(param0);

    while (Unk_020EF918[v0->unk_00](param0, v0) == 1) {
        (void)0;
    }
}

static int sub_0206A2E0 (UnkStruct_02061AB4 * param0, UnkStruct_0206A23C * param1)
{
    int v0 = sub_0206298C(param0);

    v0 = sub_02065838(v0, 0x0);

    sub_02065668(param0, v0);
    sub_02062D10(param0);
    sub_02062D34(param0);

    param1->unk_00 = 1;
    return 1;
}

static int sub_0206A30C (UnkStruct_02061AB4 * param0, UnkStruct_0206A23C * param1)
{
    if (sub_020658DC(param0) == 1) {
        param1->unk_00 = 2;
        return 1;
    }

    return 0;
}

static int sub_0206A324 (UnkStruct_02061AB4 * param0, UnkStruct_0206A23C * param1)
{
    if (param1->unk_01 == -1) {
        FieldSystem * v0 = sub_02062C00(param0);

        param1->unk_01 = sub_0205EA78(v0->unk_3C);
    }

    sub_02062D10(param0);
    sub_02062D34(param0);

    param1->unk_00 = 3;
    return 1;
}

static u32 sub_0206A354 (UnkStruct_02061AB4 * param0, int param1)
{
    u32 v0 = sub_02064238(param0, param1);

    if (sub_0205DAD4(v0) == 0) {
        v0 = (1 << 1);
    }

    v0 |= sub_02063EBC(param0, param1);
    return v0;
}

static void sub_0206A37C (UnkStruct_02061AB4 * param0, int param1, int param2, u32 param3)
{
    u32 v0;

    if (param3 == 0) {
        v0 = sub_02063EBC(param0, param1);
    } else {
        v0 = sub_0206A354(param0, param1);
    }

    if (v0 != 0) {
        param2 = sub_02065838(param1, 0x0);
    } else {
        param2 = sub_02065838(param1, param2);
        sub_02062D04(param0);
    }

    sub_02065668(param0, param2);
}

static int sub_0206A3BC (UnkStruct_02061AB4 * param0, UnkStruct_0206A23C * param1)
{
    int v0;
    FieldSystem * v1 = sub_02062C00(param0);
    int v2 = sub_0205EA78(v1->unk_3C);
    u32 v3 = sub_0205EC1C(v1->unk_3C);

    switch (v3) {
    case 0:
    case 1:
        v0 = sub_02065838(v2, 0x0);
        sub_02065668(param0, v0);
        break;
    case 2:
        sub_0206A37C(param0, v2, 0x4, param1->unk_02);
        break;
    case 3:
        sub_0206A37C(param0, v2, 0x8, param1->unk_02);
        break;
    case 4:
        sub_0206A37C(param0, v2, 0xc, param1->unk_02);
        break;
    case 5:
        sub_0206A37C(param0, v2, 0x10, param1->unk_02);
        break;
    case 6:
        sub_0206A37C(param0, v2, 0x14, param1->unk_02);
        break;
    }

    param1->unk_00 = 4;
    return 1;
}

static int sub_0206A454 (UnkStruct_02061AB4 * param0, UnkStruct_0206A23C * param1)
{
    if (sub_020658DC(param0) == 0) {
        return 0;
    }

    sub_02062D10(param0);
    sub_02062D34(param0);

    param1->unk_00 = 2;
    return 0;
}

static int(*const Unk_020EF918[5])(UnkStruct_02061AB4 *, UnkStruct_0206A23C *) = {
    sub_0206A2E0,
    sub_0206A30C,
    sub_0206A324,
    sub_0206A3BC,
    sub_0206A454
};

static int(*const Unk_020EF90C[3])(UnkStruct_02061AB4 *, UnkStruct_0206A47C *);

static void sub_0206A47C (UnkStruct_02061AB4 * param0, int param1, int param2)
{
    UnkStruct_0206A47C * v0 = sub_02062A54(param0, (sizeof(UnkStruct_0206A47C)));

    v0->unk_04 = param1;
    v0->unk_08 = param2;
}

void sub_0206A490 (UnkStruct_02061AB4 * param0)
{
    sub_0206A47C(param0, 0, 0);
}

void sub_0206A49C (UnkStruct_02061AB4 * param0)
{
    sub_0206A47C(param0, 1, 1);
}

void sub_0206A4A8 (UnkStruct_02061AB4 * param0)
{
    sub_0206A47C(param0, 2, 0);
}

void sub_0206A4B4 (UnkStruct_02061AB4 * param0)
{
    sub_0206A47C(param0, 2, 1);
}

void sub_0206A4C0 (UnkStruct_02061AB4 * param0)
{
    UnkStruct_0206A47C * v0 = sub_02062A78(param0);

    while (Unk_020EF90C[v0->unk_00](param0, v0) == 1) {
        (void)0;
    }
}

static int sub_0206A4E4 (UnkStruct_02061AB4 * param0, UnkStruct_0206A47C * param1)
{
    sub_02062D10(param0);
    param1->unk_00++;
    return 1;
}

static int sub_0206A4F8 (UnkStruct_02061AB4 * param0, UnkStruct_0206A47C * param1)
{
    sub_0206A630(param0, param1, 0xc);
    param1->unk_00++;
    return 1;
}

static int sub_0206A50C (UnkStruct_02061AB4 * param0, UnkStruct_0206A47C * param1)
{
    if (sub_020658DC(param0) == 1) {
        param1->unk_00 = 0;
        return 1;
    }

    return 0;
}

static int(*const Unk_020EF90C[3])(UnkStruct_02061AB4 *, UnkStruct_0206A47C *) = {
    sub_0206A4E4,
    sub_0206A4F8,
    sub_0206A50C
};

static const int Unk_020EF92C[4][2] = {
    {-1, 0x1},
    {0x1, -1},
    {0x0, 0x0},
    {0x0, 0x0}
};

static const int Unk_020EF94C[4][2] = {
    {0x0, 0x0},
    {0x0, 0x0},
    {0x1, -1},
    {-1, 0x1}
};

static const int Unk_020EF98C[4][2] = {
    {-1, 0x1},
    {0x1, -1},
    {0x1, 0x1},
    {-1, -1}
};

static const int Unk_020EF9AC[4][2] = {
    {0x1, 0x1},
    {-1, -1},
    {0x1, -1},
    {-1, 0x1}
};

static const int Unk_020EF96C[4][2] = {
    {0x2, 0x3},
    {0x3, 0x2},
    {0x1, 0x0},
    {0x0, 0x1}
};

static const int Unk_020EF9CC[4][2] = {
    {0x3, 0x2},
    {0x2, 0x3},
    {0x0, 0x1},
    {0x1, 0x0}
};

static const int Unk_020EF8E4[2] = {
    0x1,
    0x0
};

static BOOL sub_0206A524 (FieldSystem * param0, int param1, int param2, int param3, int param4)
{
    BOOL v0;

    param1 += Unk_020EF92C[param3][param4];
    param2 += Unk_020EF94C[param3][param4];

    v0 = sub_02054F68(param0, param1, param2);
    return v0;
}

static BOOL sub_0206A54C (FieldSystem * param0, int param1, int param2, int param3, int param4)
{
    BOOL v0;

    param1 += Unk_020EF98C[param3][param4];
    param2 += Unk_020EF9AC[param3][param4];

    v0 = sub_02054F68(param0, param1, param2);
    return v0;
}

static int sub_0206A574 (UnkStruct_02061AB4 * param0, int param1, int param2)
{
    FieldSystem * v0 = sub_02062C00(param0);
    int v1 = sub_02063020(param0);
    int v2 = sub_02063040(param0);
    BOOL v3 = sub_0206A524(v0, v1, v2, param1, param2);

    return v3;
}

static int sub_0206A5A4 (UnkStruct_02061AB4 * param0, int param1, int param2)
{
    FieldSystem * v0 = sub_02062C00(param0);
    int v1 = sub_02063020(param0);
    int v2 = sub_02063040(param0);
    BOOL v3 = sub_0206A54C(v0, v1, v2, param1, param2);

    return v3;
}

static int sub_0206A5D4 (UnkStruct_02061AB4 * param0, int param1, int param2)
{
    if (sub_0206A574(param0, param1, param2) == 0) {
        if (sub_0206A5A4(param0, param1, param2) == 0) {
            return(-1);
        }

        param1 = Unk_020EF96C[param1][param2];
    }

    return param1;
}

static u32 sub_0206A60C (UnkStruct_02061AB4 * param0, int * param1, int param2)
{
    u32 v0;

    *param1 = sub_0206A5D4(param0, *param1, param2);

    if (*param1 != -1) {
        v0 = sub_02063EBC(param0, *param1);
        return v0;
    }

    return 0;
}

static int sub_0206A630 (UnkStruct_02061AB4 * param0, UnkStruct_0206A47C * param1, int param2)
{
    u32 v0;
    int v1 = param1->unk_08;
    int v2 = sub_0206298C(param0);

    v0 = sub_0206A60C(param0, &v2, v1);

    if (v2 == -1) {
        v2 = sub_0206298C(param0);
        param2 = sub_02065838(v2, 0x1c);
        sub_02065668(param0, param2);
        return 0;
    }

    if (v0 == 0) {
        param2 = sub_02065838(v2, param2);
        sub_02062D04(param0);
        sub_02065668(param0, param2);
        return 1;
    }

    if ((v0 & (1 << 0)) && (param1->unk_04 == 2)) {
        v2 = sub_0206447C(sub_0206298C(param0));
        v1 = Unk_020EF8E4[v1];
        param1->unk_08 = v1;

        v0 = sub_0206A60C(param0, &v2, v1);

        if (v2 == -1) {
            v2 = sub_0206298C(param0);
            param2 = sub_02065838(v2, 0x1c);
            sub_02065668(param0, param2);
            return 0;
        }

        if (v0 == 0) {
            param2 = sub_02065838(v2, param2);
            sub_02062D04(param0);
            sub_02065668(param0, param2);
            return 1;
        }
    }

    if ((v0 & (1 << 1))) {
        v2 = Unk_020EF9CC[v2][v1];
        v0 = sub_0206A60C(param0, &v2, v1);

        if (v2 == -1) {
            v2 = sub_0206298C(param0);
            param2 = sub_02065838(v2, 0x1c);
            sub_02065668(param0, param2);
            return 0;
        }

        if (v0 == 0) {
            param2 = sub_02065838(v2, param2);
            sub_02062D04(param0);
            sub_02065668(param0, param2);
            return 1;
        }
    }

    v2 = sub_0206298C(param0);
    param2 = sub_02065838(v2, 0x1c);

    sub_02065668(param0, param2);
    return 0;
}
