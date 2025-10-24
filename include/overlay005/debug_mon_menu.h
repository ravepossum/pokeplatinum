#ifndef POKEPLATINUM_DEBUG_MON_MENU_H
#define POKEPLATINUM_DEBUG_MON_MENU_H

#include "field/field_system_decl.h"

#include "list_menu.h"
#include "message.h"
#include "string_template.h"
#include "sys_task_manager.h"
#include "text.h"

#define DEBUG_MON_MENU_MESSAGE_BANK 336
#define DEBUG_MON_MENU_STATS_SIZE   64
#define DEBUG_MON_MENU_MAX_PAGES    8

#define DMM_COLOR_BLACK TEXT_COLOR(1, 2, 15)
#define DMM_COLOR_BLUE  TEXT_COLOR(3, 4, 15)
#define DMM_COLOR_RED   TEXT_COLOR(5, 6, 15)
#define DMM_COLOR_PINK  TEXT_COLOR(7, 8, 15)
#define DMM_COLOR_GREEN TEXT_COLOR(9, 10, 15)

enum DebugMonMenuMode {
    DEBUG_MON_MENU_MODE_CREATE = 0,
    DEBUG_MON_MENU_MODE_EDIT,
};

enum DebugMonMenuState {
    DMM_STATE_DRAW_MENU = 0,
    DMM_STATE_HANDLE_INPUT,
    DMM_STATE_HANDLE_VALUE_INPUT,
    DMM_STATE_WAIT_A_BUTTON_PRESS,
    DMM_STATE_EXIT_MENU,
    DMM_STATE_WAIT_XY_BUTTON_PRESS,
};

enum DebugMonMenuDirection {
    DMMD_BOTH = 0,
    DMMD_INCREASE,
    DMMD_DECREASE,
};

enum DebugMonStatID {
    DEBUG_MON_SPECIES = 0,
    DEBUG_MON_LEVEL,
    DEBUG_MON_EXP,
    DEBUG_MON_OT_ID,
    DEBUG_MON_PERSONALITY,
    DEBUG_MON_GENDER,
    DEBUG_MON_NATURE,
    DEBUG_MON_MOVE1,
    DEBUG_MON_MOVE2,
    DEBUG_MON_MOVE3,
    DEBUG_MON_MOVE4,
    DEBUG_MON_HELD_ITEM,
    DEBUG_MON_ABILITY,
    DEBUG_MON_HP_IV,
    DEBUG_MON_HP_EV,
    DEBUG_MON_ATK_IV,
    DEBUG_MON_ATK_EV,
    DEBUG_MON_DEF_IV,
    DEBUG_MON_DEF_EV,
    DEBUG_MON_SPEED_IV,
    DEBUG_MON_SPEED_EV,
    DEBUG_MON_SPATK_IV,
    DEBUG_MON_SPATK_EV,
    DEBUG_MON_SPDEF_IV,
    DEBUG_MON_SPDEF_EV,
    DEBUG_MON_COOL,
    DEBUG_MON_BEAUTY,
    DEBUG_MON_CUTE,
    DEBUG_MON_SMART,
    DEBUG_MON_TOUGH,
    DEBUG_MON_SHEEN,
    DEBUG_MON_FRIENDSHIP,
    DEBUG_MON_POKERUS,
    DEBUG_MON_IS_EGG,
    DEBUG_MON_STATUS_CONDITION,
    DEBUG_MON_MAX_HP,
    DEBUG_MON_ATK,
    DEBUG_MON_DEF,
    DEBUG_MON_SPEED,
    DEBUG_MON_SP_ATK,
    DEBUG_MON_SP_DEF,
    DEBUG_MON_FATEFUL_ENCOUNTER,
    DEBUG_MON_MET_LOCATION,
    DEBUG_MON_MET_LEVEL,
    DEBUG_MON_MET_GAME,
    DEBUG_MON_POKEBALL,
    DEBUG_MON_MET_GAME_REGION_CODE,
    DEBUG_MON_FORM,
    DEBUG_MON_MET_YEAR,
    DEBUG_MON_MET_MONTH,
    DEBUG_MON_MET_DAY,
    DEBUG_MON_HATCH_LOCATION,
    DEBUG_MON_HATCH_YEAR,
    DEBUG_MON_HATCH_MONTH,
    DEBUG_MON_HATCH_DAY,
    DEBUG_MON_HAS_NICKNAME,
    MAX_DEBUG_MON_STAT,

    DEBUG_MON_BATTLE_PARAM2 = 0xFD,
    DEBUG_MON_BATTLE_PARAM1,
    DEBUG_MON_PAGE_END
};

typedef struct DebugMon {
    Pokemon *monData;
    u32 stats[DEBUG_MON_MENU_STATS_SIZE];
    u32 statBackup;
    u8 page;
    u8 cursor;
    u8 value;
} DebugMon;

typedef struct DebugMonMenu {
    FieldSystem *sys;
    Window mainWindow;
    Window titleWindow;
    MessageLoader *msgLoader;
    StringTemplate *strTemplate;
    ColoredArrow *cursor;
    DebugMon mon;
    enum DebugMonMenuState state;
    u32 mode;
    u8 partySlot;
} DebugMonMenu;

#define DMV_COUNT_NONE 0xFF

typedef struct DebugMonValue {
    u32 min;
    u32 max;
    u8 mode;
    u8 count;
} DebugMonValue;

typedef struct DebugMonValueEntry {
    u32 label;
    const DebugMonValue *value;
} DebugMonValueEntry;

typedef struct DebugMonMenuPage {
    const u8 *page;
    u8 count;
} DebugMonMenuPage;

void DebugMonMenu_Init(DebugMonMenu *monMenu);
void DebugMonMenu_HandleInput(DebugMonMenu *monMenu);
void DebugMonMenu_HandleValueInput(DebugMonMenu *monMenu);
void DebugMonMenu_DisplayPageAndCursor(DebugMonMenu *monMenu);
void DebugMonMenu_WaitButtonPress(DebugMonMenu *monMenu, int buttonMask);
void DebugMonMenu_Free(DebugMonMenu *monMenu);

#endif // POKEPLATINUM_DEBUG_MON_MENU_H
