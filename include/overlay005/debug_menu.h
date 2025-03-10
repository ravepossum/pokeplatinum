#ifndef POKEPLATINUM_DEBUG_MENU_H
#define POKEPLATINUM_DEBUG_MENU_H

#include "field/field_system_decl.h"

#include "list_menu.h"
#include "message.h"
#include "string_template.h"
#include "sys_task_manager.h"

#define DEBUG_KEY               PAD_BUTTON_R
#define DEBUG_FLAG_NO_COLLISION 2549

typedef struct DebugMenu {
    u16 data;
    u16 debugList;
    u16 cursor;
    ListMenu *listMenu;
    FieldSystem *sys;
    Window *window;
    StringList *stringList;
    void (*callback)(FieldSystem *);
} DebugMenu;

typedef void (*DebugFunction)(SysTask *, DebugMenu *);

typedef struct DebugMenuItem {
    u32 index;
    DebugFunction function;
} DebugMenuItem;

enum DebugItem {
    DEBUG_ITEM_NONE,
    DEBUG_ITEM_FLY,
    DEBUG_ITEM_CREATE_MON,
    DEBUG_ITEM_EDIT_MON,
    DEBUG_ITEM_TOGGLE_COLLISION,
    DEBUG_ITEM_ADJUST_CAMERA,
    DEBUG_ITEM_EXECUTE_FUNCTION,
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
