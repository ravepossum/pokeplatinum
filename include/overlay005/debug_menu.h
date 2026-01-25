#ifndef POKEPLATINUM_DEBUG_MENU_H
#define POKEPLATINUM_DEBUG_MENU_H

#include "constants/debug_menu.h"

#include "applications/town_map/main.h"
#include "field/field_system_decl.h"
#include "overlay005/sprite_resource_manager.h"

#include "list_menu.h"
#include "message.h"
#include "sprite_system.h"
#include "string_template.h"
#include "sys_task_manager.h"

#define DEBUG_TEXT_BLACK TEXT_COLOR(1, 2, 15)
#define DEBUG_TEXT_RED   TEXT_COLOR(3, 4, 15)
#define DEBUG_TEXT_GREEN TEXT_COLOR(5, 6, 15)
#define DEBUG_TEXT_BLUE  TEXT_COLOR(7, 8, 15)
#define DEBUG_TEXT_PINK  TEXT_COLOR(9, 10, 15)

typedef struct DebugMenu DebugMenu;
typedef struct DebugMenuItem DebugMenuItem;
typedef struct DebugSubMenu DebugSubMenu;
typedef struct DebugListNode DebugListNode;
typedef void (*DebugFunction)(SysTask *, DebugMenu *);
typedef void (*DebugSubMenuFunc)(DebugSubMenu *);

typedef struct DebugMenuItem {
    DebugFunction function;
    u32 name;
} DebugMenuItem;

typedef struct DebugMenu {
    DebugListNode *listNode;
    FieldSystem *fieldSystem;
    Window *window;
    u16 data;
    u16 listPos;
    u16 cursor;
} DebugMenu;

typedef struct DebugListNode {
    ListMenuTemplate template;
    DebugMenuItem *items;
    DebugMenu *debugMenu;
    DebugListNode *prev;
    ListMenu *listMenu;
    StringList *stringList;
} DebugListNode;

typedef struct DebugSubMenu {
    SpriteResourceManager spriteResMan;
    ManagedSprite *sprite;
    DebugMenu *debugMenu;
    Window *window;
    MessageLoader *msgLoader;
    StringTemplate *template;
    int data[MAX_SUBMENU_DATA];
    u32 type;
    int value;
    u32 digits;
} DebugSubMenu;

typedef struct DebugSubMenuConfig {
    DebugSubMenuFunc choiceFunc;
    DebugSubMenuFunc renderFunc;
    BOOL closeOnChoice;
    BOOL preserveSprite;
    int min;
    int max;
} DebugSubMenuConfig;

enum DebugMainItem {
    DEBUG_ITEM_FLY = 0,
    DEBUG_ITEM_CREATE_MON,
    DEBUG_ITEM_EDIT_MON,
    DEBUG_ITEM_ADD_ITEM,
    DEBUG_ITEM_FLAG_VAR_LIST,
    DEBUG_ITEM_ADJUST_CAMERA,
    DEBUG_ITEM_EXECUTE_FUNCTION,
    DEBUG_MAIN_ITEM_COUNT,
};

enum DebugFlagVarItem {
    DEBUG_ITEM_TOGGLE_COLLISION = 0,
    DEBUG_ITEM_TOGGLE_TRAINER_SEE,
    DEBUG_ITEM_TOGGLE_ENCOUNTERS,
    DEBUG_ITEM_SET_FLAG,
    DEBUG_ITEM_SET_VAR,
    DEBUG_FLAG_VAR_ITEM_COUNT,
};

enum DebugSubMenuType {
    DEBUG_SUBMENU_ADD_ITEM = 0,
    DEBUG_SUBMENU_ITEM_QUANTITY,
    DEBUG_SUBMENU_SET_FLAG,
    DEBUG_SUBMENU_SELECT_VAR,
    DEBUG_SUBMENU_VAR_VALUE,
    DEBUG_SUBMENU_TYPE_COUNT,
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

enum DebugTextState {
    DEBUG_TEXT_STATE_DEFAULT = -1,
    DEBUG_TEXT_STATE_INACTIVE,
    DEBUG_TEXT_STATE_ACTIVE,
};

typedef struct DebugFly {
    FieldSystem *fieldSystem;
    TownMapContext *mapCtx;
    SysTask *taskCutIn;
    int state;
} DebugFly;

void DebugMenu_Init(FieldSystem *fieldSystem);

#endif // POKEPLATINUM_DEBUG_MENU_H
