#include "overlay005/debug_mon_menu.h"

#include "constants/charcode.h"
#include "constants/heap.h"
#include "constants/map_object.h"
#include "constants/narc.h"
#include "constants/pokemon.h"
#include "consts/items.h"
#include "consts/moves.h"
#include "consts/pokemon.h"
#include "consts/species.h"

#include "struct_defs/struct_0203D8AC.h"
#include "struct_defs/struct_02090800.h"

#include "field/field_system.h"
#include "overlay005/debug_menu.h"
#include "overlay006/ov6_02243258.h"
#include "overlay061/struct_ov61_0222C884.h"
#include "overlay084/struct_ov84_02240FA8.h"

#include "core_sys.h"
#include "field_map_change.h"
#include "field_system.h"
#include "message.h"
#include "move_table.h"
#include "party.h"
#include "strbuf.h"
#include "sys_task.h"
#include "text.h"
#include "unk_0200112C.h"
#include "unk_020021B0.h"
#include "unk_0200DA60.h"
#include "unk_0200F174.h"
#include "unk_02013A04.h"
#include "unk_02018340.h"
#include "unk_0203D1B8.h"
#include "unk_020508D4.h"
#include "unk_0206B70C.h"
#include "unk_02092494.h"

static void DebugMonMenu_PrintString(Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color);
static void DebugMonMenu_SetTrainerMemo(DebugMonMenu *monMenu, BOOL playerIsOT);
static void DebugMonMenu_DisplayCursor(DebugMonMenu *monMenu, u8 mode);
static void DebugMonMenu_ChangeValue(DebugMon *mon, u8 mode);
static void DebugMonMenu_DisplayPage(DebugMonMenu *monMenu);
static void DebugMonMenu_DisplayValues(DebugMonMenu * monMenu);
static u8 DebugMonMenu_AddMon(DebugMonMenu *monMenu);

static void DebugMon_InitStats(DebugMon *mon);
static void DebugMon_CalcInitialStats(DebugMon *mon);
static void DebugMon_CalcFullStats(DebugMonMenu *monMenu, DebugMon *mon);
static void DebugMon_CalcBaseStats(DebugMon *mon);
static void DebugMon_CalcStatsFromExp(DebugMon *mon);
static void DebugMon_SetStatsFromMonData(DebugMon * mon);
static void DebugMon_SetMonDataFromStats(DebugMon *mon);
static void DebugMon_SetMoveAtPosition(Pokemon *mon, u16 moveID, u16 movePos);

static u8 DebugMonValue_Display(DebugMonMenu *monMenu, u8 statID, u32 color, u8 y);
static void DebugMonValue_PrintStr(Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color);
static void DebugMonValue_PrintStrExpanded(Window *window, MessageLoader *msgLoader, StringTemplate *strTemplate, u32 entryID, u32 x, u32 y, u32 delay, u32 color);
static void DebugMonValue_PrintNum(Window *window, MessageLoader *msgLoader, StringTemplate *strTemplate, DebugMon *mon, u32 num, u32 digits, u32 x, u32 y, u32 delay, u32 color);
static u32 DebugMonValue_GetColor(DebugMon *mon, u8 digit, u32 color);
static void DebugMonValue_PrintSpeciesName(Window *window, u32 species, u32 x, u32 y, u32 delay, u32 color);
static u32 DebugMonValue_Get(DebugMon *mon, u8 statID);
static void DebugMonValue_Set(DebugMon *mon, u32 value, u8 statID);

static const DebugMonValue sDebugValue00 = {1, SPECIES_EGG - 1, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue01 = {1, 100, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue02 = {1, 2000000, DMMD_BOTH, 7};
static const DebugMonValue sDebugValue03 = {0, 0xFFFFFFFF, DMMD_BOTH, 10};
static const DebugMonValue sDebugValue04 = {0, 0xFFFFFFFF, DMMD_BOTH, 10};
static const DebugMonValue sDebugValue05 = {0, 2, DMMD_BOTH, 0xFF};
static const DebugMonValue sDebugValue06 = {0, 24, DMMD_BOTH, 0xFF};
static const DebugMonValue sDebugValue07 = {0, MAX_MOVES, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue08 = {0, MAX_MOVES, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue09 = {0, MAX_MOVES, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue10 = {0, MAX_MOVES, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue11 = {0, MAX_ITEMS, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue12 = {0, 1, DMMD_BOTH, 0xFF};
static const DebugMonValue sDebugValue13 = {0, 0x1F, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue14 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue15 = {0, 0x1F, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue16 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue17 = {0, 0x1F, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue18 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue19 = {0, 0x1F, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue20 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue21 = {0, 0x1F, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue22 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue23 = {0, 0x1F, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue24 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue25 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue26 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue27 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue28 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue29 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue30 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue31 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue32 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue33 = {0, 1, DMMD_BOTH, 0xFF};
static const DebugMonValue sDebugValue34 = {0, 6, DMMD_BOTH, 0xFF};
static const DebugMonValue sDebugValue41 = {0, 1, DMMD_BOTH, 0xFF};
static const DebugMonValue sDebugValue42 = {0, 9999, DMMD_BOTH, 4};
static const DebugMonValue sDebugValue43 = {0, 100, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue44 = {1, 15, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue45 = {1, 16, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue46 = {1, 8, DMMD_BOTH, 1};
static const DebugMonValue sDebugValue47 = {0, 255, DMMD_BOTH, 3};
static const DebugMonValue sDebugValue48 = {0, 99, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue49 = {1, 12, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue50 = {1, 31, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue51 = {0, 9999, DMMD_BOTH, 4};
static const DebugMonValue sDebugValue52 = {0, 99, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue53 = {1, 12, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue54 = {1, 31, DMMD_BOTH, 2};
static const DebugMonValue sDebugValue55 = {0, 1, DMMD_BOTH, 0xFF};

static const DebugMonValueEntry	sDebugMonValueList[] = {
    {DMV_LABEL_00, &sDebugValue00}, {DMV_LABEL_01, &sDebugValue01}, {DMV_LABEL_02, &sDebugValue02}, {DMV_LABEL_03, &sDebugValue03},
    {DMV_LABEL_04, &sDebugValue04}, {DMV_LABEL_05, &sDebugValue05}, {DMV_LABEL_06, &sDebugValue06}, {DMV_LABEL_07, &sDebugValue07},
    {DMV_LABEL_08, &sDebugValue08}, {DMV_LABEL_09, &sDebugValue09}, {DMV_LABEL_10, &sDebugValue10}, {DMV_LABEL_11, &sDebugValue11},
    {DMV_LABEL_12, &sDebugValue12}, {DMV_LABEL_13, &sDebugValue13}, {DMV_LABEL_14, &sDebugValue14}, {DMV_LABEL_15, &sDebugValue15},
    {DMV_LABEL_16, &sDebugValue16}, {DMV_LABEL_17, &sDebugValue17}, {DMV_LABEL_18, &sDebugValue18}, {DMV_LABEL_19, &sDebugValue19},
    {DMV_LABEL_20, &sDebugValue20}, {DMV_LABEL_21, &sDebugValue21}, {DMV_LABEL_22, &sDebugValue22}, {DMV_LABEL_23, &sDebugValue23},
    {DMV_LABEL_24, &sDebugValue24}, {DMV_LABEL_25, &sDebugValue25}, {DMV_LABEL_26, &sDebugValue26}, {DMV_LABEL_27, &sDebugValue27},
    {DMV_LABEL_28, &sDebugValue28}, {DMV_LABEL_29, &sDebugValue29}, {DMV_LABEL_30, &sDebugValue30}, {DMV_LABEL_31, &sDebugValue31},
    {DMV_LABEL_32, &sDebugValue32}, {DMV_LABEL_33, &sDebugValue33}, {DMV_LABEL_34, &sDebugValue34}, {DMV_LABEL_35, &sDebugValue00},
    {DMV_LABEL_36, &sDebugValue00}, {DMV_LABEL_37, &sDebugValue00}, {DMV_LABEL_38, &sDebugValue00}, {DMV_LABEL_39, &sDebugValue00},
    {DMV_LABEL_40, &sDebugValue00}, {DMV_LABEL_41, &sDebugValue41}, {DMV_LABEL_42, &sDebugValue42}, {DMV_LABEL_43, &sDebugValue43},
    {DMV_LABEL_44, &sDebugValue44}, {DMV_LABEL_45, &sDebugValue45}, {DMV_LABEL_46, &sDebugValue46}, {DMV_LABEL_47, &sDebugValue47},
    {DMV_LABEL_49, &sDebugValue48}, {DMV_LABEL_50, &sDebugValue49}, {DMV_LABEL_51, &sDebugValue50}, {DMV_LABEL_48, &sDebugValue51},
    {DMV_LABEL_49, &sDebugValue52}, {DMV_LABEL_50, &sDebugValue53}, {DMV_LABEL_51, &sDebugValue54}, {DMV_LABEL_52, &sDebugValue55},
};

static const u32 sDebugMonIncrements[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
};

static const u32 sStatusConditionTable[][2] = {
    {DMV_STATUS_LABEL_01, 0x00000000},
    {DMV_STATUS_LABEL_02, 0x00000001},
    {DMV_STATUS_LABEL_03, 0x00000008},
    {DMV_STATUS_LABEL_04, 0x00000010},
    {DMV_STATUS_LABEL_05, 0x00000020},
    {DMV_STATUS_LABEL_06, 0x00000040},
    {DMV_STATUS_LABEL_07, 0x00000080},
};

static const u8 sDebugMonPage1[] = {
    DEBUG_MON_SPECIES, DEBUG_MON_LEVEL, DEBUG_MON_EXP,
    DEBUG_MON_OT_ID, DEBUG_MON_PERSONALITY, DEBUG_MON_GENDER,
    DEBUG_MON_NATURE, DEBUG_MON_END
};

static const u8 sDebugMonPage2[] = {
    DEBUG_MON_STATUS_CONDITION, DEBUG_MON_FRIENDSHIP, DEBUG_MON_POKERUS,
    DEBUG_MON_IS_EGG, DEBUG_MON_FATEFUL_ENCOUNTER, DEBUG_MON_HAS_NICKNAME,
    DEBUG_MON_END
};

static const u8 sDebugMonPage3[] = {
    DEBUG_MON_MOVE1, DEBUG_MON_MOVE2, DEBUG_MON_MOVE3,
    DEBUG_MON_MOVE4, DEBUG_MON_HELD_ITEM, DEBUG_MON_ABILITY,
    DEBUG_MON_END
};

static const u8 sDebugMonPage4[] = {
    DEBUG_MON_HP_IV, DEBUG_MON_HP_EV, DEBUG_MON_ATK_IV,
    DEBUG_MON_ATK_EV, DEBUG_MON_DEF_IV, DEBUG_MON_DEF_EV,
    DEBUG_MON_BATTLE_PARAM1, DEBUG_MON_END
};

static const u8 sDebugMonPage5[] = {
    DEBUG_MON_SPEED_IV, DEBUG_MON_SPEED_EV, DEBUG_MON_SPATK_IV,
    DEBUG_MON_SPATK_EV, DEBUG_MON_SPDEF_IV, DEBUG_MON_SPDEF_EV,
    DEBUG_MON_BATTLE_PARAM2, DEBUG_MON_END
};

static const u8 sDebugMonPage6[] = {
    DEBUG_MON_COOL, DEBUG_MON_BEAUTY, DEBUG_MON_CUTE,
    DEBUG_MON_SMART, DEBUG_MON_TOUGH, DEBUG_MON_SHEEN,
    DEBUG_MON_END
};

static const u8 sDebugMonPage7[] = {
    DEBUG_MON_MET_LEVEL, DEBUG_MON_MET_GAME, DEBUG_MON_POKEBALL,
    DEBUG_MON_MET_GAME_REGION_CODE, DEBUG_MON_FORM, DEBUG_MON_END
};

static const u8 sDebugMonPage8[] = {
    DEBUG_MON_MET_LOCATION, DEBUG_MON_MET_YEAR, DEBUG_MON_MET_MONTH,
    DEBUG_MON_MET_DAY, DEBUG_MON_HATCH_LOCATION, DEBUG_MON_HATCH_YEAR,
    DEBUG_MON_HATCH_MONTH, DEBUG_MON_HATCH_DAY, DEBUG_MON_END
};

static const DebugMonMenuPage sDebugMonMenuPages[DEBUG_MON_MENU_MAX_PAGES] = {
    {sDebugMonPage1, 5},
    {sDebugMonPage2, 6},
    {sDebugMonPage3, 6},
    {sDebugMonPage4, 6},
    {sDebugMonPage5, 6}, 
    {sDebugMonPage6, 6},
    {sDebugMonPage7, 5},
    {sDebugMonPage8, 8}
};

void DebugMonMenu_HandleInput (DebugMonMenu *monMenu)
{
    u8 result;

    if (gCoreSys.pressedKeys & PAD_BUTTON_A) {
        monMenu->mon.dataBackup = monMenu->mon.stats[sDebugMonMenuPages[monMenu->mon.page].page[monMenu->mon.cursor]];
        monMenu->mon.value = 0;
        DebugMonMenu_DisplayValues(monMenu);
        monMenu->state = 2;
        return;
    }

    if (gCoreSys.pressedKeys & PAD_BUTTON_B) {
        monMenu->state = 4;
        return;
    }

    if (gCoreSys.pressedKeys & PAD_BUTTON_START) {
        if (monMenu->mode != DEBUG_MON_MENU_MODE_EDIT) {
            DebugMon_CalcFullStats(monMenu, &monMenu->mon);
        } else {
            DebugMon_SetMonDataFromStats(&monMenu->mon);
        }

        result = DebugMonMenu_AddMon(monMenu);

        BGL_WindowColor(&monMenu->mainWindow, 15, 24, 64, 30*8-24, 32);

        if (result == 1) {
            DebugMonMenu_PrintString(&monMenu->mainWindow, monMenu->msgLoader, 109, 32, 72, 0, DMM_COLOR_BLACK);
        } else {
            DebugMonMenu_PrintString(&monMenu->mainWindow, monMenu->msgLoader, 108, 32, 72, 0, DMM_COLOR_BLACK);
        }

        monMenu->state = 3;
        return;
    }

    if (gCoreSys.pressedKeys & PAD_KEY_UP) {
        DebugMonMenu_DisplayCursor(monMenu, DMMD_DECREASE);
        return;
    }

    if (gCoreSys.pressedKeys & PAD_KEY_DOWN) {
        DebugMonMenu_DisplayCursor(monMenu, DMMD_INCREASE);
        return;
    }

    if ((gCoreSys.pressedKeys & PAD_BUTTON_L) && monMenu->mon.page > 0) {
        monMenu->mon.page--;
        monMenu->mon.cursor = 0;
        monMenu->state = 0;
        return;
    }

    if ((gCoreSys.pressedKeys & PAD_BUTTON_R) && monMenu->mon.page < DEBUG_MON_MENU_MAX_PAGES - 1) {
        monMenu->mon.page++;
        monMenu->mon.cursor = 0;
        monMenu->state = 0;
        return;
    }

    if (gCoreSys.pressedKeys & PAD_BUTTON_X) {
        DebugMonMenu_SetTrainerMemo(monMenu, TRUE);
        monMenu->state = 5;
        return;
    }

    if (gCoreSys.pressedKeys & PAD_BUTTON_Y) {
        DebugMonMenu_SetTrainerMemo(monMenu, FALSE);
        monMenu->state = 5;
    }
}

void DebugMonMenu_HandleValueInput (DebugMonMenu *monMenu)
{
    u8 statID = sDebugMonMenuPages[monMenu->mon.page].page[monMenu->mon.cursor];

    if (gCoreSys.pressedKeys & PAD_BUTTON_A) {
        monMenu->state = 0;
        return;
    }

    if (gCoreSys.pressedKeys & PAD_BUTTON_B) {
        monMenu->mon.stats[statID] = monMenu->mon.dataBackup;
        monMenu->state = 0;
        return;
    }

    if (gCoreSys.heldKeys & PAD_KEY_UP) {
        DebugMonMenu_ChangeValue(&monMenu->mon, DMMD_INCREASE);
        DebugMonMenu_DisplayValues(monMenu);
        return;
    }

    if (gCoreSys.heldKeys & PAD_KEY_DOWN) {
        DebugMonMenu_ChangeValue(&monMenu->mon, DMMD_DECREASE);
        DebugMonMenu_DisplayValues(monMenu);
        return;
    }

    if (sDebugMonValueList[statID].value->count != 0xFF) {
        if ((gCoreSys.pressedKeys & PAD_KEY_LEFT) && monMenu->mon.value < sDebugMonValueList[statID].value->count - 1) {
            monMenu->mon.value++;
            DebugMonMenu_DisplayValues(monMenu);
            return;
        }
        if ((gCoreSys.pressedKeys & PAD_KEY_RIGHT) && monMenu->mon.value > 0) {
            monMenu->mon.value--;
            DebugMonMenu_DisplayValues(monMenu);
        }
    }
}

void DebugMonMenu_WaitButtonPress (DebugMonMenu *monMenu)
{
    if (gCoreSys.pressedKeys & PAD_BUTTON_A) {
        monMenu->state = 0;
    }
}

void DebugMonMenu_Init (DebugMonMenu *monMenu)
{
    BGL_FillWindow(&monMenu->titleWindow, 15);
    DebugMonMenu_PrintString(&monMenu->titleWindow, monMenu->msgLoader, DMV_INFO_02, 0, 0, 0, DMM_COLOR_BLACK);

    monMenu->mon.monData = Pokemon_New(HEAP_ID_APPLICATION);

    if (monMenu->mode == DEBUG_MON_MENU_MODE_CREATE) {
        DebugMon_InitStats(&monMenu->mon);
    } else if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
        Party* party = Party_GetFromSavedata(monMenu->sys->saveData);
        Pokemon* changeMon = Party_GetPokemonBySlotIndex(party, 0);

        MI_CpuCopy8(changeMon, monMenu->mon.monData, Pokemon_GetStructSize());

        DebugMon_SetStatsFromMonData(&monMenu->mon);
    }

    monMenu->mon.page = 0;
    monMenu->mon.cursor = 0;
}

void DebugMonMenu_Free (DebugMonMenu *monMenu)
{
    Heap_FreeToHeap(monMenu->mon.monData);

    Window_Clear(&monMenu->titleWindow, 1);
    Window_Clear(&monMenu->mainWindow, 0);

    // window off
    sub_0201ACF4(&monMenu->titleWindow);
    sub_0201ACF4(&monMenu->mainWindow);

    BGL_DeleteWindow(&monMenu->titleWindow);
    BGL_DeleteWindow(&monMenu->mainWindow);

    MessageLoader_Free(monMenu->msgLoader);
    StringTemplate_Free(monMenu->strTemplate);
    sub_02014A20(monMenu->cursor);
}

static void DebugMonMenu_PrintString (Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, entryID);

    // print color string to window?
    Text_AddPrinterWithParamsAndColor(window, 0, buf, x, y, delay, color, NULL);

    Strbuf_Free(buf);
}

#define DISPLAYINFO_PRINT(info)                                                                                                           \
    {                                                                                                                                     \
        if (info.unk_04 != NULL) {                                                                                                        \
            Text_AddPrinterWithParamsAndColor(&monMenu->mainWindow, 0, info.unk_04, 8, (info.unk_00 - 1) * 16, 0, DMM_COLOR_BLACK, NULL); \
        }                                                                                                                                 \
    }

static void DebugMonMenu_SetTrainerMemo (DebugMonMenu *monMenu, BOOL playerIsOT)
{
    if (monMenu->mode != DEBUG_MON_MENU_MODE_EDIT) {
        DebugMon_CalcFullStats(monMenu, &monMenu->mon);
    } else {
        DebugMon_SetMonDataFromStats(&monMenu->mon);
    }

    BGL_FillWindow(&monMenu->mainWindow, 15);

    // get display info?
    PokemonInfoDisplayStruct* infoDisplay = sub_02092494(monMenu->mon.monData, playerIsOT, HEAP_ID_APPLICATION);
    
    DISPLAYINFO_PRINT(infoDisplay->unk_14)
    DISPLAYINFO_PRINT(infoDisplay->unk_1C)
    DISPLAYINFO_PRINT(infoDisplay->unk_24)
    DISPLAYINFO_PRINT(infoDisplay->unk_2C)
    DISPLAYINFO_PRINT(infoDisplay->unk_34)

    // free display info?
    sub_0209282C(infoDisplay);
}

static void DebugMonMenu_DisplayCursor (DebugMonMenu *monMenu, u8 mode)
{
    DebugMon *mon = &monMenu->mon;

    switch(mode) {
    case DMMD_BOTH:
        // print cursor
        sub_02014A58(monMenu->cursor, &monMenu->mainWindow, 0, mon->cursor * 16);
        // update window at vblank?
        sub_0201A9A4(&monMenu->mainWindow);
        break;

    case DMMD_INCREASE:
        if (mon->cursor < sDebugMonMenuPages[mon->page].count - 1) {
            BGL_WindowColor(&monMenu->mainWindow, 15, 0, mon->cursor * 16, 12, 16);
            mon->cursor++;
            //print cursor
            sub_02014A58(monMenu->cursor, &monMenu->mainWindow, 0, mon->cursor * 16);
            // update window at vblank?
            sub_0201A9A4(&monMenu->mainWindow);
        }
        break;

    case DMMD_DECREASE:
        if (mon->cursor > 0) {
            BGL_WindowColor(&monMenu->mainWindow, 15, 0, mon->cursor * 16, 12, 16);
            mon->cursor--;
            // print cursor
            sub_02014A58(monMenu->cursor, &monMenu->mainWindow, 0, mon->cursor * 16);
            // update window at vblank?
            sub_0201A9A4(&monMenu->mainWindow);
        }
        break;

    }
}

static void DebugMonMenu_ChangeValue (DebugMon *mon, u8 mode)
{
    u32 currentValue = DebugMonValue_Get(mon, sDebugMonMenuPages[mon->page].page[mon->cursor]);
    u32 minValue = sDebugMonValueList[sDebugMonMenuPages[mon->page].page[mon->cursor]].value->min;
    u32 maxValue = sDebugMonValueList[sDebugMonMenuPages[mon->page].page[mon->cursor]].value->max;
    u32 valueChange = sDebugMonIncrements[mon->value];

    switch(sDebugMonValueList[sDebugMonMenuPages[mon->page].page[mon->cursor]].value->mode) {
    case DMMD_BOTH:
        switch(mode) {
        case DMMD_INCREASE:
            if (currentValue < maxValue-valueChange) {
                currentValue += valueChange;
            } else {
                if ( currentValue < maxValue ) {
                    currentValue = maxValue;
                } else {
                    currentValue = minValue;
                }
            }
            break;

        case DMMD_DECREASE:
            if (currentValue > minValue + valueChange) {
                currentValue -= valueChange;
            } else {
                if (currentValue > minValue){
                    currentValue = minValue;
                } else {
                    currentValue = maxValue;
                }
            }
            break;
        }
        break;
    }
    DebugMonValue_Set(mon, currentValue, sDebugMonMenuPages[mon->page].page[mon->cursor]);
}

void DebugMonMenu_DisplayPageAndCursor (DebugMonMenu *monMenu)
{
    DebugMonMenu_DisplayPage(monMenu);
    DebugMonMenu_DisplayCursor(monMenu, DMMD_BOTH);
    monMenu->state = 1;
}

static void DebugMonMenu_DisplayPage (DebugMonMenu *monMenu)
{
    DebugMon *mon = &monMenu->mon;

    BGL_FillWindow(&monMenu->mainWindow, 15);

    u16 i = 0;
    while(sDebugMonMenuPages[mon->page].page[i] != DEBUG_MON_END) {
        DebugMonValue_Display(monMenu, sDebugMonMenuPages[mon->page].page[i], DMM_COLOR_BLACK, i*16);
        i++;
    }

    // BGL window on?
    sub_0201ACCC(&monMenu->mainWindow);
}

static void DebugMonMenu_DisplayValues (DebugMonMenu * monMenu)
{
    DebugMon *mon = &monMenu->mon;
    BGL_FillWindow(&monMenu->mainWindow, 15);

    u16 i = 0;
    while (sDebugMonMenuPages[mon->page].page[i] != DEBUG_MON_END) {

        if (i == mon->cursor) {
            DebugMonValue_Display(monMenu, sDebugMonMenuPages[mon->page].page[i], DMM_COLOR_RED, i * 16);
        } else {
            DebugMonValue_Display(monMenu, sDebugMonMenuPages[mon->page].page[i], DMM_COLOR_BLACK, i * 16);
        }

        i++;
    }

    // BGL window on?
    sub_0201ACCC(&monMenu->mainWindow);
}

static u8 DebugMonMenu_AddMon (DebugMonMenu *monMenu)
{
    Party *party;

    if (monMenu->mode == DEBUG_MON_MENU_MODE_CREATE) {
        party = Party_GetFromSavedata(monMenu->sys->saveData);

        if (Party_AddPokemon(party, monMenu->mon.monData) == TRUE) {
            return 0;
        }

        // add boxmon
        sub_02079868(SaveData_PCBoxes(monMenu->sys->saveData), Pokemon_GetBoxPokemon(monMenu->mon.monData));

        return 1;

    } else if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
        party = Party_GetFromSavedata(monMenu->sys->saveData);

        Strbuf *buf;

        if (monMenu->mon.stats[DEBUG_MON_IS_EGG]) {
            buf = MessageLoader_GetNewStrbuf(monMenu->msgLoader, 112);
            Pokemon_SetValue(monMenu->mon.monData, MON_DATA_NICKNAME_STRBUF, buf);
            Strbuf_Free(buf);
        }
        
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, 0);

        buf = Strbuf_Init(16, HEAP_ID_APPLICATION);
        Pokemon_GetValue(mon, MON_DATA_OTNAME_STRBUF, buf);
        Pokemon_SetValue(monMenu->mon.monData, MON_DATA_OTNAME_STRBUF, buf);
        Strbuf_Free(buf);

        u8 gender = Pokemon_GetValue(mon, MON_DATA_OT_GENDER, NULL);
        Pokemon_SetValue(monMenu->mon.monData, MON_DATA_OT_GENDER, &gender);
        // set party data?
        sub_0207A128(party, 0, monMenu->mon.monData);
        return 0;
    }

    return 0;
}

static void DebugMon_InitStats (DebugMon *mon)
{
    for (u32 i = 0; i < DEBUG_MON_MENU_STATS_SIZE; i++) {
        mon->stats[i] = 0;
    }

    mon->stats[DEBUG_MON_SPECIES] = 1;
    mon->stats[DEBUG_MON_PERSONALITY] = (LCRNG_Next() << 16) | LCRNG_Next();
    mon->stats[DEBUG_MON_LEVEL] = 1;
    mon->stats[DEBUG_MON_EXP] = 1;
    mon->stats[DEBUG_MON_MET_LEVEL] = 1;
    mon->stats[DEBUG_MON_MET_GAME] = 1;
    mon->stats[DEBUG_MON_POKEBALL] = 1;
    mon->stats[DEBUG_MON_MET_GAME_REGION_CODE] = 1;
    mon->stats[DEBUG_MON_FORM] = 0;
    mon->stats[DEBUG_MON_MET_YEAR] = 6;
    mon->stats[DEBUG_MON_MET_MONTH] = 9;
    mon->stats[DEBUG_MON_MET_DAY] = 28;
    mon->stats[DEBUG_MON_HATCH_YEAR] = 6;
    mon->stats[DEBUG_MON_HATCH_MONTH] = 9;
    mon->stats[DEBUG_MON_HATCH_DAY] = 28;

    DebugMon_CalcInitialStats(mon);
}

static void DebugMon_CalcInitialStats (DebugMon *mon)
{
    u32	monIVs = ((mon->stats[DEBUG_MON_HP_IV] & 0x0000001f) << 0)
                    | ((mon->stats[DEBUG_MON_ATK_IV] & 0x0000001f) << 5)
                    | ((mon->stats[DEBUG_MON_DEF_IV] & 0x0000001f) << 10)
                    | ((mon->stats[DEBUG_MON_SPEED_IV] & 0x0000001f) << 15)
                    | ((mon->stats[DEBUG_MON_SPATK_IV] & 0x0000001f) << 20)
                    | ((mon->stats[DEBUG_MON_SPDEF_IV] & 0x0000001f) << 25);

    Pokemon_InitWith(mon->monData, mon->stats[DEBUG_MON_SPECIES], mon->stats[DEBUG_MON_LEVEL], monIVs, 1, mon->stats[DEBUG_MON_PERSONALITY], 1, mon->stats[DEBUG_MON_OT_ID]);

    mon->stats[DEBUG_MON_GENDER] = Pokemon_GetGender(mon->monData);
    mon->stats[DEBUG_MON_NATURE] = Pokemon_GetNature(mon->monData);
    mon->stats[DEBUG_MON_MOVE1] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE1, NULL);
    mon->stats[DEBUG_MON_MOVE2] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE2, NULL);
    mon->stats[DEBUG_MON_MOVE3] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE3, NULL);
    mon->stats[DEBUG_MON_MOVE4] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE4, NULL);
    mon->stats[DEBUG_MON_EXP] = Pokemon_GetValue(mon->monData, MON_DATA_EXP, NULL);
    mon->stats[DEBUG_MON_MAX_HP] = Pokemon_GetValue(mon->monData, MON_DATA_MAX_HP, NULL);
    mon->stats[DEBUG_MON_ATK] = Pokemon_GetValue(mon->monData, MON_DATA_ATK, NULL);
    mon->stats[DEBUG_MON_DEF] = Pokemon_GetValue(mon->monData, MON_DATA_DEF, NULL);
    mon->stats[DEBUG_MON_SPEED] = Pokemon_GetValue(mon->monData, MON_DATA_SPEED, NULL);
    mon->stats[DEBUG_MON_SP_ATK] = Pokemon_GetValue(mon->monData, MON_DATA_SP_ATK, NULL);
    mon->stats[DEBUG_MON_SP_DEF] = Pokemon_GetValue(mon->monData, MON_DATA_SP_DEF, NULL);
}

static void DebugMon_CalcFullStats (DebugMonMenu *monMenu, DebugMon *mon)
{
    u32 monIVs = ((mon->stats[DEBUG_MON_HP_IV] & 0x0000001f) << 0)
                    | ((mon->stats[DEBUG_MON_ATK_IV] & 0x0000001f) << 5)
                    | ((mon->stats[DEBUG_MON_DEF_IV] & 0x0000001f) << 10)
                    | ((mon->stats[DEBUG_MON_SPEED_IV] & 0x0000001f) << 15)
                    | ((mon->stats[DEBUG_MON_SPATK_IV] & 0x0000001f) << 20)
                    | ((mon->stats[DEBUG_MON_SPDEF_IV] & 0x0000001f) << 25);

    Pokemon_InitWith(mon->monData, mon->stats[DEBUG_MON_SPECIES], mon->stats[DEBUG_MON_LEVEL], monIVs, 1, mon->stats[DEBUG_MON_PERSONALITY], 1, mon->stats[DEBUG_MON_OT_ID]);

    if (mon->stats[DEBUG_MON_MOVE1]) {
        DebugMon_SetMoveAtPosition(mon->monData, mon->stats[DEBUG_MON_MOVE1], 0);
    }

    if (mon->stats[DEBUG_MON_MOVE2]) {
        DebugMon_SetMoveAtPosition(mon->monData, mon->stats[DEBUG_MON_MOVE2], 1);
    }

    if (mon->stats[DEBUG_MON_MOVE3]) {
        DebugMon_SetMoveAtPosition(mon->monData, mon->stats[DEBUG_MON_MOVE3], 2);
    }

    if (mon->stats[DEBUG_MON_MOVE4]) {
        DebugMon_SetMoveAtPosition(mon->monData, mon->stats[DEBUG_MON_MOVE4], 3);
    }

    Pokemon_SetValue(mon->monData, MON_DATA_EXP, &mon->stats[DEBUG_MON_EXP]);
    Pokemon_SetValue(mon->monData, MON_DATA_HP_IV, &mon->stats[DEBUG_MON_HP_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_HP_EV, &mon->stats[DEBUG_MON_HP_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_ATK_IV, &mon->stats[DEBUG_MON_ATK_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_ATK_EV, &mon->stats[DEBUG_MON_ATK_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_DEF_IV, &mon->stats[DEBUG_MON_DEF_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_DEF_EV, &mon->stats[DEBUG_MON_DEF_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPEED_IV, &mon->stats[DEBUG_MON_SPEED_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPEED_EV, &mon->stats[DEBUG_MON_SPEED_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPATK_IV, &mon->stats[DEBUG_MON_SPATK_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPATK_EV, &mon->stats[DEBUG_MON_SPATK_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPDEF_IV, &mon->stats[DEBUG_MON_SPDEF_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPDEF_EV, &mon->stats[DEBUG_MON_SPDEF_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_COOL, &mon->stats[DEBUG_MON_COOL]);
    Pokemon_SetValue(mon->monData, MON_DATA_BEAUTY, &mon->stats[DEBUG_MON_BEAUTY]);
    Pokemon_SetValue(mon->monData, MON_DATA_CUTE, &mon->stats[DEBUG_MON_CUTE]);
    Pokemon_SetValue(mon->monData, MON_DATA_SMART, &mon->stats[DEBUG_MON_SMART]);
    Pokemon_SetValue(mon->monData, MON_DATA_TOUGH, &mon->stats[DEBUG_MON_TOUGH]);
    Pokemon_SetValue(mon->monData, MON_DATA_SHEEN, &mon->stats[DEBUG_MON_SHEEN]);
    Pokemon_SetValue(mon->monData, MON_DATA_FRIENDSHIP, &mon->stats[DEBUG_MON_FRIENDSHIP]);
    Pokemon_SetValue(mon->monData, MON_DATA_POKERUS, &mon->stats[DEBUG_MON_POKERUS]);
    Pokemon_SetValue(mon->monData, MON_DATA_HELD_ITEM, &mon->stats[DEBUG_MON_HELD_ITEM]);

    u32 ability = PokemonPersonalData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], MON_DATA_PERSONAL_ABILITY_1 + mon->stats[DEBUG_MON_ABILITY]);
    Pokemon_SetValue(mon->monData, MON_DATA_ABILITY, &ability);

    Pokemon_SetValue(mon->monData, MON_DATA_FATEFUL_ENCOUNTER, &mon->stats[DEBUG_MON_FATEFUL_ENCOUNTER]);
    Pokemon_SetValue(mon->monData, MON_DATA_HAS_NICKNAME, &mon->stats[DEBUG_MON_HAS_NICKNAME]);
    Pokemon_SetValue(mon->monData, MON_DATA_MET_LEVEL, &mon->stats[DEBUG_MON_MET_LEVEL]);
    Pokemon_SetValue(mon->monData, MON_DATA_MET_GAME, &mon->stats[DEBUG_MON_MET_GAME]);
    Pokemon_SetValue(mon->monData, MON_DATA_POKEBALL, &mon->stats[DEBUG_MON_POKEBALL]);
    Pokemon_SetValue(mon->monData, MON_DATA_MET_GAME_REGION_CODE, &mon->stats[DEBUG_MON_MET_GAME_REGION_CODE]);
    Pokemon_SetValue(mon->monData, MON_DATA_FORM, &mon->stats[DEBUG_MON_FORM]);
    Pokemon_SetValue(mon->monData, MON_DATA_STATUS_CONDITION, &sStatusConditionTable[mon->stats[DEBUG_MON_STATUS_CONDITION]][1]);

    if (mon->stats[DEBUG_MON_IS_EGG]) {
        mon->stats[DEBUG_MON_MET_LOCATION] = 0;
        mon->stats[DEBUG_MON_MET_YEAR] = 0;
        mon->stats[DEBUG_MON_MET_MONTH] = 0;
        mon->stats[DEBUG_MON_MET_DAY] = 0;
        
        if (mon->stats[DEBUG_MON_HATCH_LOCATION] == 0) {
            mon->stats[DEBUG_MON_HATCH_LOCATION] = 1;
        }
    }

    Pokemon_SetValue(mon->monData, MON_DATA_MET_LOCATION, &mon->stats[DEBUG_MON_MET_LOCATION]);
    Pokemon_SetValue(mon->monData, MON_DATA_MET_YEAR, &mon->stats[DEBUG_MON_MET_YEAR]);
    Pokemon_SetValue(mon->monData, MON_DATA_MET_MONTH, &mon->stats[DEBUG_MON_MET_MONTH]);
    Pokemon_SetValue(mon->monData, MON_DATA_MET_DAY, &mon->stats[DEBUG_MON_MET_DAY]);
    Pokemon_SetValue(mon->monData, MON_DATA_HATCH_LOCATION, &mon->stats[DEBUG_MON_HATCH_LOCATION]);
    Pokemon_SetValue(mon->monData, MON_DATA_HATCH_YEAR, &mon->stats[DEBUG_MON_HATCH_YEAR]);
    Pokemon_SetValue(mon->monData, MON_DATA_HATCH_MONTH, &mon->stats[DEBUG_MON_HATCH_MONTH]);
    Pokemon_SetValue(mon->monData, MON_DATA_HATCH_DAY, &mon->stats[DEBUG_MON_HATCH_DAY]);
    Pokemon_SetValue(mon->monData, MON_DATA_IS_EGG, &mon->stats[DEBUG_MON_IS_EGG]);

    Strbuf * buf;

    if (mon->stats[DEBUG_MON_IS_EGG]) {
        buf = MessageLoader_GetNewStrbuf(monMenu->msgLoader, DMV_EGG_NAME);
        Pokemon_SetValue(mon->monData, MON_DATA_NICKNAME_STRBUF, buf);
        Strbuf_Free(buf);
    }

    buf = MessageLoader_GetNewStrbuf(monMenu->msgLoader, DMV_OT_NAME);
    Pokemon_SetValue(mon->monData, MON_DATA_OTNAME_STRBUF, buf);
    Strbuf_Free(buf);

    Pokemon_CalcLevelAndStats(mon->monData);
}

static void DebugMon_CalcBaseStats (DebugMon *mon)
{
    Pokemon_SetValue(mon->monData, MON_DATA_HP_IV, &mon->stats[DEBUG_MON_HP_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_HP_EV, &mon->stats[DEBUG_MON_HP_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_ATK_IV, &mon->stats[DEBUG_MON_ATK_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_ATK_EV, &mon->stats[DEBUG_MON_ATK_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_DEF_IV, &mon->stats[DEBUG_MON_DEF_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_DEF_EV, &mon->stats[DEBUG_MON_DEF_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPEED_IV, &mon->stats[DEBUG_MON_SPEED_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPEED_EV, &mon->stats[DEBUG_MON_SPEED_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPATK_IV, &mon->stats[DEBUG_MON_SPATK_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPATK_EV, &mon->stats[DEBUG_MON_SPATK_EV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPDEF_IV, &mon->stats[DEBUG_MON_SPDEF_IV]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPDEF_EV, &mon->stats[DEBUG_MON_SPDEF_EV]);

    Pokemon_CalcLevelAndStats(mon->monData);

    mon->stats[DEBUG_MON_MAX_HP] = Pokemon_GetValue(mon->monData, MON_DATA_MAX_HP, NULL);
    mon->stats[DEBUG_MON_ATK] = Pokemon_GetValue(mon->monData, MON_DATA_ATK, NULL);
    mon->stats[DEBUG_MON_DEF] = Pokemon_GetValue(mon->monData, MON_DATA_DEF, NULL);
    mon->stats[DEBUG_MON_SPEED] = Pokemon_GetValue(mon->monData, MON_DATA_SPEED, NULL);
    mon->stats[DEBUG_MON_SP_ATK] = Pokemon_GetValue(mon->monData, MON_DATA_SP_ATK, NULL);
    mon->stats[DEBUG_MON_SP_DEF] = Pokemon_GetValue(mon->monData, MON_DATA_SP_DEF, NULL);
}

static void DebugMon_CalcStatsFromExp (DebugMon *mon)
{
    u32	nullData = 0;
    Pokemon_SetValue(mon->monData, MON_DATA_EXP, &mon->stats[DEBUG_MON_EXP]);
    Pokemon_SetValue(mon->monData, MON_DATA_MOVE1, &nullData);
    Pokemon_SetValue(mon->monData, MON_DATA_MOVE2, &nullData);
    Pokemon_SetValue(mon->monData, MON_DATA_MOVE3, &nullData);
    Pokemon_SetValue(mon->monData, MON_DATA_MOVE4, &nullData);

    Pokemon_CalcLevelAndStats(mon->monData);
    Pokemon_SetDefaultMoves(mon->monData);

    mon->stats[DEBUG_MON_LEVEL] = Pokemon_GetValue(mon->monData, MON_DATA_LEVEL, NULL);
    mon->stats[DEBUG_MON_MOVE1] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE1, NULL);
    mon->stats[DEBUG_MON_MOVE2] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE2, NULL);
    mon->stats[DEBUG_MON_MOVE3] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE3, NULL);
    mon->stats[DEBUG_MON_MOVE4] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE4, NULL);

    mon->stats[DEBUG_MON_MAX_HP] = Pokemon_GetValue(mon->monData, MON_DATA_MAX_HP, NULL);
    mon->stats[DEBUG_MON_ATK] = Pokemon_GetValue(mon->monData, MON_DATA_ATK, NULL);
    mon->stats[DEBUG_MON_DEF] = Pokemon_GetValue(mon->monData, MON_DATA_DEF, NULL);
    mon->stats[DEBUG_MON_SPEED] = Pokemon_GetValue(mon->monData, MON_DATA_SPEED, NULL);
    mon->stats[DEBUG_MON_SP_ATK] = Pokemon_GetValue(mon->monData, MON_DATA_SP_ATK, NULL);
    mon->stats[DEBUG_MON_SP_DEF] = Pokemon_GetValue(mon->monData, MON_DATA_SP_DEF, NULL);
}

#define DEBUG_MON_SET_STAT_FROM_MON_DATA(statID, monDataValue) {mon->stats[statID] = Pokemon_GetValue(mon->monData, monDataValue, NULL);}

static void DebugMon_SetStatsFromMonData (DebugMon * mon)
{
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SPECIES, MON_DATA_SPECIES)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_LEVEL, MON_DATA_LEVEL)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_EXP, MON_DATA_EXP)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_OT_ID, MON_DATA_OT_ID)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_PERSONALITY, MON_DATA_PERSONALITY)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_GENDER, MON_DATA_GENDER)
    mon->stats[DEBUG_MON_NATURE] = Pokemon_GetNature(mon->monData);
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MOVE1, MON_DATA_MOVE1)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MOVE2, MON_DATA_MOVE2)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MOVE3, MON_DATA_MOVE3)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MOVE4, MON_DATA_MOVE4)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_HELD_ITEM, MON_DATA_HELD_ITEM)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_ABILITY, MON_DATA_ABILITY)

    if (mon->stats[DEBUG_MON_ABILITY] == PokemonPersonalData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], MON_DATA_ABILITY)) {
        mon->stats[DEBUG_MON_ABILITY] = 0;
    } else {
        mon->stats[DEBUG_MON_ABILITY] = 1;
    }

    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_HP_IV, MON_DATA_HP_IV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_HP_EV, MON_DATA_HP_EV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_ATK_IV, MON_DATA_ATK_IV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_ATK_EV, MON_DATA_ATK_EV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_DEF_IV, MON_DATA_DEF_IV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_DEF_EV, MON_DATA_DEF_EV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SPEED_IV, MON_DATA_SPEED_IV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SPEED_EV, MON_DATA_SPEED_EV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SPATK_IV, MON_DATA_SPATK_IV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SPATK_EV, MON_DATA_SPATK_EV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SPDEF_IV, MON_DATA_SPDEF_IV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SPDEF_EV, MON_DATA_SPDEF_EV)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_COOL, MON_DATA_COOL)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_BEAUTY, MON_DATA_BEAUTY)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_CUTE, MON_DATA_CUTE)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SMART, MON_DATA_SMART)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_TOUGH, MON_DATA_TOUGH)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SHEEN, MON_DATA_SHEEN)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_FRIENDSHIP, MON_DATA_FRIENDSHIP)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_POKERUS, MON_DATA_POKERUS)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_IS_EGG, MON_DATA_IS_EGG)

    u32 statusCondition = Pokemon_GetValue(mon->monData, MON_DATA_STATUS_CONDITION, NULL);
    mon->stats[DEBUG_MON_STATUS_CONDITION] = 0;
    for (u32 i = 0; i < 7; i++) {
        if(statusCondition == sStatusConditionTable[i][1]) {
            mon->stats[DEBUG_MON_STATUS_CONDITION] = i;
            break;
        }
    }

    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MAX_HP, MON_DATA_MAX_HP)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_ATK, MON_DATA_ATK)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_DEF, MON_DATA_DEF)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SPEED, MON_DATA_SPEED)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SP_ATK, MON_DATA_SP_ATK)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_SP_DEF, MON_DATA_SP_DEF)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_FATEFUL_ENCOUNTER, MON_DATA_FATEFUL_ENCOUNTER)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_HAS_NICKNAME, MON_DATA_HAS_NICKNAME)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MET_LEVEL, MON_DATA_MET_LEVEL)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MET_GAME, MON_DATA_MET_GAME)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_POKEBALL, MON_DATA_POKEBALL)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MET_GAME_REGION_CODE, MON_DATA_MET_GAME_REGION_CODE)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_FORM, MON_DATA_FORM)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MET_LOCATION, MON_DATA_MET_LOCATION)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MET_YEAR, MON_DATA_MET_YEAR)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MET_MONTH, MON_DATA_MET_MONTH)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_MET_DAY, MON_DATA_MET_DAY)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_HATCH_LOCATION, MON_DATA_HATCH_LOCATION)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_HATCH_YEAR, MON_DATA_HATCH_YEAR)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_HATCH_MONTH, MON_DATA_HATCH_MONTH)
    DEBUG_MON_SET_STAT_FROM_MON_DATA(DEBUG_MON_HATCH_DAY, MON_DATA_HATCH_DAY)
}

#define DEBUG_MON_CLEAR_STAT(statID) {mon->stats[statID] = 0;}
#define DEBUG_MON_SET_MON_DATA_FROM_STAT(statID, monDataValue) {Pokemon_SetValue(mon->monData, monDataValue, &mon->stats[statID]);}

static void DebugMon_SetMonDataFromStats (DebugMon *mon)
{
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SPECIES, MON_DATA_SPECIES)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_LEVEL, MON_DATA_LEVEL)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_EXP, MON_DATA_EXP)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_OT_ID, MON_DATA_OT_ID)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_PERSONALITY, MON_DATA_PERSONALITY)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_GENDER, MON_DATA_GENDER)

    if (mon->stats[DEBUG_MON_MOVE1]) {
        DebugMon_SetMoveAtPosition(mon->monData, mon->stats[DEBUG_MON_MOVE1], 0);
    }
    if (mon->stats[DEBUG_MON_MOVE2]) {
        DebugMon_SetMoveAtPosition(mon->monData, mon->stats[DEBUG_MON_MOVE2], 1);
    }
    if (mon->stats[DEBUG_MON_MOVE3]) {
        DebugMon_SetMoveAtPosition(mon->monData, mon->stats[DEBUG_MON_MOVE3], 2);
    }
    if (mon->stats[DEBUG_MON_MOVE4]) {
        DebugMon_SetMoveAtPosition(mon->monData, mon->stats[DEBUG_MON_MOVE4], 3);
    }

    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_HELD_ITEM, MON_DATA_HELD_ITEM)

    int ability = PokemonPersonalData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], MON_DATA_ABILITY + mon->stats[DEBUG_MON_ABILITY]);
    Pokemon_SetValue(mon->monData, MON_DATA_ABILITY, &ability);

    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_HP_IV, MON_DATA_HP_IV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_HP_EV, MON_DATA_HP_EV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_ATK_IV, MON_DATA_ATK_IV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_ATK_EV, MON_DATA_ATK_EV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_DEF_IV, MON_DATA_DEF_IV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_DEF_EV, MON_DATA_DEF_EV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SPEED_IV, MON_DATA_SPEED_IV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SPEED_EV, MON_DATA_SPEED_EV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SPATK_IV, MON_DATA_SPATK_IV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SPATK_EV, MON_DATA_SPATK_EV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SPDEF_IV, MON_DATA_SPDEF_IV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SPDEF_EV, MON_DATA_SPDEF_EV)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_COOL, MON_DATA_COOL)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_BEAUTY, MON_DATA_BEAUTY)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_CUTE, MON_DATA_CUTE)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SMART, MON_DATA_SMART)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_TOUGH, MON_DATA_TOUGH)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SHEEN, MON_DATA_SHEEN)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_FRIENDSHIP, MON_DATA_FRIENDSHIP)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_POKERUS, MON_DATA_POKERUS)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_IS_EGG, MON_DATA_IS_EGG)

    Pokemon_SetValue(mon->monData, MON_DATA_STATUS_CONDITION, &sStatusConditionTable[mon->stats[DEBUG_MON_STATUS_CONDITION]][1]);

    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_MAX_HP, MON_DATA_MAX_HP)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_ATK, MON_DATA_ATK)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_DEF, MON_DATA_DEF)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SPEED, MON_DATA_SPEED)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SP_ATK, MON_DATA_SP_ATK)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_SP_DEF, MON_DATA_SP_DEF)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_FATEFUL_ENCOUNTER, MON_DATA_FATEFUL_ENCOUNTER)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_HAS_NICKNAME, MON_DATA_HAS_NICKNAME)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_MET_LEVEL, MON_DATA_MET_LEVEL)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_MET_GAME, MON_DATA_MET_GAME)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_POKEBALL, MON_DATA_POKEBALL)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_MET_GAME_REGION_CODE, MON_DATA_MET_GAME_REGION_CODE)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_FORM, MON_DATA_FORM)

    if (mon->stats[DEBUG_MON_IS_EGG]) {
        DEBUG_MON_CLEAR_STAT(DEBUG_MON_MET_LOCATION)
        DEBUG_MON_CLEAR_STAT(DEBUG_MON_MET_YEAR)
        DEBUG_MON_CLEAR_STAT(DEBUG_MON_MET_MONTH)
        DEBUG_MON_CLEAR_STAT(DEBUG_MON_MET_DAY)
        if (mon->stats[MON_DATA_HATCH_LOCATION] == 0) {
            mon->stats[MON_DATA_HATCH_LOCATION] = 1;
        }
    }
    else {
        DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_MET_LOCATION, MON_DATA_MET_LOCATION)
        DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_MET_YEAR, MON_DATA_MET_YEAR)
        DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_MET_MONTH, MON_DATA_MET_MONTH)
        DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_MET_DAY, MON_DATA_MET_DAY)
    }
    
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_HATCH_LOCATION, MON_DATA_HATCH_LOCATION)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_HATCH_YEAR, MON_DATA_HATCH_YEAR)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_HATCH_MONTH, MON_DATA_HATCH_MONTH)
    DEBUG_MON_SET_MON_DATA_FROM_STAT(DEBUG_MON_HATCH_DAY, MON_DATA_HATCH_DAY)
    
    Pokemon_CalcLevelAndStats(mon->monData);
}

static void DebugMon_SetMoveAtPosition (Pokemon *mon, u16 moveID, u16 movePos)
{
    u8 pp = MoveTable_LoadParam(moveID, MOVEATTRIBUTE_PP);
    Pokemon_SetValue(mon, MON_DATA_MOVE1 + movePos, &moveID);
    Pokemon_SetValue(mon, MON_DATA_MOVE1_CUR_PP + movePos, &pp);
}

static u8 DebugMonValue_Display (DebugMonMenu *monMenu, u8 statID, u32 color, u8 y)
{
    DebugMon *mon = &monMenu->mon;

    if (statID == DEBUG_MON_BATTLE_PARAM1) {
        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_LABEL_35, 12, y, 0xFF, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_MAX_HP], 3, 12 + 24, y + 16, 0xFF, DMM_COLOR_BLACK);

        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_LABEL_36, 12 + 72, y, 0xFF, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_ATK], 3, 12 + 72 + 24, y + 16, 0xFF, DMM_COLOR_BLACK);

        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_LABEL_37, 12 + 72 + 72, y, 0xFF, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_DEF], 3, 12 + 72 + 72 + 24, y + 16, 0xFF, DMM_COLOR_BLACK);
        return 0;
    }

    if (statID == DEBUG_MON_BATTLE_PARAM2) {
        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_LABEL_38, 12, y, 0xFF, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_SPEED], 3, 12 +24, y + 16, 0xFF, DMM_COLOR_BLACK);

        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_LABEL_39, 12 + 72, y, 0xFF, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_SP_ATK], 3, 12 + 72 + 24, y + 16, 0xFF, DMM_COLOR_BLACK);

        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_LABEL_40, 12 + 72 + 72, y, 0xFF, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_SP_DEF], 3, 12 + 72 + 72 + 24, y + 16, 0xFF, DMM_COLOR_BLACK);
        return 0;
    }

    DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, sDebugMonValueList[statID].label, 12, y, 0xFF, color );

    u8 count = sDebugMonValueList[statID].value->count;

    switch(statID) {
    case DEBUG_MON_SPECIES:
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[statID], count, 12 + 72 + 24, y, 0xFF, color);
        DebugMonValue_PrintSpeciesName(&monMenu->mainWindow, mon->stats[statID], 12 + 72 + 24 + 32, y, 0xFF, DMM_COLOR_BLACK);
        break;

    case DEBUG_MON_GENDER:
        switch(mon->stats[statID]) {
        case 0:
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_STRING_01, 12 + 72 + 24, y, 0xFF, DMM_COLOR_BLACK);
            break;
        case 1:
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_STRING_02, 12 + 72 + 24, y, 0xFF, DMM_COLOR_BLACK);
            break;
        case 2:
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_STRING_00, 12 + 72 + 24, y, 0xFF, DMM_COLOR_BLACK);
            break;
        }
        break;

    case DEBUG_MON_MOVE1:
    case DEBUG_MON_MOVE2:
    case DEBUG_MON_MOVE3:
    case DEBUG_MON_MOVE4:
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[statID], count, 12 + 72 + 24, y, 0xFF, color);
        StringTemplate_SetMoveName(monMenu->strTemplate, 0, mon->stats[statID]);
        DebugMonValue_PrintStrExpanded(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, DMV_STRING_08, 12 + 72 + 24 + 32, y, 0xFF, color);
        break;

    case DEBUG_MON_HELD_ITEM:
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[statID], count, 12 + 72 + 24, y, 0xFF, color);
        StringTemplate_SetItemName(monMenu->strTemplate, 0, mon->stats[statID]);
        DebugMonValue_PrintStrExpanded(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, DMV_STRING_09, 12 + 72 + 24 + 32, y, 0xFF, color);
        break;

    case DEBUG_MON_ABILITY:
        StringTemplate_SetAbilityName(monMenu->strTemplate, 0, PokemonPersonalData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], MON_DATA_ABILITY + mon->stats[statID]));
        DebugMonValue_PrintStrExpanded(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, DMV_STRING_10, 12 + 72 + 24, y, 0xFF, color);
        break;

    case DEBUG_MON_STATUS_CONDITION:
        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, sStatusConditionTable[mon->stats[statID]][0], 12 + 72 + 24, y, 0xFF, DMM_COLOR_BLACK);
        break;

    case DEBUG_MON_OT_ID:
    case DEBUG_MON_PERSONALITY:
    case DEBUG_MON_HP_IV:
    case DEBUG_MON_ATK_IV:
    case DEBUG_MON_DEF_IV:
    case DEBUG_MON_SPEED_IV:
    case DEBUG_MON_SPATK_IV:
    case DEBUG_MON_SPDEF_IV:
    case DEBUG_MON_LEVEL:
    case DEBUG_MON_HP_EV:
    case DEBUG_MON_ATK_EV:
    case DEBUG_MON_DEF_EV:
    case DEBUG_MON_SPEED_EV:
    case DEBUG_MON_SPATK_EV:
    case DEBUG_MON_SPDEF_EV:
    case DEBUG_MON_COOL:
    case DEBUG_MON_BEAUTY:
    case DEBUG_MON_CUTE:
    case DEBUG_MON_SMART:
    case DEBUG_MON_TOUGH:
    case DEBUG_MON_SHEEN:
    case DEBUG_MON_FRIENDSHIP:
    case DEBUG_MON_POKERUS:
    case DEBUG_MON_EXP:
    case DEBUG_MON_MET_LOCATION:
    case DEBUG_MON_MET_LEVEL:
    case DEBUG_MON_MET_GAME:
    case DEBUG_MON_POKEBALL:
    case DEBUG_MON_MET_GAME_REGION_CODE:
    case DEBUG_MON_FORM:
    case DEBUG_MON_MET_YEAR:
    case DEBUG_MON_MET_MONTH:
    case DEBUG_MON_MET_DAY:
    case DEBUG_MON_HATCH_LOCATION:
    case DEBUG_MON_HATCH_YEAR:
    case DEBUG_MON_HATCH_MONTH:
    case DEBUG_MON_HATCH_DAY:
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[statID], count, 12 + 72 + 24, y, 0xFF, color);
        break;

    case DEBUG_MON_IS_EGG:
    case DEBUG_MON_FATEFUL_ENCOUNTER:
    case DEBUG_MON_HAS_NICKNAME:
        if(mon->stats[statID]) {
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_STRING_04, 12 + 72 + 24, y, 0xFF, DMM_COLOR_BLACK);
        } else {
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DMV_STRING_05, 12 + 72 + 24, y, 0xFF, DMM_COLOR_BLACK);
        }
        break;

    case DEBUG_MON_NATURE:
        StringTemplate_SetNatureName(monMenu->strTemplate, 0, mon->stats[statID]);
        DebugMonValue_PrintStrExpanded(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, DMV_STRING_11, 12 + 72 + 24, y, 0xFF, color);
        break;

    default:
        break;
    }

    return 0;
}

static void DebugMonValue_PrintStr (Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, entryID);

    // print color string
    Text_AddPrinterWithParamsAndColor(window, 0, buf, x, y, delay, color, NULL);
    Strbuf_Free(buf);
}

static void DebugMonValue_PrintStrExpanded (Window *window, MessageLoader *msgLoader, StringTemplate *strTemplate, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, entryID);
    Strbuf *bufExp = Strbuf_Init(128, HEAP_ID_APPLICATION);

    StringTemplate_Format(strTemplate, bufExp, buf);
    
    // print color string
    Text_AddPrinterWithParamsAndColor(window, 0, bufExp, x, y, delay, color, NULL);

    Strbuf_Free(buf);
    Strbuf_Free(bufExp);
}

static void DebugMonValue_PrintNum (Window *window, MessageLoader *msgLoader, StringTemplate *strTemplate, DebugMon *mon, u32 num, u32 digits, u32 x, u32 y, u32 delay, u32 color)
{
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, DMV_STRING_06);
    Strbuf *bufExp = Strbuf_Init(32, HEAP_ID_APPLICATION);
    
    u32 i;
    u32 tmp = 1;
    
    for (i = 0; i < digits - 1; i++) {
        tmp *= 10;
    }

    u32 size = 0;
    for (i = digits; i >= 1; i--) {
        StringTemplate_SetNumber(strTemplate, 0, num / tmp, 1, PADDING_MODE_ZEROES, CHARSET_MODE_EN);
        StringTemplate_Format(strTemplate, bufExp, buf);
        
        // print color string
        Text_AddPrinterWithParamsAndColor(window, 0, bufExp, x + size, y, 0xFF, DebugMonValue_GetColor(mon, i - 1, color), NULL);
        num %= tmp;
        tmp /= 10;

        // get string font width?
        size += Font_CalcStrbufWidth(0, bufExp, 0);
    }

    if (delay == 0) {
        // update window at vblank?
        sub_0201A9A4(window);
    }

    Strbuf_Free(buf);
    Strbuf_Free(bufExp);
}

// ravetodo better name probably
static u32 DebugMonValue_GetColor (DebugMon *mon, u8 digit, u32 color)
{
    if (color == DMM_COLOR_RED) {
        if (digit == mon->value) {
            return DMM_COLOR_BLUE;
        } else {
            return DMM_COLOR_RED;
        }
    }
    return DMM_COLOR_BLACK;
}

static void DebugMonValue_PrintSpeciesName (Window *window, u32 species, u32 x, u32 y, u32 delay, u32 color)
{
    MessageLoader *msgLoader = MessageLoader_Init(MESSAGE_LOADER_NARC_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, 412, HEAP_ID_APPLICATION);
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, species);

    // print color string
    Text_AddPrinterWithParamsAndColor(window, 0, buf, x, y, delay, color, NULL);

    Strbuf_Free(buf);
    MessageLoader_Free(msgLoader);
}

static u32 DebugMonValue_Get (DebugMon *mon, u8 statID)
{
    return mon->stats[statID];
}

static void DebugMonValue_Set (DebugMon *mon, u32 value, u8 statID)
{
    switch(statID) {
    case DEBUG_MON_SPECIES:
    case DEBUG_MON_PERSONALITY:
    case DEBUG_MON_OT_ID:
        mon->stats[statID] = value;
        DebugMon_CalcInitialStats(mon);
        break;

    case DEBUG_MON_LEVEL:
        mon->stats[statID] = value;
        mon->stats[DEBUG_MON_EXP] = Pokemon_GetSpeciesBaseExpAt(mon->stats[DEBUG_MON_SPECIES], value);
        DebugMon_CalcInitialStats(mon);
        break;

    case DEBUG_MON_EXP:
        mon->stats[statID] = value;
        DebugMon_CalcStatsFromExp(mon);
        break;

    case DEBUG_MON_HP_IV:
    case DEBUG_MON_ATK_IV:
    case DEBUG_MON_DEF_IV:
    case DEBUG_MON_SPEED_IV:
    case DEBUG_MON_SPATK_IV:
    case DEBUG_MON_SPDEF_IV:
    case DEBUG_MON_HP_EV:
    case DEBUG_MON_ATK_EV:
    case DEBUG_MON_DEF_EV:
    case DEBUG_MON_SPEED_EV:
    case DEBUG_MON_SPATK_EV:
    case DEBUG_MON_SPDEF_EV:
        mon->stats[statID] = value;
        DebugMon_CalcBaseStats(mon);
        break;

    default:
        mon->stats[statID] = value;
        break;
    }
}