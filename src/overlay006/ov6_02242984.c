#include <nitro.h>
#include <string.h>

#include "inlines.h"

#include "field/field_system.h"
#include "struct_defs/struct_02049FA8.h"
#include "overlay006/struct_ov6_02242634.h"

#include "narc.h"
#include "heap.h"
#include "unk_0202631C.h"
#include "unk_0202D7A8.h"
#include "unk_02039C80.h"
#include "unk_0203A378.h"
#include "unk_0203CC84.h"
#include "unk_0205E7D0.h"
#include "overlay006/ov6_02240C9C.h"
#include "overlay006/ov6_02242984.h"
#include "overlay006/ov6_02242F74.h"
#include "overlay006/ov6_022477B8.h"

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov6_02242A10;

typedef struct UnkStruct_ov6_02242A8C_t {
    UnkStruct_ov6_02242A10 unk_00[6];
    UnkStruct_02049FA8 unk_18;
    FieldSystem * unk_2C;
    int unk_30;
} UnkStruct_ov6_02242A8C;

int ov6_02242984 (FieldSystem * param0)
{
    u8 v0;
    BOOL v1;
    int v2[12];
    UnkStruct_ov6_02242634 * v3;

    v3 = (UnkStruct_ov6_02242634 *)sub_0203A640(param0);

    for (v0 = 0; v0 < 12; v0++) {
        v2[v0] = v3->unk_04[v0].unk_04;
    }

    v1 = sub_02027474(sub_02027560(sub_0203D174(param0)));

    ov6_02242F74(sub_0202D814(sub_0202D834(param0->unk_0C), 1), v1, param0->unk_1C->unk_00, &v2[6], &v2[7]);
    ov6_02240C9C(v3, &v2[2], &v2[3]);
    ov6_022477B8(v3, v1, &v2[8], &v2[9]);

    return v2[inline_020564D0(12)];
}

UnkStruct_ov6_02242A8C * ov6_02242A10 (const int param0, FieldSystem * param1)
{
    u8 v0;
    UnkStruct_ov6_02242A8C * v1;

    v1 = Heap_AllocFromHeapAtEnd(param0, sizeof(UnkStruct_ov6_02242A8C));
    v1->unk_2C = param1;

    {
        u8 v2;
        UnkStruct_ov6_02242A10 * v3;

        v3 = NARC_AllocAtEndAndReadWholeMemberByIndexPair(NARC_INDEX_ARC__ENCDATA_EX, 11, 4);

        for (v0 = 0; v0 < 5; v0++) {
            v2 = inline_020564D0(36);
            v1->unk_00[v0].unk_00 = v3[v2].unk_00;
            v1->unk_00[v0].unk_02 = v3[v2].unk_02;
        }

        {
            v1->unk_00[5].unk_00 = sub_0205EABC(param1->unk_3C);
            v1->unk_00[5].unk_02 = sub_0205EAC8(param1->unk_3C);
            v1->unk_30 = param1->unk_1C->unk_00;
        }

        Heap_FreeToHeap(v3);
    }

    return v1;
}

void ov6_02242A8C (UnkStruct_ov6_02242A8C * param0)
{
    Heap_FreeToHeap(param0);
}

void ov6_02242A94 (const u8 param0, UnkStruct_ov6_02242A8C * param1)
{
    int v0;
    int v1, v2;

    if (param0 == 0) {
        int v3;

        v3 = 240;
        v1 = param1->unk_00[param0].unk_00 / 32;
        v2 = param1->unk_00[param0].unk_02 / 32;
        v0 = sub_02039F10(v3, v1, v2);
    } else if (param0 == 5) {
        v0 = param1->unk_30;
    } else {
        v1 = param1->unk_00[param0].unk_00 / 32;
        v2 = param1->unk_00[param0].unk_02 / 32;
        v0 = sub_02039E30(param1->unk_2C->unk_2C, v1, v2);
    }

    inline_02049FA8(&param1->unk_18, v0, -1, param1->unk_00[param0].unk_00, param1->unk_00[param0].unk_02, 0);
}

UnkStruct_02049FA8 * ov6_02242AEC (UnkStruct_ov6_02242A8C * param0)
{
    return &param0->unk_18;
}
