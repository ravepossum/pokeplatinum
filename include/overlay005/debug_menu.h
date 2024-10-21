#ifndef POKEPLATINUM_DEBUG_MENU_H
#define POKEPLATINUM_DEBUG_MENU_H

#include "struct_decls/struct_0200112C_decl.h"
#include "struct_decls/struct_02013A04_decl.h"
#include "struct_decls/struct_020149F0_decl.h"
#include "struct_defs/struct_0205AA50.h"

#include "field/field_system_decl.h"

#include "message.h"
#include "string_template.h"
#include "sys_task_manager.h"

#define DEBUG_KEY                 PAD_BUTTON_R
#define DEBUG_MENU_DUMMY_FUNCTION 0xFFFFFFFD
#define DEBUG_MENU_MESSAGE_BANK   328
#define DEBUG_FLAG_NO_COLLISION   2549

typedef struct DebugMenu {
    u8 data;
    u16 debugList;
    u16 cursor;
    BmpList *bmpList;
    FieldSystem *sys;
    Window *window;
    ResourceMetadata *menuList;
    void (*callback)(FieldSystem *);
} DebugMenu;

typedef struct DebugMenuItem {
    u32 index;
    u32 function;
} DebugMenuItem;

enum DebugItems {
    DEBUG_HEADER = 1,
    DEBUG_FLY,
    DEBUG_CREATE_MON,
    DEBUG_EDIT_MON,
    DEBUG_TOGGLE_COLLISION,
};

typedef struct DebugFly {
    FieldSystem *sys;
    void *data;
    SysTask *taskCutIn;
    int sequence;
    int wipeFunc;
} DebugFly;

void DebugMenu_Init(FieldSystem *sys);

#endif // POKEPLATINUM_DEBUG_MENU_H
