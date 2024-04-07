#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_020216E0_decl.h"
#include "struct_decls/struct_02061AB4_decl.h"
#include "overlay005/struct_ov5_021ECB8C_decl.h"

#include "overlay005/struct_ov5_021ED01C.h"

#include "unk_02020AEC.h"
#include "unk_02061804.h"
#include "overlay005/ov5_021ECA70.h"
#include "overlay005/ov5_021ECE40.h"

typedef struct UnkStruct_ov5_021ECB8C_t {
    UnkStruct_020216E0 * unk_00;
    UnkStruct_ov5_021ED01C unk_04;
};

static void ov5_021ECB90(LocalMapObject * param0);
static void ov5_021ECBA4(LocalMapObject * param0);
static void ov5_021ECBCC(LocalMapObject * param0, UnkStruct_020216E0 * param1);
static void ov5_021ECC0C(LocalMapObject * param0);

void ov5_021ECA70 (LocalMapObject * param0)
{
    VecFx32 v0;

    ov5_021ECB90(param0);

    v0.x = 0;
    v0.y = (FX32_CONST(0));
    v0.z = 0;

    sub_020630AC(param0, &v0);
}

void ov5_021ECA90 (LocalMapObject * param0)
{
    UnkStruct_ov5_021ECB8C * v0 = sub_02062AF0(param0);
    UnkStruct_020216E0 * v1 = v0->unk_00;

    if (ov5_021EDD94(param0) == 1) {
        return;
    }

    if (v1 == NULL) {
        return;
    }

    ov5_021ECBA4(param0);
    sub_02021368(v1, FX32_ONE);
}

void ov5_021ECABC (LocalMapObject * param0)
{
    ov5_021ECC0C(param0);
}

void ov5_021ECAC4 (LocalMapObject * param0)
{
    UnkStruct_ov5_021ECB8C * v0;

    v0 = sub_02062AF0(param0);

    if (v0->unk_00 != NULL) {
        ov5_021ED01C(v0->unk_00, &v0->unk_04);
    }

    ov5_021ECFA4(param0, &v0->unk_00);
    sub_020628BC(param0, (1 << 21));
}

void ov5_021ECAF0 (LocalMapObject * param0)
{
    UnkStruct_ov5_021ECB8C * v0;

    v0 = sub_02062AF0(param0);

    if (ov5_021EDD94(param0) == 1) {
        return;
    }

    if (v0->unk_00 == NULL) {
        ov5_021ECF04(param0, &v0->unk_00);
    }

    if (v0->unk_00 != NULL) {
        ov5_021ED03C(v0->unk_00, &v0->unk_04);
        ov5_021EDEB4(param0, v0->unk_00);
        sub_020628C4(param0, (1 << 21));
    }
}

void ov5_021ECB34 (LocalMapObject * param0)
{
    VecFx32 v0;

    ov5_021ECB90(param0);

    v0.x = 0;
    v0.y = (FX32_CONST(24));
    v0.z = 0;

    sub_020630AC(param0, &v0);
}

void ov5_021ECB58 (LocalMapObject * param0)
{
    UnkStruct_ov5_021ECB8C * v0 = sub_02062AF0(param0);
    UnkStruct_020216E0 * v1 = v0->unk_00;

    if (ov5_021EDD94(param0) == 1) {
        return;
    }

    if (v1 == NULL) {
        return;
    }

    ov5_021ECBA4(param0);
}

void ov5_021ECB78 (LocalMapObject * param0)
{
    ov5_021ECC0C(param0);
}

UnkStruct_020216E0 * ov5_021ECB80 (LocalMapObject * param0)
{
    UnkStruct_ov5_021ECB8C * v0 = sub_02062AF0(param0);
    return ov5_021ECB8C(v0);
}

UnkStruct_020216E0 * ov5_021ECB8C (UnkStruct_ov5_021ECB8C * param0)
{
    return param0->unk_00;
}

static void ov5_021ECB90 (LocalMapObject * param0)
{
    UnkStruct_ov5_021ECB8C * v0;

    v0 = sub_02062ACC(param0, (sizeof(UnkStruct_ov5_021ECB8C)));

    ov5_021ECF04(param0, &v0->unk_00);
}

static void ov5_021ECBA4 (LocalMapObject * param0)
{
    UnkStruct_ov5_021ECB8C * v0 = sub_02062AF0(param0);
    UnkStruct_020216E0 * v1 = v0->unk_00;

    ov5_021EDEB4(param0, v1);
    sub_02021368(v1, (FX32_ONE));
    ov5_021ECBCC(param0, v1);
}

static void ov5_021ECBCC (LocalMapObject * param0, UnkStruct_020216E0 * param1)
{
    int v0 = 1;

    if (sub_020628D8(param0, (1 << 9)) == 1) {
        v0 = 0;
    }

    if (sub_020628D8(param0, (1 << 12)) == 1) {
        if (sub_020628D8(param0, (1 << 13)) == 0) {
            v0 = 0;
        }
    }

    sub_02021320(param1, v0);
}

static void ov5_021ECC0C (LocalMapObject * param0)
{
    UnkStruct_ov5_021ECB8C * v0;

    v0 = sub_02062AF0(param0);
    ov5_021ECFA4(param0, &v0->unk_00);
}
