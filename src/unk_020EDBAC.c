#include <nitro.h>
#include <string.h>

#include "constdata/const_020EE4B8.h"

#include "struct_decls/struct_02061AB4_decl.h"

#include "constdata/const_020EE31C.h"
#include "constdata/const_020EE3A8.h"

#include "struct_defs/struct_020EDF0C.h"

#include "unk_02061804.h"
#include "unk_0206450C.h"
#include "unk_020677F4.h"
#include "unk_02069BE0.h"

extern int (*const Unk_020EEC78[])(LocalMapObject *);
extern int (*const Unk_020EEC48[])(LocalMapObject *);
extern int (*const Unk_020EEC88[])(LocalMapObject *);
extern int (*const Unk_020EEC80[])(LocalMapObject *);
extern int (*const Unk_020EF59C[])(LocalMapObject *);
extern int (*const Unk_020EF590[])(LocalMapObject *);
extern int (*const Unk_020EF584[])(LocalMapObject *);
extern int (*const Unk_020EF578[])(LocalMapObject *);
extern int (*const Unk_020EF56C[])(LocalMapObject *);
extern int (*const Unk_020EF560[])(LocalMapObject *);
extern int (*const Unk_020EF554[])(LocalMapObject *);
extern int (*const Unk_020EF548[])(LocalMapObject *);
extern int (*const Unk_020EF53C[])(LocalMapObject *);
extern int (*const Unk_020EF530[])(LocalMapObject *);
extern int (*const Unk_020EF524[])(LocalMapObject *);
extern int (*const Unk_020EF518[])(LocalMapObject *);
extern int (*const Unk_020EF50C[])(LocalMapObject *);
extern int (*const Unk_020EF500[])(LocalMapObject *);
extern int (*const Unk_020EF4F4[])(LocalMapObject *);
extern int (*const Unk_020EF4E8[])(LocalMapObject *);
extern int (*const Unk_020EF4DC[])(LocalMapObject *);
extern int (*const Unk_020EF4D0[])(LocalMapObject *);
extern int (*const Unk_020EF4C4[])(LocalMapObject *);
extern int (*const Unk_020EF4B8[])(LocalMapObject *);
extern int (*const Unk_020EF4AC[])(LocalMapObject *);
extern int (*const Unk_020EF4A0[])(LocalMapObject *);
extern int (*const Unk_020EF494[])(LocalMapObject *);
extern int (*const Unk_020EF488[])(LocalMapObject *);
extern int (*const Unk_020EF194[])(LocalMapObject *);
extern int (*const Unk_020EF224[])(LocalMapObject *);
extern int (*const Unk_020EF440[])(LocalMapObject *);
extern int (*const Unk_020EF470[])(LocalMapObject *);
extern int (*const Unk_020EF47C[])(LocalMapObject *);
extern int (*const Unk_020EF230[])(LocalMapObject *);
extern int (*const Unk_020EF458[])(LocalMapObject *);
extern int (*const Unk_020EF464[])(LocalMapObject *);
extern int (*const Unk_020EF5A8[])(LocalMapObject *);
extern int (*const Unk_020EF5B4[])(LocalMapObject *);
extern int (*const Unk_020EF434[])(LocalMapObject *);
extern int (*const Unk_020EF428[])(LocalMapObject *);
extern int (*const Unk_020EF41C[])(LocalMapObject *);
extern int (*const Unk_020EF410[])(LocalMapObject *);
extern int (*const Unk_020EF404[])(LocalMapObject *);
extern int (*const Unk_020EF3F8[])(LocalMapObject *);
extern int (*const Unk_020EF3EC[])(LocalMapObject *);
extern int (*const Unk_020EF3E0[])(LocalMapObject *);
extern int (*const Unk_020EF3D4[])(LocalMapObject *);
extern int (*const Unk_020EF3C8[])(LocalMapObject *);
extern int (*const Unk_020EF3BC[])(LocalMapObject *);
extern int (*const Unk_020EF3B0[])(LocalMapObject *);
extern int (*const Unk_020EF3A4[])(LocalMapObject *);
extern int (*const Unk_020EF398[])(LocalMapObject *);
extern int (*const Unk_020EF38C[])(LocalMapObject *);
extern int (*const Unk_020EF380[])(LocalMapObject *);
extern int (*const Unk_020EF374[])(LocalMapObject *);
extern int (*const Unk_020EF368[])(LocalMapObject *);
extern int (*const Unk_020EF35C[])(LocalMapObject *);
extern int (*const Unk_020EF350[])(LocalMapObject *);
extern int (*const Unk_020EF344[])(LocalMapObject *);
extern int (*const Unk_020EF338[])(LocalMapObject *);
extern int (*const Unk_020EF32C[])(LocalMapObject *);
extern int (*const Unk_020EF320[])(LocalMapObject *);
extern int (*const Unk_020EF314[])(LocalMapObject *);
extern int (*const Unk_020EF308[])(LocalMapObject *);
extern int (*const Unk_020EF2FC[])(LocalMapObject *);
extern int (*const Unk_020EF2F0[])(LocalMapObject *);
extern int (*const Unk_020EF2E4[])(LocalMapObject *);
extern int (*const Unk_020EF2D8[])(LocalMapObject *);
extern int (*const Unk_020EEC98[])(LocalMapObject *);
extern int (*const Unk_020EF2C0[])(LocalMapObject *);
extern int (*const Unk_020EF2B4[])(LocalMapObject *);
extern int (*const Unk_020EF2A8[])(LocalMapObject *);
extern int (*const Unk_020EF020[])(LocalMapObject *);
extern int (*const Unk_020EF17C[])(LocalMapObject *);
extern int (*const Unk_020EF1A0[])(LocalMapObject *);
extern int (*const Unk_020EF44C[])(LocalMapObject *);
extern int (*const Unk_020EF29C[])(LocalMapObject *);
extern int (*const Unk_020EF290[])(LocalMapObject *);
extern int (*const Unk_020EF284[])(LocalMapObject *);
extern int (*const Unk_020EF278[])(LocalMapObject *);
extern int (*const Unk_020EF26C[])(LocalMapObject *);
extern int (*const Unk_020EF260[])(LocalMapObject *);
extern int (*const Unk_020EF254[])(LocalMapObject *);
extern int (*const Unk_020EF248[])(LocalMapObject *);
extern int (*const Unk_020EF23C[])(LocalMapObject *);
extern int (*const Unk_020EEC70[])(LocalMapObject *);
extern int (*const Unk_020EEC68[])(LocalMapObject *);
extern int (*const Unk_020EEC50[])(LocalMapObject *);
extern int (*const Unk_020EEC90[])(LocalMapObject *);
extern int (*const Unk_020EEC58[])(LocalMapObject *);
extern int (*const Unk_020EEC60[])(LocalMapObject *);
extern int (*const Unk_020EF218[])(LocalMapObject *);
extern int (*const Unk_020EF20C[])(LocalMapObject *);
extern int (*const Unk_020EF200[])(LocalMapObject *);
extern int (*const Unk_020EF1F4[])(LocalMapObject *);
extern int (*const Unk_020EF1E8[])(LocalMapObject *);
extern int (*const Unk_020EF1DC[])(LocalMapObject *);
extern int (*const Unk_020EF1D0[])(LocalMapObject *);
extern int (*const Unk_020EF1C4[])(LocalMapObject *);
extern int (*const Unk_020EF1B8[])(LocalMapObject *);
extern int (*const Unk_020EF188[])(LocalMapObject *);
extern int (*const Unk_020EF5C0[])(LocalMapObject *);
extern int (*const Unk_020EF164[])(LocalMapObject *);
extern int (*const Unk_020EF158[])(LocalMapObject *);
extern int (*const Unk_020EF14C[])(LocalMapObject *);
extern int (*const Unk_020EF140[])(LocalMapObject *);
extern int (*const Unk_020EF134[])(LocalMapObject *);
extern int (*const Unk_020EF128[])(LocalMapObject *);
extern int (*const Unk_020EF11C[])(LocalMapObject *);
extern int (*const Unk_020EF110[])(LocalMapObject *);
extern int (*const Unk_020EF104[])(LocalMapObject *);
extern int (*const Unk_020EF0F8[])(LocalMapObject *);
extern int (*const Unk_020EF0EC[])(LocalMapObject *);
extern int (*const Unk_020EF0E0[])(LocalMapObject *);
extern int (*const Unk_020EF0D4[])(LocalMapObject *);
extern int (*const Unk_020EF0C8[])(LocalMapObject *);
extern int (*const Unk_020EF0BC[])(LocalMapObject *);
extern int (*const Unk_020EF2CC[])(LocalMapObject *);
extern int (*const Unk_020EF0A4[])(LocalMapObject *);
extern int (*const Unk_020EF098[])(LocalMapObject *);
extern int (*const Unk_020EF08C[])(LocalMapObject *);
extern int (*const Unk_020EF080[])(LocalMapObject *);
extern int (*const Unk_020EF074[])(LocalMapObject *);
extern int (*const Unk_020EF068[])(LocalMapObject *);
extern int (*const Unk_020EF05C[])(LocalMapObject *);
extern int (*const Unk_020EF050[])(LocalMapObject *);
extern int (*const Unk_020EF044[])(LocalMapObject *);
extern int (*const Unk_020EF038[])(LocalMapObject *);
extern int (*const Unk_020EF014[])(LocalMapObject *);
extern int (*const Unk_020EF5CC[])(LocalMapObject *);
extern int (*const Unk_020EEFF0[])(LocalMapObject *);
extern int (*const Unk_020EEFE4[])(LocalMapObject *);
extern int (*const Unk_020EEFD8[])(LocalMapObject *);
extern int (*const Unk_020EEFCC[])(LocalMapObject *);
extern int (*const Unk_020EEFC0[])(LocalMapObject *);
extern int (*const Unk_020EEFB4[])(LocalMapObject *);
extern int (*const Unk_020EEFA8[])(LocalMapObject *);
extern int (*const Unk_020EF0B0[])(LocalMapObject *);
extern int (*const Unk_020EEF90[])(LocalMapObject *);
extern int (*const Unk_020EEF84[])(LocalMapObject *);
extern int (*const Unk_020EEF6C[])(LocalMapObject *);
extern int (*const Unk_020EEF78[])(LocalMapObject *);
extern int (*const Unk_020EF008[])(LocalMapObject *);
extern int (*const Unk_020EF02C[])(LocalMapObject *);
extern int (*const Unk_020EF1AC[])(LocalMapObject *);
extern int (*const Unk_020EF5D8[])(LocalMapObject *);
extern int (*const Unk_020EECBC[])(LocalMapObject *);
extern int (*const Unk_020EECB0[])(LocalMapObject *);
extern int (*const Unk_020EECA4[])(LocalMapObject *);
extern int (*const Unk_020EEF9C[])(LocalMapObject *);
extern int (*const Unk_020EEF60[])(LocalMapObject *);
extern int (*const Unk_020EEFFC[])(LocalMapObject *);
extern int (*const Unk_020EF170[])(LocalMapObject *);
extern int (*const Unk_020EF5E4[])(LocalMapObject *);


static const UnkStruct_020EDF0C Unk_020EDF0C = {
    0x0,
    sub_020633E0,
    sub_020633E4,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDE58 = {
    0x1,
    sub_020633E0,
    sub_020633E4,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE308 = {
    0x2,
    sub_0206453C,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE2F4 = {
    0x3,
    sub_02064628,
    sub_02064690,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE2E0 = {
    0x3,
    sub_02064638,
    sub_02064690,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDEE4 = {
    0x3,
    sub_02064648,
    sub_02064690,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE2B8 = {
    0x3,
    sub_02064658,
    sub_02064690,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDE94 = {
    0x3,
    sub_02064548,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDEBC = {
    0x3,
    sub_02064554,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDED0 = {
    0x3,
    sub_02064560,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE268 = {
    0x3,
    sub_0206456C,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE254 = {
    0x3,
    sub_02064578,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE240 = {
    0x3,
    sub_02064584,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDF70 = {
    0x3,
    sub_02064590,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE218 = {
    0x3,
    sub_0206459C,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE204 = {
    0x3,
    sub_020645A8,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDFC0 = {
    0x3,
    sub_020645B4,
    sub_020645C0,
    sub_02064624,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDFD4 = {
    0x3,
    sub_0206493C,
    sub_02064918,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE1C8 = {
    0x3,
    sub_02064948,
    sub_02064918,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE1B4 = {
    0x3,
    sub_02064954,
    sub_02064918,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE1A0 = {
    0x3,
    sub_02064960,
    sub_02064918,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE024 = {
    0x3,
    sub_02064990,
    sub_020649A8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE178 = {
    0x3,
    sub_0206499C,
    sub_020649A8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE164 = {
    0x3,
    sub_02064AF0,
    sub_02064AFC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE060 = {
    0x3,
    sub_02064C28,
    sub_02064C48,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE074 = {
    0x3,
    sub_02064DC8,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE128 = {
    0x3,
    sub_02064DD8,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE114 = {
    0x3,
    sub_02064DE8,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE0B0 = {
    0x3,
    sub_02064DF8,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE0C4 = {
    0x3,
    sub_02064E08,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE0EC = {
    0x3,
    sub_02064E18,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE100 = {
    0x3,
    sub_02064E28,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE13C = {
    0x3,
    sub_02064E38,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE150 = {
    0x3,
    sub_02064E48,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE18C = {
    0x3,
    sub_02064E58,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE1DC = {
    0x3,
    sub_02064E68,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE1F0 = {
    0x3,
    sub_02064E78,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE22C = {
    0x3,
    sub_02064E88,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE27C = {
    0x3,
    sub_02064E98,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE290 = {
    0x3,
    sub_02064EA8,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE2A4 = {
    0x3,
    sub_02064EB8,
    sub_02064EC8,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE2CC = {
    0x3,
    sub_0206505C,
    sub_020650DC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDFE8 = {
    0x3,
    sub_0206506C,
    sub_020650DC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDDCC = {
    0x3,
    sub_0206507C,
    sub_020650DC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDDE0 = {
    0x3,
    sub_0206508C,
    sub_020650DC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDDF4 = {
    0x3,
    sub_0206509C,
    sub_020650DC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDF98 = {
    0x3,
    sub_020650AC,
    sub_020650DC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDE08 = {
    0x3,
    sub_020650BC,
    sub_020650DC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDE1C = {
    0x3,
    sub_020650CC,
    sub_020650DC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDE30 = {
    0x2f,
    sub_02067840,
    sub_02067850,
    sub_0206786C,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDF48 = {
    0x3,
    sub_02069BE0,
    sub_02069C0C,
    sub_02069C44,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDE44 = {
    0x3,
    sub_02069DF4,
    sub_02069E1C,
    sub_02069E4C,
    sub_02069E50
};

static const UnkStruct_020EDF0C Unk_020EDF20 = {
    0x33,
    sub_0206A104,
    sub_0206A134,
    sub_0206A158,
    sub_0206A168
};

static const UnkStruct_020EDF0C Unk_020EDE6C = {
    0x34,
    sub_0206A110,
    sub_0206A134,
    sub_0206A158,
    sub_0206A168
};

static const UnkStruct_020EDF0C Unk_020EDEF8 = {
    0x35,
    sub_0206A11C,
    sub_0206A134,
    sub_0206A158,
    sub_0206A168
};

static const UnkStruct_020EDF0C Unk_020EDE80 = {
    0x36,
    sub_0206A128,
    sub_0206A134,
    sub_0206A158,
    sub_0206A168
};

static const UnkStruct_020EDF0C Unk_020EDEA8 = {
    0x37,
    sub_0206A25C,
    sub_0206A2BC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDF34 = {
    0x38,
    sub_0206A268,
    sub_0206A2BC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDF5C = {
    0x39,
    sub_0206A274,
    sub_0206A2BC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDF84 = {
    0x3A,
    sub_0206A280,
    sub_0206A2BC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDFAC = {
    0x3B,
    sub_0206A28C,
    sub_0206A2BC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EDFFC = {
    0x3C,
    sub_0206A298,
    sub_0206A2BC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE010 = {
    0x3D,
    sub_0206A2A4,
    sub_0206A2BC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE038 = {
    0x3E,
    sub_0206A2B0,
    sub_0206A2BC,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE04C = {
    0x3f,
    sub_0206A490,
    sub_0206A4C0,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE088 = {
    0x40,
    sub_0206A49C,
    sub_0206A4C0,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE09C = {
    0x41,
    sub_0206A4A8,
    sub_0206A4C0,
    sub_020633E8,
    sub_020633EC
};

static const UnkStruct_020EDF0C Unk_020EE0D8 = {
    0x42,
    sub_0206A4B4,
    sub_0206A4C0,
    sub_020633E8,
    sub_020633EC
};

const UnkStruct_020EDF0C * const Unk_020EE3A8[] = {
    &Unk_020EDF0C,
    &Unk_020EDE58,
    &Unk_020EE308,
    &Unk_020EE2F4,
    &Unk_020EE2E0,
    &Unk_020EDEE4,
    &Unk_020EDE94,
    &Unk_020EDEBC,
    &Unk_020EDED0,
    &Unk_020EE268,
    &Unk_020EE254,
    &Unk_020EE240,
    &Unk_020EDF70,
    &Unk_020EE218,
    &Unk_020EDFD4,
    &Unk_020EE1C8,
    &Unk_020EE1B4,
    &Unk_020EE1A0,
    &Unk_020EE024,
    &Unk_020EE178,
    &Unk_020EE060,
    &Unk_020EE074,
    &Unk_020EE128,
    &Unk_020EE114,
    &Unk_020EE0B0,
    &Unk_020EE0C4,
    &Unk_020EE0EC,
    &Unk_020EE100,
    &Unk_020EE13C,
    &Unk_020EE150,
    &Unk_020EE18C,
    &Unk_020EE1DC,
    &Unk_020EE1F0,
    &Unk_020EE22C,
    &Unk_020EE27C,
    &Unk_020EE290,
    &Unk_020EE2A4,
    &Unk_020EE2CC,
    &Unk_020EDFE8,
    &Unk_020EDDCC,
    &Unk_020EDDE0,
    &Unk_020EDDF4,
    &Unk_020EDF98,
    &Unk_020EDE08,
    &Unk_020EDE1C,
    &Unk_020EE204,
    &Unk_020EDFC0,
    &Unk_020EDE30,
    &Unk_020EDF48,
    &Unk_020EE164,
    &Unk_020EDE44,
    &Unk_020EDF20,
    &Unk_020EDE6C,
    &Unk_020EDEF8,
    &Unk_020EDE80,
    &Unk_020EDEA8,
    &Unk_020EDF34,
    &Unk_020EDF5C,
    &Unk_020EDF84,
    &Unk_020EDFAC,
    &Unk_020EDFFC,
    &Unk_020EE010,
    &Unk_020EE038,
    &Unk_020EE04C,
    &Unk_020EE088,
    &Unk_020EE09C,
    &Unk_020EE0D8,
    &Unk_020EE2B8
};

int (*const * const Unk_020EE4B8[0x9a])(LocalMapObject *) = {
    Unk_020EEC78,
    Unk_020EEC48,
    Unk_020EEC88,
    Unk_020EEC80,
    Unk_020EF59C,
    Unk_020EF590,
    Unk_020EF584,
    Unk_020EF578,
    Unk_020EF56C,
    Unk_020EF560,
    Unk_020EF554,
    Unk_020EF548,
    Unk_020EF53C,
    Unk_020EF530,
    Unk_020EF524,
    Unk_020EF518,
    Unk_020EF50C,
    Unk_020EF500,
    Unk_020EF4F4,
    Unk_020EF4E8,
    Unk_020EF4DC,
    Unk_020EF4D0,
    Unk_020EF4C4,
    Unk_020EF4B8,
    Unk_020EF47C,
    Unk_020EF230,
    Unk_020EF458,
    Unk_020EF464,
    Unk_020EF5A8,
    Unk_020EF5B4,
    Unk_020EF434,
    Unk_020EF428,
    Unk_020EF41C,
    Unk_020EF410,
    Unk_020EF404,
    Unk_020EF3F8,
    Unk_020EF3EC,
    Unk_020EF3E0,
    Unk_020EF3D4,
    Unk_020EF3C8,
    Unk_020EF3BC,
    Unk_020EF3B0,
    Unk_020EF3A4,
    Unk_020EF398,
    Unk_020EF38C,
    Unk_020EF380,
    Unk_020EF374,
    Unk_020EF368,
    Unk_020EF35C,
    Unk_020EF350,
    Unk_020EF344,
    Unk_020EF338,
    Unk_020EF32C,
    Unk_020EF320,
    Unk_020EF314,
    Unk_020EF308,
    Unk_020EF2FC,
    Unk_020EF2F0,
    Unk_020EF2E4,
    Unk_020EF2D8,
    Unk_020EF29C,
    Unk_020EF290,
    Unk_020EF284,
    Unk_020EF278,
    Unk_020EF26C,
    Unk_020EF260,
    Unk_020EF254,
    Unk_020EF248,
    Unk_020EF23C,
    Unk_020EEC70,
    Unk_020EEC68,
    Unk_020EEC50,
    Unk_020EEC90,
    Unk_020EEC58,
    Unk_020EEC60,
    Unk_020EF218,
    Unk_020EF200,
    Unk_020EF1F4,
    Unk_020EF1E8,
    Unk_020EF1DC,
    Unk_020EF1D0,
    Unk_020EF1C4,
    Unk_020EF1B8,
    Unk_020EF188,
    Unk_020EF4AC,
    Unk_020EF4A0,
    Unk_020EF494,
    Unk_020EF488,
    Unk_020EF194,
    Unk_020EF224,
    Unk_020EF440,
    Unk_020EF470,
    Unk_020EEC98,
    Unk_020EF2C0,
    Unk_020EF2B4,
    Unk_020EF2A8,
    Unk_020EF5C0,
    Unk_020EF164,
    Unk_020EF158,
    Unk_020EF14C,
    Unk_020EF140,
    Unk_020EF134,
    Unk_020EF128,
    Unk_020EF20C,
    Unk_020EF11C,
    Unk_020EF110,
    Unk_020EF104,
    Unk_020EF0F8,
    Unk_020EF0EC,
    Unk_020EF0E0,
    Unk_020EF0D4,
    Unk_020EF0C8,
    Unk_020EF0BC,
    Unk_020EF2CC,
    Unk_020EF0A4,
    Unk_020EF098,
    Unk_020EF08C,
    Unk_020EF020,
    Unk_020EF17C,
    Unk_020EF1A0,
    Unk_020EF44C,
    Unk_020EF5CC,
    Unk_020EEFF0,
    Unk_020EEFE4,
    Unk_020EEFD8,
    Unk_020EEFCC,
    Unk_020EEFC0,
    Unk_020EEFB4,
    Unk_020EEFA8,
    Unk_020EF0B0,
    Unk_020EEF90,
    Unk_020EEF84,
    Unk_020EEF6C,
    Unk_020EEF78,
    Unk_020EF008,
    Unk_020EF02C,
    Unk_020EF1AC,
    Unk_020EF5D8,
    Unk_020EECBC,
    Unk_020EECB0,
    Unk_020EECA4,
    Unk_020EEF9C,
    Unk_020EEF60,
    Unk_020EEFFC,
    Unk_020EF170,
    Unk_020EF080,
    Unk_020EF074,
    Unk_020EF068,
    Unk_020EF05C,
    Unk_020EF050,
    Unk_020EF044,
    Unk_020EF038,
    Unk_020EF014,
    Unk_020EF5E4
};

static const int Unk_020EDD5C[] = {
    0x0,
    0x1,
    0x2,
    0x3
};

static const int Unk_020EDCDC[] = {
    0x4,
    0x5,
    0x6,
    0x7
};

static const int Unk_020EDC5C[] = {
    0x8,
    0x9,
    0xA,
    0xB
};

static const int Unk_020EDCEC[] = {
    0xC,
    0xD,
    0xE,
    0xf
};

static const int Unk_020EDD2C[] = {
    0x10,
    0x11,
    0x12,
    0x13
};

static const int Unk_020EDCFC[] = {
    0x14,
    0x15,
    0x16,
    0x17
};

static const int Unk_020EDC3C[] = {
    0x18,
    0x19,
    0x1A,
    0x1B
};

static const int Unk_020EDBAC[] = {
    0x1C,
    0x1D,
    0x1E,
    0x1f
};

static const int Unk_020EDD0C[] = {
    0x20,
    0x21,
    0x22,
    0x23
};

static const int Unk_020EDD9C[] = {
    0x24,
    0x25,
    0x26,
    0x27
};

static const int Unk_020EDC8C[] = {
    0x28,
    0x29,
    0x2A,
    0x2B
};

static const int Unk_020EDDBC[] = {
    0x2C,
    0x2D,
    0x2E,
    0x2f
};

static const int Unk_020EDC9C[] = {
    0x30,
    0x31,
    0x32,
    0x33
};

static const int Unk_020EDCBC[] = {
    0x34,
    0x35,
    0x36,
    0x37
};

static const int Unk_020EDBFC[] = {
    0x38,
    0x39,
    0x3A,
    0x3B
};

static const int Unk_020EDC7C[] = {
    0x4C,
    0x4D,
    0x4E,
    0x4f
};

static const int Unk_020EDD7C[] = {
    0x50,
    0x51,
    0x52,
    0x53
};

static const int Unk_020EDC6C[] = {
    0x54,
    0x55,
    0x56,
    0x57
};

static const int Unk_020EDBEC[] = {
    0x58,
    0x59,
    0x5A,
    0x5B
};

static const int Unk_020EDD3C[] = {
    0x5C,
    0x5D,
    0x5C,
    0x5D
};

static const int Unk_020EDC4C[] = {
    0x5E,
    0x5f,
    0x5E,
    0x5f
};

static const int Unk_020EDBDC[] = {
    0x60,
    0x61,
    0x62,
    0x63
};

static const int Unk_020EDC2C[] = {
    0x75,
    0x76,
    0x77,
    0x78
};

static const int Unk_020EDCCC[] = {
    0x69,
    0x6A,
    0x6B,
    0x6C
};

static const int Unk_020EDC0C[] = {
    0x6D,
    0x6E,
    0x6f,
    0x70
};

static const int Unk_020EDCAC[] = {
    0x71,
    0x72,
    0x73,
    0x74
};

static const int Unk_020EDC1C[] = {
    0x91,
    0x92,
    0x93,
    0x94
};

static const int Unk_020EDD8C[] = {
    0x95,
    0x96,
    0x97,
    0x98
};

static const int Unk_020EDD4C[] = {
    0x79,
    0x7A,
    0x7B,
    0x7C
};

static const int Unk_020EDD1C[] = {
    0x7D,
    0x7E,
    0x7f,
    0x80
};

static const int Unk_020EDD6C[] = {
    0x81,
    0x82,
    0x83,
    0x84
};

static const int Unk_020EDBCC[] = {
    0x85,
    0x86,
    0x87,
    0x88
};

static const int Unk_020EDDAC[] = {
    0x89,
    0x8A,
    0x8B,
    0x8C
};

static const int Unk_020EDBBC[] = {
    0x8D,
    0x8E,
    0x8f,
    0x90
};

const int * const Unk_020EE31C[] = {
    Unk_020EDD5C,
    Unk_020EDCDC,
    Unk_020EDC5C,
    Unk_020EDCEC,
    Unk_020EDD2C,
    Unk_020EDCFC,
    Unk_020EDC3C,
    Unk_020EDBAC,
    Unk_020EDD0C,
    Unk_020EDD9C,
    Unk_020EDC8C,
    Unk_020EDDBC,
    Unk_020EDC9C,
    Unk_020EDCBC,
    Unk_020EDBFC,
    Unk_020EDC7C,
    Unk_020EDD7C,
    Unk_020EDC6C,
    Unk_020EDBEC,
    Unk_020EDD3C,
    Unk_020EDC4C,
    Unk_020EDBDC,
    Unk_020EDCCC,
    Unk_020EDC0C,
    Unk_020EDC2C,
    Unk_020EDCAC,
    Unk_020EDD4C,
    Unk_020EDD1C,
    Unk_020EDD6C,
    Unk_020EDBCC,
    Unk_020EDDAC,
    Unk_020EDBBC,
    Unk_020EDC1C,
    Unk_020EDD8C,
    NULL
};
