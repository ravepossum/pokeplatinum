#ifndef POKEPLATINUM_DEBUG_MENU_H
#define POKEPLATINUM_DEBUG_MENU_H

#include "generated/vars_flags.h"

#include "applications/town_map/main.h"
#include "field/field_system_decl.h"

#include "list_menu.h"
#include "message.h"
#include "string_template.h"
#include "sys_task_manager.h"

#define DEBUG_KEY               PAD_BUTTON_R
#define DEBUG_FLAG_NO_COLLISION FLAG_UNUSED_2420

typedef struct DebugMenu {
    u16 data;
    u16 listPos;
    u16 cursor;
    ListMenu *listMenu;
    FieldSystem *fieldSystem;
    Window *window;
    StringList *stringList;
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

enum DebugFlyState {
    DEBUG_FLY_FADE_OUT = 0,
    DEBUG_FLY_WAIT_FADE_AND_INIT,
    DEBUG_FLY_START_MAP,
    DEBUG_FLY_FADE_IN,
    DEBUG_FLY_WAIT_FADE_AND_CUTIN,
    DEBUG_FLY_WAIT_CUTIN_AND_START_FLY,
    DEBUG_FLY_EXIT,
};

typedef struct DebugFly {
    FieldSystem *fieldSystem;
    TownMapContext *mapCtx;
    SysTask *taskCutIn;
    int state;
} DebugFly;

void DebugMenu_Init(FieldSystem *fieldSystem);

#endif // POKEPLATINUM_DEBUG_MENU_H
