#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_02006C24_decl.h"
#include "struct_decls/struct_0205E884_decl.h"
#include "struct_decls/struct_02061830_decl.h"
#include "struct_decls/struct_02061AB4_decl.h"
#include "overlay005/struct_ov5_021D1BEC_decl.h"

#include "struct_defs/struct_020170F4.h"
#include "struct_defs/struct_02017248.h"
#include "struct_defs/struct_02017294.h"
#include "field/field_system.h"
#include "field/field_system_sub2_t.h"
#include "overlay006/struct_ov6_0223E6EC.h"

#include "unk_02005474.h"
#include "narc.h"
#include "unk_02006E3C.h"
#include "unk_020170BC.h"
#include "heap.h"
#include "unk_0205E7D0.h"
#include "unk_02061804.h"
#include "overlay005/ov5_021D1A94.h"
#include "overlay006/ov6_02248050.h"

typedef struct {
    UnkStruct_02017294 unk_00;
    UnkStruct_020170F4 unk_78;
    UnkStruct_02017248 unk_88[4];
    u32 unk_D8;
} UnkStruct_ov6_02248140;

typedef struct {
    UnkStruct_ov6_02248140 unk_00;
    NNSFndAllocator unk_DC;
    u32 unk_EC;
} UnkStruct_ov6_0224825C;

typedef struct {
    UnkStruct_ov6_02248140 unk_00;
    NNSFndAllocator unk_DC;
    u32 unk_EC;
} UnkStruct_ov6_02248340;

typedef struct UnkStruct_ov6_02248498_t {
    UnkStruct_ov6_02248140 unk_00[2];
    NNSFndAllocator unk_1B8;
    u32 unk_1C8;
} UnkStruct_ov6_02248498;

typedef struct {
    UnkStruct_02017294 unk_00;
    UnkStruct_02017248 unk_78[4];
    u32 unk_C8;
} UnkStruct_ov6_022486B4;

typedef struct {
    UnkStruct_020170F4 unk_00;
    UnkStruct_ov6_022486B4 unk_10[16];
    void * unk_CD0[4];
    FieldSystem * unk_CE0;
    int unk_CE4;
    fx32 unk_CE8;
    fx32 unk_CEC;
    VecFx32 unk_CF0;
    NNSFndAllocator unk_CFC;
    u32 unk_D0C;
} UnkStruct_ov6_022487F8;

static void ov6_02248050 (UnkStruct_02061830 * param0, u32 param1, UnkStruct_02017294 * param2)
{
    int v0;
    int v1, v2;
    VecFx32 v3;
    LocalMapObject * v4;

    v4 = sub_0206251C(param0, param1);

    sub_02063050(v4, &v3);

    v1 = sub_02063020(v4);
    v2 = sub_02063040(v4);
    v0 = sub_0206298C(v4);

    switch (v0) {
    case 0:
        v2--;
        break;
    case 1:
        v2++;
        break;
    case 2:
        v1--;
        break;
    case 3:
        v1++;
        break;
    default:
        GF_ASSERT(0);
        break;
    }

    sub_02017350(param2, (((v1) << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1), v3.y, (((v2) << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1));
}

static void ov6_022480BC (PlayerAvatar * const param0, UnkStruct_02017294 * param1)
{
    int v0;
    int v1, v2;
    VecFx32 v3;

    sub_0205EAEC(param0, &v3);

    v1 = Player_XPos(param0);
    v2 = Player_ZPos(param0);
    v0 = Player_Dir(param0);

    switch (v0) {
    case 0:
        v2--;
        break;
    case 1:
        v2++;
        break;
    case 2:
        v1--;
        break;
    case 3:
        v1++;
        break;
    default:
        GF_ASSERT(0);
        break;
    }

    sub_02017350(param1, (((v1) << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1), v3.y, (((v2) << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1));
}

static void ov6_02248124 (PlayerAvatar * const param0, UnkStruct_02017294 * param1)
{
    VecFx32 v0;

    sub_0205EAEC(param0, &v0);
    sub_02017350(param1, v0.x, v0.y, v0.z);
}

static void ov6_02248140 (UnkStruct_ov6_02248140 * param0, NARC * param1, u32 param2, u32 param3, int param4, NNSFndAllocator * param5)
{
    int v0;

    memset(param0, 0, sizeof(UnkStruct_ov6_02248140));

    sub_020170D8(&param0->unk_78, param1, param2, 4);
    sub_02017258(&param0->unk_00, &param0->unk_78);

    param0->unk_D8 = param4;

    for (v0 = 0; v0 < param0->unk_D8; v0++) {
        sub_02017164(&param0->unk_88[v0], &param0->unk_78, param1, param3 + v0, 4, param5);
        sub_0201727C(&param0->unk_00, &param0->unk_88[v0]);
    }
}

static void ov6_022481BC (UnkStruct_ov6_02248140 * param0, NNSFndAllocator * param1)
{
    int v0;

    sub_02017110(&param0->unk_78);

    for (v0 = 0; v0 < param0->unk_D8; v0++) {
        sub_020171A0(&param0->unk_88[v0], param1);
    }
}

static BOOL ov6_022481F0 (UnkStruct_ov6_02248140 * param0)
{
    int v0;
    BOOL v1;

    v1 = 1;

    for (v0 = 0; v0 < param0->unk_D8; v0++) {
        v1 &= sub_02017204(&param0->unk_88[v0], FX32_ONE);
    }

    return v1;
}

static void ov6_02248224 (UnkStruct_ov6_02248140 * param0)
{
    int v0;

    for (v0 = 0; v0 < param0->unk_D8; v0++) {
        sub_020171CC(&param0->unk_88[v0], FX32_ONE);
    }
}

static void ov6_02248254 (UnkStruct_ov6_02248140 * param0)
{
    sub_02017294(&param0->unk_00);
}

static void ov6_0224825C (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_0224825C * v0 = param2;
    int v1;
    NARC * v2;

    memset(v0, 0, sizeof(UnkStruct_ov6_0224825C));
    Heap_FndInitAllocatorForExpHeap(&v0->unk_DC, 4, 32);

    v2 = NARC_ctor(NARC_INDEX_GRAPHIC__HIDEN_EFFECT, 4);

    ov6_02248140(&v0->unk_00, v2, 3, 0, 3, &v0->unk_DC);
    NARC_dtor(v2);

    ov6_022480BC(param1->playerAvatar, &v0->unk_00.unk_00);
    Sound_PlayEffect(1611);

    v0->unk_EC = 0;
}

static void ov6_022482BC (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_0224825C * v0 = param2;
    ov6_022481BC(&v0->unk_00, &v0->unk_DC);
}

static void ov6_022482CC (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_0224825C * v0 = param2;
    int v1;
    BOOL v2;

    switch (v0->unk_EC) {
    case 0:
        v2 = ov6_022481F0(&v0->unk_00);

        if (v2 == 1) {
            sub_02017348(&v0->unk_00.unk_00, 0);
            v0->unk_EC++;
        }
        break;
    case 1:
        break;
    default:
        break;
    }
}

static void ov6_02248300 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_0224825C * v0 = param2;
    ov6_02248254(&v0->unk_00);
}

UnkStruct_ov5_021D1BEC * ov6_0224830C (FieldSystem * param0)
{
    static const UnkStruct_ov6_0223E6EC v0 = {
        1024, sizeof(UnkStruct_ov6_0224825C), ov6_0224825C, ov6_022482BC, ov6_022482CC, ov6_02248300
    };
    UnkStruct_ov5_021D1BEC * v1;

    v1 = ov5_021D1B6C(param0->unk_04->unk_04, &v0);
    return v1;
}

void ov6_02248320 (UnkStruct_ov5_021D1BEC * param0)
{
    ov5_021D1BEC(param0);
}

BOOL ov6_02248328 (UnkStruct_ov5_021D1BEC * param0)
{
    UnkStruct_ov6_0224825C * v0 = ov5_021D1C2C(param0);

    if (v0->unk_EC == 1) {
        return 1;
    }

    return 0;
}

static void ov6_02248340 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_02248340 * v0 = param2;
    int v1;
    NARC * v2;

    memset(v0, 0, sizeof(UnkStruct_ov6_02248340));
    Heap_FndInitAllocatorForExpHeap(&v0->unk_DC, 4, 32);

    v2 = NARC_ctor(NARC_INDEX_GRAPHIC__HIDEN_EFFECT, 4);

    ov6_02248140(&v0->unk_00, v2, 8, 4, 4, &v0->unk_DC);
    NARC_dtor(v2);

    ov6_022480BC(param1->playerAvatar, &v0->unk_00.unk_00);
    Sound_PlayEffect(1610);

    v0->unk_EC = 0;
}

static void ov6_022483A0 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_02248340 * v0 = param2;
    int v1;
    NARC * v2;

    memset(v0, 0, sizeof(UnkStruct_ov6_02248340));
    Heap_FndInitAllocatorForExpHeap(&v0->unk_DC, 4, 32);

    v2 = NARC_ctor(NARC_INDEX_GRAPHIC__HIDEN_EFFECT, 4);

    ov6_02248140(&v0->unk_00, v2, 8, 4, 4, &v0->unk_DC);
    ov6_02248050(param1->unk_38, 0, &v0->unk_00.unk_00);

    Sound_PlayEffect(1610);
    NARC_dtor(v2);

    v0->unk_EC = 0;
}

static void ov6_02248400 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_02248340 * v0 = param2;
    ov6_022481BC(&v0->unk_00, &v0->unk_DC);
}

static void ov6_02248410 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_02248340 * v0 = param2;
    int v1;
    BOOL v2;

    switch (v0->unk_EC) {
    case 0:
        v2 = ov6_022481F0(&v0->unk_00);

        if (v2 == 1) {
            sub_02017348(&v0->unk_00.unk_00, 0);
            v0->unk_EC++;
        }
        break;
    case 1:
        break;
    default:
        break;
    }
}

static void ov6_02248444 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_02248340 * v0 = param2;
    ov6_02248254(&v0->unk_00);
}

UnkStruct_ov5_021D1BEC * ov6_02248450 (FieldSystem * param0)
{
    static const UnkStruct_ov6_0223E6EC v0 = {
        1024,
        sizeof(UnkStruct_ov6_02248340),
        ov6_02248340,
        ov6_02248400,
        ov6_02248410,
        ov6_02248444
    };
    UnkStruct_ov5_021D1BEC * v1;

    v1 = ov5_021D1B6C(param0->unk_04->unk_04, &v0);
    return v1;
}

UnkStruct_ov5_021D1BEC * ov6_02248464 (FieldSystem * param0)
{
    static const UnkStruct_ov6_0223E6EC v0 = {
        1024,
        sizeof(UnkStruct_ov6_02248340),
        ov6_022483A0,
        ov6_02248400,
        ov6_02248410,
        ov6_02248444
    };
    UnkStruct_ov5_021D1BEC * v1;

    v1 = ov5_021D1B6C(param0->unk_04->unk_04, &v0);
    return v1;
}

void ov6_02248478 (UnkStruct_ov5_021D1BEC * param0)
{
    ov5_021D1BEC(param0);
}

BOOL ov6_02248480 (UnkStruct_ov5_021D1BEC * param0)
{
    UnkStruct_ov6_02248340 * v0 = ov5_021D1C2C(param0);

    if (v0->unk_EC == 1) {
        return 1;
    }

    return 0;
}

static void ov6_02248498 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_02248498 * v0 = param2;
    int v1;
    NARC * v2;

    memset(v0, 0, sizeof(UnkStruct_ov6_02248498));
    Heap_FndInitAllocatorForExpHeap(&v0->unk_1B8, 4, 32);

    v2 = NARC_ctor(NARC_INDEX_GRAPHIC__HIDEN_EFFECT, 4);

    ov6_02248140(&v0->unk_00[0], v2, 14, 12, 2, &v0->unk_1B8);
    ov6_02248124(param1->playerAvatar, &v0->unk_00[0].unk_00);

    NARC_dtor(v2);
    Sound_PlayEffect(1613);

    v0->unk_1C8 = 0;
}

static void ov6_02248500 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_02248498 * v0 = param2;
    int v1;

    for (v1 = 0; v1 < 2; v1++) {
        ov6_022481BC(&v0->unk_00[v1], &v0->unk_1B8);
    }
}

static void ov6_02248520 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_02248498 * v0 = param2;
    BOOL v1;
    NARC * v2;

    switch (v0->unk_1C8) {
    case 0:
        v2 = NARC_ctor(NARC_INDEX_GRAPHIC__HIDEN_EFFECT, 4);
        ov6_02248140(&v0->unk_00[1], v2, 11, 9, 2, &v0->unk_1B8);
        NARC_dtor(v2);
        ov6_02248124(param1->playerAvatar, &v0->unk_00[1].unk_00);
        sub_02017348(&v0->unk_00[1].unk_00, 0);
        v0->unk_1C8++;
    case 1:
        v1 = ov6_022481F0(&v0->unk_00[0]);
        ov6_02248124(param1->playerAvatar, &v0->unk_00[0].unk_00);

        if (v1 == 1) {
            sub_02017348(&v0->unk_00[1].unk_00, 1);
            sub_02017348(&v0->unk_00[0].unk_00, 0);
            ov6_02248124(param1->playerAvatar, &v0->unk_00[1].unk_00);
            v0->unk_1C8++;
        }
        break;
    case 2:
        ov6_02248224(&v0->unk_00[1]);
        ov6_02248124(param1->playerAvatar, &v0->unk_00[1].unk_00);
        break;
    default:
        break;
    }
}

static void ov6_022485DC (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_02248498 * v0 = param2;
    int v1;

    for (v1 = 0; v1 < 2; v1++) {
        ov6_02248254(&v0->unk_00[v1]);
    }
}

UnkStruct_ov5_021D1BEC * ov6_022485F4 (FieldSystem * param0)
{
    static const UnkStruct_ov6_0223E6EC v0 = {
        1024,
        sizeof(UnkStruct_ov6_02248498),
        ov6_02248498,
        ov6_02248500,
        ov6_02248520,
        ov6_022485DC
    };
    UnkStruct_ov5_021D1BEC * v1;

    v1 = ov5_021D1B6C(param0->unk_04->unk_04, &v0);
    return v1;
}

void ov6_02248608 (UnkStruct_ov5_021D1BEC * param0)
{
    ov5_021D1BEC(param0);
}

static void ov6_02248610 (UnkStruct_ov6_022486B4 * param0, PlayerAvatar * const param1, fx32 param2, fx32 param3)
{
    VecFx32 v0;
    int v1;

    GF_ASSERT(param0->unk_C8 == 0);

    sub_0205EAEC(param1, &v0);
    sub_02017350(&param0->unk_00, v0.x, v0.y + param2, v0.z + param3);

    param0->unk_C8 = 1;

    for (v1 = 0; v1 < 4; v1++) {
        sub_02017240(&param0->unk_78[v1], 0);
    }

    sub_02017348(&param0->unk_00, 1);
    Sound_PlayEffect(1575);
}

static void ov6_02248678 (UnkStruct_ov6_022486B4 * param0)
{
    int v0;
    BOOL v1;

    if (param0->unk_C8 == 0) {
        return;
    }

    v1 = 1;

    for (v0 = 0; v0 < 4; v0++) {
        v1 &= sub_02017204(&param0->unk_78[v0], FX32_ONE);
    }

    if (v1 == 1) {
        param0->unk_C8 = 0;
        sub_02017348(&param0->unk_00, 0);
    }
}

static void ov6_022486B4 (UnkStruct_ov6_022486B4 * param0, UnkStruct_020170F4 * param1, NNSFndAllocator * param2, void ** param3)
{
    int v0;

    memset(param0, 0, sizeof(UnkStruct_ov6_022486B4));
    sub_02017258(&param0->unk_00, param1);

    for (v0 = 0; v0 < 4; v0++) {
        sub_02017190(&param0->unk_78[v0], param1, param3[v0], param2);
        sub_0201727C(&param0->unk_00, &param0->unk_78[v0]);
    }

    sub_02017348(&param0->unk_00, 0);
}

static void ov6_02248700 (UnkStruct_ov6_022486B4 * param0, NNSFndAllocator * param1)
{
    int v0;

    for (v0 = 0; v0 < 4; v0++) {
        sub_020171A0(&param0->unk_78[v0], param1);
    }

    memset(param0, 0, sizeof(UnkStruct_ov6_022486B4));
}

static void ov6_02248728 (UnkStruct_ov6_022487F8 * param0)
{
    VecFx32 v0;

    sub_0205EAEC(param0->unk_CE0->playerAvatar, &v0);

    if (((v0.x - param0->unk_CF0.x) == 0) && (v0.y < param0->unk_CF0.y) && (v0.z > param0->unk_CF0.z)) {
        param0->unk_CE8 = 0;
        param0->unk_CEC = ((16 * FX32_ONE)) * 2;
    } else {
        param0->unk_CE8 = ((16 * FX32_ONE) * 2);
        param0->unk_CEC = ((16 * FX32_ONE));
    }

    param0->unk_CF0 = v0;
}

static void ov6_02248798 (UnkStruct_ov6_022487F8 * param0)
{
    int v0;

    ov6_02248728(param0);

    for (v0 = 0; v0 < 16; v0++) {
        if (param0->unk_10[v0].unk_C8 == 0) {
            ov6_02248610(&param0->unk_10[v0], param0->unk_CE0->playerAvatar, param0->unk_CE8, param0->unk_CEC);
            return;
        }
    }

    GF_ASSERT(0);
}

static void ov6_022487E0 (UnkStruct_ov6_022487F8 * param0)
{
    int v0;

    for (v0 = 0; v0 < 16; v0++) {
        ov6_02248678(&param0->unk_10[v0]);
    }
}

static void ov6_022487F8 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_022487F8 * v0 = param2;
    int v1;
    NARC * v2;

    memset(v0, 0, sizeof(UnkStruct_ov6_022487F8));
    Heap_FndInitAllocatorForExpHeap(&v0->unk_CFC, 4, 32);

    v2 = NARC_ctor(NARC_INDEX_GRAPHIC__HIDEN_EFFECT, 4);

    sub_020170D8(&v0->unk_00, v2, 8, 4);

    for (v1 = 0; v1 < 4; v1++) {
        v0->unk_CD0[v1] = sub_0200723C(v2, 4 + v1, 0, 4, 0);
    }

    for (v1 = 0; v1 < 16; v1++) {
        ov6_022486B4(&v0->unk_10[v1], &v0->unk_00, &v0->unk_CFC, v0->unk_CD0);
    }

    v0->unk_CE0 = param1;
    sub_0205EAEC(v0->unk_CE0->playerAvatar, &v0->unk_CF0);

    v0->unk_D0C = 0;
    NARC_dtor(v2);
}

static void ov6_0224889C (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_022487F8 * v0 = param2;
    int v1;

    for (v1 = 0; v1 < 16; v1++) {
        ov6_02248700(&v0->unk_10[v1], &v0->unk_CFC);
    }

    sub_02017110(&v0->unk_00);

    for (v1 = 0; v1 < 4; v1++) {
        Heap_FreeToHeap(v0->unk_CD0[v1]);
    }
}

static void ov6_022488D8 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_022487F8 * v0 = param2;

    switch (v0->unk_D0C) {
    case 0:
        v0->unk_CE4--;

        if (v0->unk_CE4 < 0) {
            v0->unk_CE4 = 4;
            ov6_02248798(v0);
        }

        ov6_022487E0(v0);
        break;
    default:
        break;
    }
}

static void ov6_02248914 (UnkStruct_ov5_021D1BEC * param0, FieldSystem * param1, void * param2)
{
    UnkStruct_ov6_022487F8 * v0 = param2;
    int v1;

    for (v1 = 0; v1 < 16; v1++) {
        sub_02017294(&v0->unk_10[v1].unk_00);
    }
}

UnkStruct_ov5_021D1BEC * ov6_0224892C (FieldSystem * param0)
{
    static const UnkStruct_ov6_0223E6EC v0 = {
        1024,
        sizeof(UnkStruct_ov6_022487F8),
        ov6_022487F8,
        ov6_0224889C,
        ov6_022488D8,
        ov6_02248914
    };
    UnkStruct_ov5_021D1BEC * v1;

    v1 = ov5_021D1B6C(param0->unk_04->unk_04, &v0);
    return v1;
}

void ov6_02248940 (UnkStruct_ov5_021D1BEC * param0)
{
    ov5_021D1BEC(param0);
}
