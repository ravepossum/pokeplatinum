#ifndef POKEPLATINUM_DEBUG_MON_MENU_H
#define POKEPLATINUM_DEBUG_MON_MENU_H

#include "field/field_system_decl.h"

#include "list_menu.h"
#include "message.h"
#include "string_template.h"
#include "sys_task_manager.h"
#include "text.h"

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

enum DebugMonValueDirection {
    DMV_DIR_NONE = 0,
    DMV_DIR_INCREASE,
    DMV_DIR_DECREASE,
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
    DEBUG_MON_STATUS,
    DEBUG_MON_MAX_HP,
    DEBUG_MON_ATK,
    DEBUG_MON_DEF,
    DEBUG_MON_SPEED,
    DEBUG_MON_SP_ATK,
    DEBUG_MON_SP_DEF,
    DEBUG_MON_FATEFUL_ENCOUNTER,
    DEBUG_MON_EGG_LOCATION,
    DEBUG_MON_MET_LEVEL,
    DEBUG_MON_MET_GAME,
    DEBUG_MON_POKEBALL,
    DEBUG_MON_FORM,
    DEBUG_MON_EGG_YEAR,
    DEBUG_MON_EGG_MONTH,
    DEBUG_MON_EGG_DAY,
    DEBUG_MON_MET_LOCATION,
    DEBUG_MON_MET_YEAR,
    DEBUG_MON_MET_MONTH,
    DEBUG_MON_MET_DAY,
    DEBUG_MON_HAS_NICKNAME,
    MAX_DEBUG_MON_STAT,

    DEBUG_MON_BATTLE_STATS_1 = 0xFD,
    DEBUG_MON_BATTLE_STATS_2,
};

typedef struct DebugMon {
    Pokemon *data;
    u32 stats[MAX_DEBUG_MON_STAT];
    u32 statBackup;
} DebugMon;

typedef struct DebugMonMenu {
    FieldSystem *fieldSystem;
    Window mainWindow;
    Window titleWindow;
    MessageLoader *msgLoader;
    StringTemplate *strTemplate;
    ColoredArrow *arrow;
    DebugMon mon;
    enum DebugMonMenuState state;
    u32 mode;
    u8 partySlot;
    u8 page;
    u8 cursor;
    u8 digits;
} DebugMonMenu;

typedef struct DebugMonStatBounds {
    u32 min;
    u32 max;
    u32 digits;
} DebugMonStatBounds;

typedef struct DebugMonStat {
    u32 label;
    const DebugMonStatBounds *bounds;
} DebugMonStat;

typedef struct DebugMonMenuPage {
    const u8 *page;
    u8 count;
} DebugMonMenuPage;

void DebugMonMenu_New(FieldSystem *fieldSystem, enum DebugMonMenuMode mode);

#endif // POKEPLATINUM_DEBUG_MON_MENU_H
