#ifndef POKEPLATINUM_DEBUG_MON_MENU_H
#define POKEPLATINUM_DEBUG_MON_MENU_H

#include "field/field_system_decl.h"
#include "struct_decls/struct_0200112C_decl.h"
#include "struct_defs/struct_0205AA50.h"
#include "struct_decls/struct_02013A04_decl.h"
#include "struct_decls/struct_020149F0_decl.h"
#include "sys_task_manager.h"
#include "string_template.h"
#include "message.h"

#define DEBUG_MON_MENU_MESSAGE_BANK	336
#define DEBUG_MON_MENU_STATS_SIZE	64
#define	DEBUG_MON_MENU_MAX_PAGES	8

#define TEXT_COLOR_MASK	            0xFF
#define TEXT_FG_COLOR_SHIFT         16
#define TEXT_SHADOW_COLOR_SHIFT     8
#define TEXT_BG_COLOR_SHIFT         0

#define DEBUG_TEXT_COLOR(fgColor, shadowColor, bgColor) (u32)(((fgColor & TEXT_COLOR_MASK) << TEXT_FG_COLOR_SHIFT) |            \
                                                                ((shadowColor & TEXT_COLOR_MASK) << TEXT_SHADOW_COLOR_SHIFT) |  \
                                                                ((bgColor & TEXT_COLOR_MASK) << TEXT_BG_COLOR_SHIFT))

#define	DMM_COLOR_BLACK             DEBUG_TEXT_COLOR(1, 2, 15)
#define	DMM_COLOR_BLUE              DEBUG_TEXT_COLOR(3, 4, 15)
#define	DMM_COLOR_RED               DEBUG_TEXT_COLOR(5, 6, 15)
#define	DMM_COLOR_PINK              DEBUG_TEXT_COLOR(7, 8, 15)
#define	DMM_COLOR_GREEN             DEBUG_TEXT_COLOR(9, 10, 15)

typedef struct DebugMon {
    Pokemon *monData;
    u32 stats[DEBUG_MON_MENU_STATS_SIZE];
    u32 dataBackup; //ravetodo better name?
    u16 str[0x100]; //ravetodo better name? unused?
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
    UnkStruct_020149F0 *cursor;
    DebugMon mon;
    u32 state;
    u32 mode;
} DebugMonMenu;

typedef struct DebugMonValue {
    u32	min;
    u32	max;
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

enum DebugMonMenuMode {
    DEBUG_MON_MENU_MODE_CREATE = 0,
    DEBUG_MON_MENU_MODE_EDIT,
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

    DEBUG_MON_BATTLE_PARAM2	= 0xFD,
    DEBUG_MON_BATTLE_PARAM1,
    DEBUG_MON_END
};

// ravetodo give these names
enum DebugMonValueLabel {
    DMV_LABEL_00 = 36,
    DMV_LABEL_01,
    DMV_LABEL_02,
    DMV_LABEL_03,
    DMV_LABEL_04,
    DMV_LABEL_05,
    DMV_LABEL_06,
    DMV_LABEL_07,
    DMV_LABEL_08,
    DMV_LABEL_09,
    DMV_LABEL_10,
    DMV_LABEL_11,
    DMV_LABEL_12,
    DMV_LABEL_13,
    DMV_LABEL_14,
    DMV_LABEL_15,
    DMV_LABEL_16,
    DMV_LABEL_17,
    DMV_LABEL_18,
    DMV_LABEL_19,
    DMV_LABEL_20,
    DMV_LABEL_21,
    DMV_LABEL_22,
    DMV_LABEL_23,
    DMV_LABEL_24,
    DMV_LABEL_25,
    DMV_LABEL_26,
    DMV_LABEL_27,
    DMV_LABEL_28,
    DMV_LABEL_29,
    DMV_LABEL_30,
    DMV_LABEL_31,
    DMV_LABEL_32,
    DMV_LABEL_33,
    DMV_LABEL_34,
    DMV_LABEL_35,
    DMV_LABEL_36,
    DMV_LABEL_37,
    DMV_LABEL_38,
    DMV_LABEL_39,
    DMV_LABEL_40,
    DMV_LABEL_41,
    DMV_LABEL_42,
    DMV_LABEL_43,
    DMV_LABEL_44,
    DMV_LABEL_45,
    DMV_LABEL_46,
    DMV_LABEL_47,
    DMV_LABEL_48,
    DMV_LABEL_49,
    DMV_LABEL_50,
    DMV_LABEL_51,
    DMV_LABEL_52,
    DMV_STATUS_LABEL_01,
    DMV_STATUS_LABEL_02,
    DMV_STATUS_LABEL_03,
    DMV_STATUS_LABEL_04,
    DMV_STATUS_LABEL_05,
    DMV_STATUS_LABEL_06,
    DMV_STATUS_LABEL_07,
    DMV_STRING_00,
    DMV_STRING_01,
    DMV_STRING_02,
    DMV_STRING_03,
    DMV_STRING_04,
    DMV_STRING_05,
    DMV_STRING_06,
    DMV_STRING_07,
    DMV_STRING_08,
    DMV_STRING_09,
    DMV_STRING_10,
    DMV_STRING_11,
    DMV_INFO_00,
    DMV_INFO_01,
    DMV_INFO_02,
    DMV_OT_NAME,
    DMV_EGG_NAME,
};

void DebugMonMenu_Init(DebugMonMenu *monMenu);
void DebugMonMenu_HandleInput(DebugMonMenu *monMenu);
void DebugMonMenu_HandleValueInput(DebugMonMenu *monMenu);
void DebugMonMenu_DisplayPageAndCursor(DebugMonMenu *monMenu);
void DebugMonMenu_WaitButtonPress(DebugMonMenu *monMenu);
void DebugMonMenu_Free(DebugMonMenu *monMenu);

#endif // POKEPLATINUM_DEBUG_MON_MENU_H
