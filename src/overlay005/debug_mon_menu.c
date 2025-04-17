#include "overlay005/debug_mon_menu.h"

#include "constants/charcode.h"
#include "constants/heap.h"
#include "constants/map_object.h"
#include "constants/narc.h"
#include "constants/pokemon.h"
#include "generated/genders.h"
#include "generated/items.h"
#include "generated/moves.h"
#include "generated/species.h"
#include "generated/text_banks.h"

#include "struct_defs/struct_0203D8AC.h"
#include "struct_defs/struct_02090800.h"

#include "field/field_system.h"
#include "overlay005/debug_menu.h"
#include "overlay006/ov6_02243258.h"

#include "field_map_change.h"
#include "field_system.h"
#include "message.h"
#include "move_table.h"
#include "party.h"
#include "pc_boxes.h"
#include "render_window.h"
#include "strbuf.h"
#include "sys_task.h"
#include "system.h"
#include "text.h"
#include "unk_0200F174.h"
#include "unk_0203D1B8.h"
#include "unk_0206B70C.h"
#include "unk_02092494.h"

#include "res/text/bank/unk_0336.h"

static void DebugMonMenu_PrintString(Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color);
static void DebugMonMenu_SetTrainerMemo(DebugMonMenu *monMenu, BOOL playerIsOT);
static void DebugMonMenu_DisplayCursor(DebugMonMenu *monMenu, u8 mode);
static void DebugMonMenu_ChangeValue(DebugMon *mon, u8 mode);
static void DebugMonMenu_DisplayPage(DebugMonMenu *monMenu);
static void DebugMonMenu_DisplayValues(DebugMonMenu *monMenu);
static u8 DebugMonMenu_AddMon(DebugMonMenu *monMenu);
static void DebugMonMenu_SetEditMon(DebugMonMenu *monMenu, u8 partySlot);

static void DebugMon_InitStats(DebugMon *mon);
static void DebugMon_CalcInitialStats(DebugMon *mon);
static void DebugMon_CalcFullStats(DebugMonMenu *monMenu, DebugMon *mon);
static void DebugMon_CalcBaseStats(DebugMon *mon);
static void DebugMon_CalcStatsFromExp(DebugMon *mon);
static void DebugMon_SetStatsFromMonData(DebugMon *mon);
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

static const DebugMonValue sDebugValueSpeciesStats = { 1, SPECIES_EGG - 1, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueLevel = { 1, 100, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueExpPoints = { 1, 2000000, DMMD_BOTH, 7 };
static const DebugMonValue sDebugValueOTID = { 0, 0xFFFFFFFF, DMMD_BOTH, 10 };
static const DebugMonValue sDebugValuePersonality = { 0, 0xFFFFFFFF, DMMD_BOTH, 10 };
static const DebugMonValue sDebugValueGender = { 0, 2, DMMD_BOTH, DMV_COUNT_NONE };
static const DebugMonValue sDebugValueNature = { 0, 24, DMMD_BOTH, DMV_COUNT_NONE };
static const DebugMonValue sDebugValueMoves = { 0, MAX_MOVES, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueHeldItem = { 0, MAX_ITEMS, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueAbility = { 0, 1, DMMD_BOTH, DMV_COUNT_NONE };
static const DebugMonValue sDebugValueIVs = { 0, MAX_IVS_SINGLE_STAT, DMMD_BOTH, 2 };
static const DebugMonValue sDebugValueEVs = { 0, MAX_EVS_SINGLE_STAT, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueContestStats = { 0, 255, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueSheen = { 0, 255, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueFriendship = { 0, 255, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValuePokerus = { 0, 255, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueIsEgg = { 0, 1, DMMD_BOTH, DMV_COUNT_NONE };
static const DebugMonValue sDebugValueStatus = { 0, 6, DMMD_BOTH, DMV_COUNT_NONE };
static const DebugMonValue sDebugValueIsFatefulEncounter = { 0, 1, DMMD_BOTH, DMV_COUNT_NONE };
static const DebugMonValue sDebugValueLocation = { 0, 9999, DMMD_BOTH, 4 };
static const DebugMonValue sDebugValueMetLevel = { 0, 100, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueMetGame = { 1, 15, DMMD_BOTH, 2 };
static const DebugMonValue sDebugValuePokeball = { 1, 16, DMMD_BOTH, 2 };
static const DebugMonValue sDebugValueMetRegion = { 1, 8, DMMD_BOTH, 1 };
static const DebugMonValue sDebugValueForm = { 0, 255, DMMD_BOTH, 3 };
static const DebugMonValue sDebugValueYear = { 0, 99, DMMD_BOTH, 2 };
static const DebugMonValue sDebugValueMonth = { 1, 12, DMMD_BOTH, 2 };
static const DebugMonValue sDebugValueDay = { 1, 31, DMMD_BOTH, 2 };
static const DebugMonValue sDebugValueHasNickname = { 0, 1, DMMD_BOTH, DMV_COUNT_NONE };

static const DebugMonValueEntry sDebugMonValueList[MAX_DEBUG_MON_STAT] = {
    [DEBUG_MON_SPECIES] = { dmm_label_species, &sDebugValueSpeciesStats },
    [DEBUG_MON_LEVEL] = { dmm_label_level, &sDebugValueLevel },
    [DEBUG_MON_EXP] = { dmm_label_exp_points, &sDebugValueExpPoints },
    [DEBUG_MON_OT_ID] = { dmm_label_ot_id, &sDebugValueOTID },
    [DEBUG_MON_PERSONALITY] = { dmm_label_personality, &sDebugValuePersonality },
    [DEBUG_MON_GENDER] = { dmm_label_gender, &sDebugValueGender },
    [DEBUG_MON_NATURE] = { dmm_label_nature, &sDebugValueNature },
    [DEBUG_MON_MOVE1] = { dmm_label_move_1, &sDebugValueMoves },
    [DEBUG_MON_MOVE2] = { dmm_label_move_2, &sDebugValueMoves },
    [DEBUG_MON_MOVE3] = { dmm_label_move_3, &sDebugValueMoves },
    [DEBUG_MON_MOVE4] = { dmm_label_move_4, &sDebugValueMoves },
    [DEBUG_MON_HELD_ITEM] = { dmm_label_held_item, &sDebugValueHeldItem },
    [DEBUG_MON_ABILITY] = { dmm_label_ability, &sDebugValueAbility },
    [DEBUG_MON_HP_IV] = { dmm_label_hp_iv, &sDebugValueIVs },
    [DEBUG_MON_HP_EV] = { dmm_label_hp_ev, &sDebugValueEVs },
    [DEBUG_MON_ATK_IV] = { dmm_label_attack_iv, &sDebugValueIVs },
    [DEBUG_MON_ATK_EV] = { dmm_label_attack_ev, &sDebugValueEVs },
    [DEBUG_MON_DEF_IV] = { dmm_label_defense_iv, &sDebugValueIVs },
    [DEBUG_MON_DEF_EV] = { dmm_label_defense_ev, &sDebugValueEVs },
    [DEBUG_MON_SPEED_IV] = { dmm_label_speed_iv, &sDebugValueIVs },
    [DEBUG_MON_SPEED_EV] = { dmm_label_speed_ev, &sDebugValueEVs },
    [DEBUG_MON_SPATK_IV] = { dmm_label_sp_attack_iv, &sDebugValueIVs },
    [DEBUG_MON_SPATK_EV] = { dmm_label_sp_attack_ev, &sDebugValueEVs },
    [DEBUG_MON_SPDEF_IV] = { dmm_label_sp_defense_iv, &sDebugValueIVs },
    [DEBUG_MON_SPDEF_EV] = { dmm_label_sp_defense_ev, &sDebugValueEVs },
    [DEBUG_MON_COOL] = { dmm_label_cool, &sDebugValueContestStats },
    [DEBUG_MON_BEAUTY] = { dmm_label_beauty, &sDebugValueContestStats },
    [DEBUG_MON_CUTE] = { dmm_label_cute, &sDebugValueContestStats },
    [DEBUG_MON_SMART] = { dmm_label_smart, &sDebugValueContestStats },
    [DEBUG_MON_TOUGH] = { dmm_label_tough, &sDebugValueContestStats },
    [DEBUG_MON_SHEEN] = { dmm_label_sheen, &sDebugValueSheen },
    [DEBUG_MON_FRIENDSHIP] = { dmm_label_friendship, &sDebugValueFriendship },
    [DEBUG_MON_POKERUS] = { dmm_label_pokerus, &sDebugValuePokerus },
    [DEBUG_MON_IS_EGG] = { dmm_label_egg, &sDebugValueIsEgg },
    [DEBUG_MON_STATUS_CONDITION] = { dmm_label_status, &sDebugValueStatus },
    [DEBUG_MON_MAX_HP] = { dmm_label_hp, &sDebugValueSpeciesStats },
    [DEBUG_MON_ATK] = { dmm_label_attack, &sDebugValueSpeciesStats },
    [DEBUG_MON_DEF] = { dmm_label_defense, &sDebugValueSpeciesStats },
    [DEBUG_MON_SPEED] = { dmm_label_speed, &sDebugValueSpeciesStats },
    [DEBUG_MON_SP_ATK] = { dmm_label_sp_attack, &sDebugValueSpeciesStats },
    [DEBUG_MON_SP_DEF] = { dmm_label_sp_defense, &sDebugValueSpeciesStats },
    [DEBUG_MON_FATEFUL_ENCOUNTER] = { dmm_label_fateful_encounter, &sDebugValueIsFatefulEncounter },
    [DEBUG_MON_MET_LOCATION] = { dmm_label_met_location, &sDebugValueLocation },
    [DEBUG_MON_MET_LEVEL] = { dmm_label_met_level, &sDebugValueMetLevel },
    [DEBUG_MON_MET_GAME] = { dmm_label_met_game, &sDebugValueMetGame },
    [DEBUG_MON_POKEBALL] = { dmm_label_pokeball, &sDebugValuePokeball },
    [DEBUG_MON_MET_GAME_REGION_CODE] = { dmm_label_met_region, &sDebugValueMetRegion },
    [DEBUG_MON_FORM] = { dmm_label_form, &sDebugValueForm },
    [DEBUG_MON_MET_YEAR] = { dmm_label_year, &sDebugValueYear },
    [DEBUG_MON_MET_MONTH] = { dmm_label_month, &sDebugValueMonth },
    [DEBUG_MON_MET_DAY] = { dmm_label_day, &sDebugValueDay },
    [DEBUG_MON_HATCH_LOCATION] = { dmm_label_hatch_location, &sDebugValueLocation },
    [DEBUG_MON_HATCH_YEAR] = { dmm_label_year, &sDebugValueYear },
    [DEBUG_MON_HATCH_MONTH] = { dmm_label_month, &sDebugValueMonth },
    [DEBUG_MON_HATCH_DAY] = { dmm_label_day, &sDebugValueDay },
    [DEBUG_MON_HAS_NICKNAME] = { dmm_label_nickname, &sDebugValueHasNickname },
};

static const u32 sDebugMonIncrements[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
};

static const u32 sStatusConditionTable[][2] = {
    { dmm_label_status_normal, 0x00000000 },
    { dmm_label_status_sleep, 0x00000001 },
    { dmm_label_status_poison, 0x00000008 },
    { dmm_label_status_burned, 0x00000010 },
    { dmm_label_status_frozen, 0x00000020 },
    { dmm_label_status_paralysed, 0x00000040 },
    { dmm_label_status_badly_poisoned, 0x00000080 },
};

static const u8 sDebugMonPage1[] = {
    DEBUG_MON_SPECIES,
    DEBUG_MON_LEVEL,
    DEBUG_MON_EXP,
    DEBUG_MON_OT_ID,
    DEBUG_MON_PERSONALITY,
    DEBUG_MON_GENDER,
    DEBUG_MON_NATURE,
    DEBUG_MON_PAGE_END,
};

static const u8 sDebugMonPage2[] = {
    DEBUG_MON_STATUS_CONDITION,
    DEBUG_MON_FRIENDSHIP,
    DEBUG_MON_POKERUS,
    DEBUG_MON_IS_EGG,
    DEBUG_MON_FATEFUL_ENCOUNTER,
    DEBUG_MON_HAS_NICKNAME,
    DEBUG_MON_PAGE_END,
};

static const u8 sDebugMonPage3[] = {
    DEBUG_MON_MOVE1,
    DEBUG_MON_MOVE2,
    DEBUG_MON_MOVE3,
    DEBUG_MON_MOVE4,
    DEBUG_MON_HELD_ITEM,
    DEBUG_MON_ABILITY,
    DEBUG_MON_PAGE_END,
};

static const u8 sDebugMonPage4[] = {
    DEBUG_MON_HP_IV,
    DEBUG_MON_HP_EV,
    DEBUG_MON_ATK_IV,
    DEBUG_MON_ATK_EV,
    DEBUG_MON_DEF_IV,
    DEBUG_MON_DEF_EV,
    DEBUG_MON_BATTLE_PARAM1,
    DEBUG_MON_PAGE_END,
};

static const u8 sDebugMonPage5[] = {
    DEBUG_MON_SPEED_IV,
    DEBUG_MON_SPEED_EV,
    DEBUG_MON_SPATK_IV,
    DEBUG_MON_SPATK_EV,
    DEBUG_MON_SPDEF_IV,
    DEBUG_MON_SPDEF_EV,
    DEBUG_MON_BATTLE_PARAM2,
    DEBUG_MON_PAGE_END,
};

static const u8 sDebugMonPage6[] = {
    DEBUG_MON_COOL,
    DEBUG_MON_BEAUTY,
    DEBUG_MON_CUTE,
    DEBUG_MON_SMART,
    DEBUG_MON_TOUGH,
    DEBUG_MON_SHEEN,
    DEBUG_MON_PAGE_END,
};

static const u8 sDebugMonPage7[] = {
    DEBUG_MON_MET_LEVEL,
    DEBUG_MON_MET_GAME,
    DEBUG_MON_POKEBALL,
    DEBUG_MON_MET_GAME_REGION_CODE,
    DEBUG_MON_FORM,
    DEBUG_MON_PAGE_END,
};

static const u8 sDebugMonPage8[] = {
    DEBUG_MON_MET_LOCATION,
    DEBUG_MON_MET_YEAR,
    DEBUG_MON_MET_MONTH,
    DEBUG_MON_MET_DAY,
    DEBUG_MON_HATCH_LOCATION,
    DEBUG_MON_HATCH_YEAR,
    DEBUG_MON_HATCH_MONTH,
    DEBUG_MON_HATCH_DAY,
    DEBUG_MON_PAGE_END,
};

static const DebugMonMenuPage sDebugMonMenuPages[DEBUG_MON_MENU_MAX_PAGES] = {
    { sDebugMonPage1, 5 },
    { sDebugMonPage2, 6 },
    { sDebugMonPage3, 6 },
    { sDebugMonPage4, 6 },
    { sDebugMonPage5, 6 },
    { sDebugMonPage6, 6 },
    { sDebugMonPage7, 5 },
    { sDebugMonPage8, 8 }
};

void DebugMonMenu_HandleInput(DebugMonMenu *monMenu)
{
    u8 result;

    if (JOY_NEW(PAD_BUTTON_A)) {
        monMenu->mon.statBackup = monMenu->mon.stats[sDebugMonMenuPages[monMenu->mon.page].page[monMenu->mon.cursor]];
        monMenu->mon.value = 0;
        DebugMonMenu_DisplayValues(monMenu);
        monMenu->state = DMM_STATE_HANDLE_VALUE_INPUT;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_B)) {
        monMenu->state = DMM_STATE_EXIT_MENU;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_START)) {
        if (monMenu->mode != DEBUG_MON_MENU_MODE_EDIT) {
            DebugMon_CalcFullStats(monMenu, &monMenu->mon);
        } else {
            DebugMon_SetMonDataFromStats(&monMenu->mon);
        }

        result = DebugMonMenu_AddMon(monMenu);

        Window_FillRectWithColor(&monMenu->mainWindow, 15, 24, 64, 30 * 8 - 24, 32);

        if (result == 1) {
            DebugMonMenu_PrintString(&monMenu->mainWindow, monMenu->msgLoader, dmm_mon_added_to_pc, 32, 72, TEXT_SPEED_INSTANT, DMM_COLOR_BLACK);
        } else {
            if (monMenu->mode != DEBUG_MON_MENU_MODE_EDIT) {
                DebugMonMenu_PrintString(&monMenu->mainWindow, monMenu->msgLoader, dmm_mon_added_to_party, 32, 72, TEXT_SPEED_INSTANT, DMM_COLOR_BLACK);
            } else {
                DebugMonMenu_PrintString(&monMenu->mainWindow, monMenu->msgLoader, dmm_mon_edited, 32, 72, TEXT_SPEED_INSTANT, DMM_COLOR_BLACK);
            }
        }

        monMenu->state = DMM_STATE_WAIT_A_BUTTON_PRESS;
        return;
    }

    if (JOY_NEW(PAD_KEY_UP)) {
        DebugMonMenu_DisplayCursor(monMenu, DMMD_DECREASE);
        return;
    }

    if (JOY_NEW(PAD_KEY_DOWN)) {
        DebugMonMenu_DisplayCursor(monMenu, DMMD_INCREASE);
        return;
    }

    if (JOY_NEW(PAD_BUTTON_L) && monMenu->mon.page > 0) {
        monMenu->mon.page--;
        monMenu->mon.cursor = 0;
        monMenu->state = DMM_STATE_DRAW_MENU;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_R) && monMenu->mon.page < DEBUG_MON_MENU_MAX_PAGES - 1) {
        monMenu->mon.page++;
        monMenu->mon.cursor = 0;
        monMenu->state = DMM_STATE_DRAW_MENU;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_X)) {
        if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
            int partyCount = Party_GetCurrentCount(SaveData_GetParty(monMenu->sys->saveData));

            if (monMenu->partySlot == 0) {
                monMenu->partySlot = partyCount - 1;
            } else {
                monMenu->partySlot--;
            }

            DebugMonMenu_SetEditMon(monMenu, monMenu->partySlot);
            monMenu->state = DMM_STATE_DRAW_MENU;
        } else {
            DebugMonMenu_SetTrainerMemo(monMenu, TRUE);
            monMenu->state = DMM_STATE_WAIT_XY_BUTTON_PRESS;
        }
        return;
    }

    if (JOY_NEW(PAD_BUTTON_Y)) {
        if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
            int partyCount = Party_GetCurrentCount(SaveData_GetParty(monMenu->sys->saveData));

            if (monMenu->partySlot == partyCount - 1) {
                monMenu->partySlot = 0;
            } else {
                monMenu->partySlot++;
            }

            DebugMonMenu_SetEditMon(monMenu, monMenu->partySlot);
            monMenu->state = DMM_STATE_DRAW_MENU;
        } else {
            DebugMonMenu_SetTrainerMemo(monMenu, TRUE);
            monMenu->state = DMM_STATE_WAIT_XY_BUTTON_PRESS;
        }
    }
}

void DebugMonMenu_HandleValueInput(DebugMonMenu *monMenu)
{
    u8 statID = sDebugMonMenuPages[monMenu->mon.page].page[monMenu->mon.cursor];

    if (JOY_NEW(PAD_BUTTON_A)) {
        monMenu->state = DMM_STATE_DRAW_MENU;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_B)) {
        monMenu->mon.stats[statID] = monMenu->mon.statBackup;
        monMenu->state = DMM_STATE_DRAW_MENU;
        return;
    }

    if (JOY_HELD(PAD_KEY_UP)) {
        DebugMonMenu_ChangeValue(&monMenu->mon, DMMD_INCREASE);
        DebugMonMenu_DisplayValues(monMenu);
        return;
    }

    if (JOY_HELD(PAD_KEY_DOWN)) {
        DebugMonMenu_ChangeValue(&monMenu->mon, DMMD_DECREASE);
        DebugMonMenu_DisplayValues(monMenu);
        return;
    }

    if (sDebugMonValueList[statID].value->count != DMV_COUNT_NONE) {
        if (JOY_NEW(PAD_KEY_LEFT) && monMenu->mon.value < sDebugMonValueList[statID].value->count - 1) {
            monMenu->mon.value++;
            DebugMonMenu_DisplayValues(monMenu);
            return;
        }
        if (JOY_NEW(PAD_KEY_RIGHT) && monMenu->mon.value > 0) {
            monMenu->mon.value--;
            DebugMonMenu_DisplayValues(monMenu);
        }
    }
}

void DebugMonMenu_WaitButtonPress(DebugMonMenu *monMenu, int buttonMask)
{
    if (JOY_NEW(buttonMask)) {
        monMenu->state = DMM_STATE_DRAW_MENU;
    }
}

void DebugMonMenu_Init(DebugMonMenu *monMenu)
{
    Window_FillTilemap(&monMenu->titleWindow, 15);
    if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
        DebugMonMenu_PrintString(&monMenu->titleWindow, monMenu->msgLoader, dmm_instructions_edit_mon, 0, 0, TEXT_SPEED_INSTANT, DMM_COLOR_BLACK);
    } else {
        DebugMonMenu_PrintString(&monMenu->titleWindow, monMenu->msgLoader, dmm_instructions_create_mon, 0, 0, TEXT_SPEED_INSTANT, DMM_COLOR_BLACK);
    }

    monMenu->mon.monData = Pokemon_New(HEAP_ID_APPLICATION);

    if (monMenu->mode == DEBUG_MON_MENU_MODE_CREATE) {
        DebugMon_InitStats(&monMenu->mon);
    } else if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
        DebugMonMenu_SetEditMon(monMenu, monMenu->partySlot);
    }

    monMenu->mon.page = 0;
    monMenu->mon.cursor = 0;
}

void DebugMonMenu_Free(DebugMonMenu *monMenu)
{
    Heap_FreeToHeap(monMenu->mon.monData);

    Window_EraseStandardFrame(&monMenu->titleWindow, TRUE);
    Window_EraseStandardFrame(&monMenu->mainWindow, FALSE);

    Window_ClearAndCopyToVRAM(&monMenu->titleWindow);
    Window_ClearAndCopyToVRAM(&monMenu->mainWindow);

    Window_Remove(&monMenu->titleWindow);
    Window_Remove(&monMenu->mainWindow);

    MessageLoader_Free(monMenu->msgLoader);
    StringTemplate_Free(monMenu->strTemplate);
    ColoredArrow_Free(monMenu->cursor);
}

static void DebugMonMenu_PrintString(Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, entryID);
    Text_AddPrinterWithParamsAndColor(window, 0, buf, x, y, delay, color, NULL);
    Strbuf_Free(buf);
}

#define DISPLAY_INFO_PRINT(info)                                                                                                          \
    {                                                                                                                                     \
        if (info.unk_04 != NULL) {                                                                                                        \
            Text_AddPrinterWithParamsAndColor(&monMenu->mainWindow, 0, info.unk_04, 8, (info.unk_00 - 1) * 16, 0, DMM_COLOR_BLACK, NULL); \
        }                                                                                                                                 \
    }

static void DebugMonMenu_SetTrainerMemo(DebugMonMenu *monMenu, BOOL playerIsOT)
{
    if (monMenu->mode != DEBUG_MON_MENU_MODE_EDIT) {
        DebugMon_CalcFullStats(monMenu, &monMenu->mon);
    } else {
        DebugMon_SetMonDataFromStats(&monMenu->mon);
    }

    Window_FillTilemap(&monMenu->mainWindow, 15);

    // get display info?
    PokemonInfoDisplayStruct *infoDisplay = sub_02092494(monMenu->mon.monData, playerIsOT, HEAP_ID_APPLICATION);

    DISPLAY_INFO_PRINT(infoDisplay->unk_14)
    DISPLAY_INFO_PRINT(infoDisplay->unk_1C)
    DISPLAY_INFO_PRINT(infoDisplay->unk_24)
    DISPLAY_INFO_PRINT(infoDisplay->unk_2C)
    DISPLAY_INFO_PRINT(infoDisplay->unk_34)

    // free display info?
    sub_0209282C(infoDisplay);
}

static void DebugMonMenu_DisplayCursor(DebugMonMenu *monMenu, u8 mode)
{
    DebugMon *mon = &monMenu->mon;

    switch (mode) {
    case DMMD_BOTH:
        ColoredArrow_Print(monMenu->cursor, &monMenu->mainWindow, 0, mon->cursor * 16);
        Window_ScheduleCopyToVRAM(&monMenu->mainWindow);
        break;

    case DMMD_INCREASE:
        if (mon->cursor < sDebugMonMenuPages[mon->page].count - 1) {
            Window_FillRectWithColor(&monMenu->mainWindow, 15, 0, mon->cursor * 16, 12, 16);
            mon->cursor++;
            ColoredArrow_Print(monMenu->cursor, &monMenu->mainWindow, 0, mon->cursor * 16);
            Window_ScheduleCopyToVRAM(&monMenu->mainWindow);
        }
        break;

    case DMMD_DECREASE:
        if (mon->cursor > 0) {
            Window_FillRectWithColor(&monMenu->mainWindow, 15, 0, mon->cursor * 16, 12, 16);
            mon->cursor--;
            ColoredArrow_Print(monMenu->cursor, &monMenu->mainWindow, 0, mon->cursor * 16);
            Window_ScheduleCopyToVRAM(&monMenu->mainWindow);
        }
        break;
    }
}

static void DebugMonMenu_ChangeValue(DebugMon *mon, u8 mode)
{
    u32 currentValue = DebugMonValue_Get(mon, sDebugMonMenuPages[mon->page].page[mon->cursor]);
    u32 minValue = sDebugMonValueList[sDebugMonMenuPages[mon->page].page[mon->cursor]].value->min;
    u32 maxValue = sDebugMonValueList[sDebugMonMenuPages[mon->page].page[mon->cursor]].value->max;
    u32 valueChange = sDebugMonIncrements[mon->value];

    switch (sDebugMonValueList[sDebugMonMenuPages[mon->page].page[mon->cursor]].value->mode) {
    case DMMD_BOTH:
        switch (mode) {
        case DMMD_INCREASE:
            if (currentValue < maxValue - valueChange) {
                currentValue += valueChange;
            } else {
                if (currentValue < maxValue) {
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
                if (currentValue > minValue) {
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

void DebugMonMenu_DisplayPageAndCursor(DebugMonMenu *monMenu)
{
    DebugMonMenu_DisplayPage(monMenu);
    DebugMonMenu_DisplayCursor(monMenu, DMMD_BOTH);
    monMenu->state = DMM_STATE_HANDLE_INPUT;
}

static void DebugMonMenu_DisplayPage(DebugMonMenu *monMenu)
{
    DebugMon *mon = &monMenu->mon;

    Window_FillTilemap(&monMenu->mainWindow, 15);

    u16 i = 0;
    while (sDebugMonMenuPages[mon->page].page[i] != DEBUG_MON_PAGE_END) {
        DebugMonValue_Display(monMenu, sDebugMonMenuPages[mon->page].page[i], DMM_COLOR_BLACK, i * 16);
        i++;
    }

    Window_LoadTiles(&monMenu->mainWindow);
}

static void DebugMonMenu_DisplayValues(DebugMonMenu *monMenu)
{
    DebugMon *mon = &monMenu->mon;
    Window_FillTilemap(&monMenu->mainWindow, 15);

    u16 i = 0;
    while (sDebugMonMenuPages[mon->page].page[i] != DEBUG_MON_PAGE_END) {

        if (i == mon->cursor) {
            DebugMonValue_Display(monMenu, sDebugMonMenuPages[mon->page].page[i], DMM_COLOR_RED, i * 16);
        } else {
            DebugMonValue_Display(monMenu, sDebugMonMenuPages[mon->page].page[i], DMM_COLOR_BLACK, i * 16);
        }

        i++;
    }

    Window_LoadTiles(&monMenu->mainWindow);
}

static u8 DebugMonMenu_AddMon(DebugMonMenu *monMenu)
{
    Party *party;

    if (monMenu->mode == DEBUG_MON_MENU_MODE_CREATE) {
        party = SaveData_GetParty(monMenu->sys->saveData);

        if (Party_AddPokemon(party, monMenu->mon.monData) == TRUE) {
            return 0;
        }

        PCBoxes_TryStoreBoxMon(SaveData_GetPCBoxes(monMenu->sys->saveData), Pokemon_GetBoxPokemon(monMenu->mon.monData));

        return 1;

    } else if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
        party = SaveData_GetParty(monMenu->sys->saveData);

        Strbuf *buf;

        if (monMenu->mon.stats[DEBUG_MON_IS_EGG]) {
            buf = MessageLoader_GetNewStrbuf(monMenu->msgLoader, 112);
            Pokemon_SetValue(monMenu->mon.monData, MON_DATA_NICKNAME_STRBUF, buf);
            Strbuf_Free(buf);
        }

        Pokemon *mon = Party_GetPokemonBySlotIndex(party, monMenu->partySlot);

        buf = Strbuf_Init(16, HEAP_ID_APPLICATION);
        Pokemon_GetValue(mon, MON_DATA_OTNAME_STRBUF, buf);
        Pokemon_SetValue(monMenu->mon.monData, MON_DATA_OTNAME_STRBUF, buf);
        Strbuf_Free(buf);

        u8 gender = Pokemon_GetValue(mon, MON_DATA_OT_GENDER, NULL);
        Pokemon_SetValue(monMenu->mon.monData, MON_DATA_OT_GENDER, &gender);
        // set party data?
        Party_AddPokemonBySlotIndex(party, monMenu->partySlot, monMenu->mon.monData);
        return 0;
    }

    return 0;
}

static void DebugMonMenu_SetEditMon(DebugMonMenu *monMenu, u8 partySlot)
{
    Party *party = SaveData_GetParty(monMenu->sys->saveData);
    Pokemon *changeMon = Party_GetPokemonBySlotIndex(party, partySlot);
    MI_CpuCopy8(changeMon, monMenu->mon.monData, Pokemon_GetStructSize());
    DebugMon_SetStatsFromMonData(&monMenu->mon);
}

static void DebugMon_InitStats(DebugMon *mon)
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

static void DebugMon_CalcInitialStats(DebugMon *mon)
{
    u32 monIVs = ((mon->stats[DEBUG_MON_HP_IV] & 0x0000001f) << 0)
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

static void DebugMon_CalcFullStats(DebugMonMenu *monMenu, DebugMon *mon)
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

    u32 ability = SpeciesData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], SPECIES_DATA_ABILITY_1 + mon->stats[DEBUG_MON_ABILITY]);
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

    Strbuf *buf;

    if (mon->stats[DEBUG_MON_IS_EGG]) {
        buf = MessageLoader_GetNewStrbuf(monMenu->msgLoader, dmm_debug_egg_name);
        Pokemon_SetValue(mon->monData, MON_DATA_NICKNAME_STRBUF, buf);
        Strbuf_Free(buf);
    }

    buf = MessageLoader_GetNewStrbuf(monMenu->msgLoader, dmm_debug_ot);
    Pokemon_SetValue(mon->monData, MON_DATA_OTNAME_STRBUF, buf);
    Strbuf_Free(buf);

    Pokemon_CalcLevelAndStats(mon->monData);
}

static void DebugMon_CalcBaseStats(DebugMon *mon)
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

static void DebugMon_CalcStatsFromExp(DebugMon *mon)
{
    u32 nullData = 0;
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

static void DebugMon_SetStatsFromMonData(DebugMon *mon)
{
    mon->stats[DEBUG_MON_SPECIES] = Pokemon_GetValue(mon->monData, MON_DATA_SPECIES, NULL);
    mon->stats[DEBUG_MON_LEVEL] = Pokemon_GetValue(mon->monData, MON_DATA_LEVEL, NULL);
    mon->stats[DEBUG_MON_EXP] = Pokemon_GetValue(mon->monData, MON_DATA_EXP, NULL);
    mon->stats[DEBUG_MON_OT_ID] = Pokemon_GetValue(mon->monData, MON_DATA_OT_ID, NULL);
    mon->stats[DEBUG_MON_PERSONALITY] = Pokemon_GetValue(mon->monData, MON_DATA_PERSONALITY, NULL);
    mon->stats[DEBUG_MON_GENDER] = Pokemon_GetValue(mon->monData, MON_DATA_GENDER, NULL);
    mon->stats[DEBUG_MON_NATURE] = Pokemon_GetNature(mon->monData);
    mon->stats[DEBUG_MON_MOVE1] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE1, NULL);
    mon->stats[DEBUG_MON_MOVE2] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE2, NULL);
    mon->stats[DEBUG_MON_MOVE3] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE3, NULL);
    mon->stats[DEBUG_MON_MOVE4] = Pokemon_GetValue(mon->monData, MON_DATA_MOVE4, NULL);
    mon->stats[DEBUG_MON_HELD_ITEM] = Pokemon_GetValue(mon->monData, MON_DATA_HELD_ITEM, NULL);
    mon->stats[DEBUG_MON_ABILITY] = Pokemon_GetValue(mon->monData, MON_DATA_ABILITY, NULL);

    if (mon->stats[DEBUG_MON_ABILITY] == SpeciesData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], SPECIES_DATA_ABILITY_1)) {
        mon->stats[DEBUG_MON_ABILITY] = 0;
    } else {
        mon->stats[DEBUG_MON_ABILITY] = 1;
    }

    mon->stats[DEBUG_MON_HP_IV] = Pokemon_GetValue(mon->monData, MON_DATA_HP_IV, NULL);
    mon->stats[DEBUG_MON_HP_EV] = Pokemon_GetValue(mon->monData, MON_DATA_HP_EV, NULL);
    mon->stats[DEBUG_MON_ATK_IV] = Pokemon_GetValue(mon->monData, MON_DATA_ATK_IV, NULL);
    mon->stats[DEBUG_MON_ATK_EV] = Pokemon_GetValue(mon->monData, MON_DATA_ATK_EV, NULL);
    mon->stats[DEBUG_MON_DEF_IV] = Pokemon_GetValue(mon->monData, MON_DATA_DEF_IV, NULL);
    mon->stats[DEBUG_MON_DEF_EV] = Pokemon_GetValue(mon->monData, MON_DATA_DEF_EV, NULL);
    mon->stats[DEBUG_MON_SPEED_IV] = Pokemon_GetValue(mon->monData, MON_DATA_SPEED_IV, NULL);
    mon->stats[DEBUG_MON_SPEED_EV] = Pokemon_GetValue(mon->monData, MON_DATA_SPEED_EV, NULL);
    mon->stats[DEBUG_MON_SPATK_IV] = Pokemon_GetValue(mon->monData, MON_DATA_SPATK_IV, NULL);
    mon->stats[DEBUG_MON_SPATK_EV] = Pokemon_GetValue(mon->monData, MON_DATA_SPATK_EV, NULL);
    mon->stats[DEBUG_MON_SPDEF_IV] = Pokemon_GetValue(mon->monData, MON_DATA_SPDEF_IV, NULL);
    mon->stats[DEBUG_MON_SPDEF_EV] = Pokemon_GetValue(mon->monData, MON_DATA_SPDEF_EV, NULL);
    mon->stats[DEBUG_MON_COOL] = Pokemon_GetValue(mon->monData, MON_DATA_COOL, NULL);
    mon->stats[DEBUG_MON_BEAUTY] = Pokemon_GetValue(mon->monData, MON_DATA_BEAUTY, NULL);
    mon->stats[DEBUG_MON_CUTE] = Pokemon_GetValue(mon->monData, MON_DATA_CUTE, NULL);
    mon->stats[DEBUG_MON_SMART] = Pokemon_GetValue(mon->monData, MON_DATA_SMART, NULL);
    mon->stats[DEBUG_MON_TOUGH] = Pokemon_GetValue(mon->monData, MON_DATA_TOUGH, NULL);
    mon->stats[DEBUG_MON_SHEEN] = Pokemon_GetValue(mon->monData, MON_DATA_SHEEN, NULL);
    mon->stats[DEBUG_MON_FRIENDSHIP] = Pokemon_GetValue(mon->monData, MON_DATA_FRIENDSHIP, NULL);
    mon->stats[DEBUG_MON_POKERUS] = Pokemon_GetValue(mon->monData, MON_DATA_POKERUS, NULL);
    mon->stats[DEBUG_MON_IS_EGG] = Pokemon_GetValue(mon->monData, MON_DATA_IS_EGG, NULL);

    u32 statusCondition = Pokemon_GetValue(mon->monData, MON_DATA_STATUS_CONDITION, NULL);
    mon->stats[DEBUG_MON_STATUS_CONDITION] = 0;
    for (u32 i = 0; i < 7; i++) {
        if (statusCondition == sStatusConditionTable[i][1]) {
            mon->stats[DEBUG_MON_STATUS_CONDITION] = i;
            break;
        }
    }

    mon->stats[DEBUG_MON_MAX_HP] = Pokemon_GetValue(mon->monData, MON_DATA_MAX_HP, NULL);
    mon->stats[DEBUG_MON_ATK] = Pokemon_GetValue(mon->monData, MON_DATA_ATK, NULL);
    mon->stats[DEBUG_MON_DEF] = Pokemon_GetValue(mon->monData, MON_DATA_DEF, NULL);
    mon->stats[DEBUG_MON_SPEED] = Pokemon_GetValue(mon->monData, MON_DATA_SPEED, NULL);
    mon->stats[DEBUG_MON_SP_ATK] = Pokemon_GetValue(mon->monData, MON_DATA_SP_ATK, NULL);
    mon->stats[DEBUG_MON_SP_DEF] = Pokemon_GetValue(mon->monData, MON_DATA_SP_DEF, NULL);
    mon->stats[DEBUG_MON_FATEFUL_ENCOUNTER] = Pokemon_GetValue(mon->monData, MON_DATA_FATEFUL_ENCOUNTER, NULL);
    mon->stats[DEBUG_MON_HAS_NICKNAME] = Pokemon_GetValue(mon->monData, MON_DATA_HAS_NICKNAME, NULL);
    mon->stats[DEBUG_MON_MET_LEVEL] = Pokemon_GetValue(mon->monData, MON_DATA_MET_LEVEL, NULL);
    mon->stats[DEBUG_MON_MET_GAME] = Pokemon_GetValue(mon->monData, MON_DATA_MET_GAME, NULL);
    mon->stats[DEBUG_MON_POKEBALL] = Pokemon_GetValue(mon->monData, MON_DATA_POKEBALL, NULL);
    mon->stats[DEBUG_MON_MET_GAME_REGION_CODE] = Pokemon_GetValue(mon->monData, MON_DATA_MET_GAME_REGION_CODE, NULL);
    mon->stats[DEBUG_MON_FORM] = Pokemon_GetValue(mon->monData, MON_DATA_FORM, NULL);
    mon->stats[DEBUG_MON_MET_LOCATION] = Pokemon_GetValue(mon->monData, MON_DATA_MET_LOCATION, NULL);
    mon->stats[DEBUG_MON_MET_YEAR] = Pokemon_GetValue(mon->monData, MON_DATA_MET_YEAR, NULL);
    mon->stats[DEBUG_MON_MET_MONTH] = Pokemon_GetValue(mon->monData, MON_DATA_MET_MONTH, NULL);
    mon->stats[DEBUG_MON_MET_DAY] = Pokemon_GetValue(mon->monData, MON_DATA_MET_DAY, NULL);
    mon->stats[DEBUG_MON_HATCH_LOCATION] = Pokemon_GetValue(mon->monData, MON_DATA_HATCH_LOCATION, NULL);
    mon->stats[DEBUG_MON_HATCH_YEAR] = Pokemon_GetValue(mon->monData, MON_DATA_HATCH_YEAR, NULL);
    mon->stats[DEBUG_MON_HATCH_MONTH] = Pokemon_GetValue(mon->monData, MON_DATA_HATCH_MONTH, NULL);
    mon->stats[DEBUG_MON_HATCH_DAY] = Pokemon_GetValue(mon->monData, MON_DATA_HATCH_DAY, NULL);
}

static void DebugMon_SetMonDataFromStats(DebugMon *mon)
{
    Pokemon_SetValue(mon->monData, MON_DATA_SPECIES, &mon->stats[DEBUG_MON_SPECIES]);
    Pokemon_SetValue(mon->monData, MON_DATA_LEVEL, &mon->stats[DEBUG_MON_LEVEL]);
    Pokemon_SetValue(mon->monData, MON_DATA_EXP, &mon->stats[DEBUG_MON_EXP]);
    Pokemon_SetValue(mon->monData, MON_DATA_OT_ID, &mon->stats[DEBUG_MON_OT_ID]);
    Pokemon_SetValue(mon->monData, MON_DATA_PERSONALITY, &mon->stats[DEBUG_MON_PERSONALITY]);
    Pokemon_SetValue(mon->monData, MON_DATA_GENDER, &mon->stats[DEBUG_MON_GENDER]);

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

    Pokemon_SetValue(mon->monData, MON_DATA_HELD_ITEM, &mon->stats[DEBUG_MON_HELD_ITEM]);

    int ability = SpeciesData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], SPECIES_DATA_ABILITY_1 + mon->stats[DEBUG_MON_ABILITY]);
    Pokemon_SetValue(mon->monData, MON_DATA_ABILITY, &ability);

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
    Pokemon_SetValue(mon->monData, MON_DATA_IS_EGG, &mon->stats[DEBUG_MON_IS_EGG]);

    Pokemon_SetValue(mon->monData, MON_DATA_STATUS_CONDITION, &sStatusConditionTable[mon->stats[DEBUG_MON_STATUS_CONDITION]][1]);

    Pokemon_SetValue(mon->monData, MON_DATA_MAX_HP, &mon->stats[DEBUG_MON_MAX_HP]);
    Pokemon_SetValue(mon->monData, MON_DATA_ATK, &mon->stats[DEBUG_MON_ATK]);
    Pokemon_SetValue(mon->monData, MON_DATA_DEF, &mon->stats[DEBUG_MON_DEF]);
    Pokemon_SetValue(mon->monData, MON_DATA_SPEED, &mon->stats[DEBUG_MON_SPEED]);
    Pokemon_SetValue(mon->monData, MON_DATA_SP_ATK, &mon->stats[DEBUG_MON_SP_ATK]);
    Pokemon_SetValue(mon->monData, MON_DATA_SP_DEF, &mon->stats[DEBUG_MON_SP_DEF]);
    Pokemon_SetValue(mon->monData, MON_DATA_FATEFUL_ENCOUNTER, &mon->stats[DEBUG_MON_FATEFUL_ENCOUNTER]);
    Pokemon_SetValue(mon->monData, MON_DATA_HAS_NICKNAME, &mon->stats[DEBUG_MON_HAS_NICKNAME]);
    Pokemon_SetValue(mon->monData, MON_DATA_MET_LEVEL, &mon->stats[DEBUG_MON_MET_LEVEL]);
    Pokemon_SetValue(mon->monData, MON_DATA_MET_GAME, &mon->stats[DEBUG_MON_MET_GAME]);
    Pokemon_SetValue(mon->monData, MON_DATA_POKEBALL, &mon->stats[DEBUG_MON_POKEBALL]);
    Pokemon_SetValue(mon->monData, MON_DATA_MET_GAME_REGION_CODE, &mon->stats[DEBUG_MON_MET_GAME_REGION_CODE]);
    Pokemon_SetValue(mon->monData, MON_DATA_FORM, &mon->stats[DEBUG_MON_FORM]);

    if (mon->stats[DEBUG_MON_IS_EGG]) {
        mon->stats[DEBUG_MON_MET_LOCATION] = 0;
        mon->stats[DEBUG_MON_MET_YEAR] = 0;
        mon->stats[DEBUG_MON_MET_MONTH] = 0;
        mon->stats[DEBUG_MON_MET_DAY] = 0;
        if (mon->stats[MON_DATA_HATCH_LOCATION] == 0) {
            mon->stats[MON_DATA_HATCH_LOCATION] = 1;
        }
    } else {
        Pokemon_SetValue(mon->monData, MON_DATA_MET_LOCATION, &mon->stats[DEBUG_MON_MET_LOCATION]);
        Pokemon_SetValue(mon->monData, MON_DATA_MET_YEAR, &mon->stats[DEBUG_MON_MET_YEAR]);
        Pokemon_SetValue(mon->monData, MON_DATA_MET_MONTH, &mon->stats[DEBUG_MON_MET_MONTH]);
        Pokemon_SetValue(mon->monData, MON_DATA_MET_DAY, &mon->stats[DEBUG_MON_MET_DAY]);
    }

    Pokemon_SetValue(mon->monData, MON_DATA_HATCH_LOCATION, &mon->stats[DEBUG_MON_HATCH_LOCATION]);
    Pokemon_SetValue(mon->monData, MON_DATA_HATCH_YEAR, &mon->stats[DEBUG_MON_HATCH_YEAR]);
    Pokemon_SetValue(mon->monData, MON_DATA_HATCH_MONTH, &mon->stats[DEBUG_MON_HATCH_MONTH]);
    Pokemon_SetValue(mon->monData, MON_DATA_HATCH_DAY, &mon->stats[DEBUG_MON_HATCH_DAY]);

    Pokemon_CalcLevelAndStats(mon->monData);
}

static void DebugMon_SetMoveAtPosition(Pokemon *mon, u16 moveID, u16 movePos)
{
    u8 pp = MoveTable_LoadParam(moveID, MOVEATTRIBUTE_PP);
    Pokemon_SetValue(mon, MON_DATA_MOVE1 + movePos, &moveID);
    Pokemon_SetValue(mon, MON_DATA_MOVE1_CUR_PP + movePos, &pp);
}

static u8 DebugMonValue_Display(DebugMonMenu *monMenu, u8 statID, u32 color, u8 y)
{
    DebugMon *mon = &monMenu->mon;

    if (statID == DEBUG_MON_BATTLE_PARAM1) {
        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_label_hp, 12, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_MAX_HP], 3, 12 + 24, y + 16, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);

        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_label_attack, 12 + 72, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_ATK], 3, 12 + 72 + 24, y + 16, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);

        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_label_defense, 12 + 72 + 72, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_DEF], 3, 12 + 72 + 72 + 24, y + 16, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);
        return 0;
    }

    if (statID == DEBUG_MON_BATTLE_PARAM2) {
        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_label_speed, 12, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_SPEED], 3, 12 + 24, y + 16, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);

        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_label_sp_attack, 12 + 72, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_SP_ATK], 3, 12 + 72 + 24, y + 16, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);

        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_label_sp_defense, 12 + 72 + 72, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_GREEN);
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[DEBUG_MON_SP_DEF], 3, 12 + 72 + 72 + 24, y + 16, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);
        return 0;
    }

    DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, sDebugMonValueList[statID].label, 12, y, TEXT_SPEED_NO_TRANSFER, color);

    u8 count = sDebugMonValueList[statID].value->count;

    switch (statID) {
    case DEBUG_MON_SPECIES:
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[statID], count, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, color);
        DebugMonValue_PrintSpeciesName(&monMenu->mainWindow, mon->stats[statID], 12 + 72 + 24 + 32, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);
        break;

    case DEBUG_MON_GENDER:
        switch (mon->stats[statID]) {
        case GENDER_MALE:
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_symbol_male, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);
            break;
        case GENDER_FEMALE:
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_symbol_female, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);
            break;
        case GENDER_NONE:
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_none, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);
            break;
        }
        break;

    case DEBUG_MON_MOVE1:
    case DEBUG_MON_MOVE2:
    case DEBUG_MON_MOVE3:
    case DEBUG_MON_MOVE4:
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[statID], count, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, color);
        StringTemplate_SetMoveName(monMenu->strTemplate, 0, mon->stats[statID]);
        DebugMonValue_PrintStrExpanded(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, dmm_template_move_name, 12 + 72 + 24 + 32, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    case DEBUG_MON_HELD_ITEM:
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[statID], count, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, color);
        StringTemplate_SetItemName(monMenu->strTemplate, 0, mon->stats[statID]);
        DebugMonValue_PrintStrExpanded(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, dmm_template_held_item, 12 + 72 + 24 + 32, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    case DEBUG_MON_ABILITY:
        StringTemplate_SetAbilityName(monMenu->strTemplate, 0, SpeciesData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], SPECIES_DATA_ABILITY_1 + mon->stats[statID]));
        DebugMonValue_PrintStrExpanded(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, dmm_template_ability, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    case DEBUG_MON_STATUS_CONDITION:
        DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, sStatusConditionTable[mon->stats[statID]][0], 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);
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
        DebugMonValue_PrintNum(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, mon, mon->stats[statID], count, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    case DEBUG_MON_IS_EGG:
    case DEBUG_MON_FATEFUL_ENCOUNTER:
    case DEBUG_MON_HAS_NICKNAME:
        if (mon->stats[statID]) {
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_on, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);
        } else {
            DebugMonValue_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, dmm_off, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, DMM_COLOR_BLACK);
        }
        break;

    case DEBUG_MON_NATURE:
        StringTemplate_SetNatureName(monMenu->strTemplate, 0, mon->stats[statID]);
        DebugMonValue_PrintStrExpanded(&monMenu->mainWindow, monMenu->msgLoader, monMenu->strTemplate, dmm_template_nature, 12 + 72 + 24, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    default:
        break;
    }

    return 0;
}

static void DebugMonValue_PrintStr(Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, entryID);
    Text_AddPrinterWithParamsAndColor(window, 0, buf, x, y, delay, color, NULL);
    Strbuf_Free(buf);
}

static void DebugMonValue_PrintStrExpanded(Window *window, MessageLoader *msgLoader, StringTemplate *strTemplate, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, entryID);
    Strbuf *bufExp = Strbuf_Init(128, HEAP_ID_APPLICATION);

    StringTemplate_Format(strTemplate, bufExp, buf);

    Text_AddPrinterWithParamsAndColor(window, 0, bufExp, x, y, delay, color, NULL);

    Strbuf_Free(buf);
    Strbuf_Free(bufExp);
}

static void DebugMonValue_PrintNum(Window *window, MessageLoader *msgLoader, StringTemplate *strTemplate, DebugMon *mon, u32 num, u32 digits, u32 x, u32 y, u32 delay, u32 color)
{
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, dmm_template_number);
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

        Text_AddPrinterWithParamsAndColor(window, 0, bufExp, x + size, y, TEXT_SPEED_NO_TRANSFER, DebugMonValue_GetColor(mon, i - 1, color), NULL);
        num %= tmp;
        tmp /= 10;
        size += Font_CalcStrbufWidth(0, bufExp, 0);
    }

    if (delay == 0) {
        Window_ScheduleCopyToVRAM(window);
    }

    Strbuf_Free(buf);
    Strbuf_Free(bufExp);
}

static u32 DebugMonValue_GetColor(DebugMon *mon, u8 digit, u32 color)
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

static void DebugMonValue_PrintSpeciesName(Window *window, u32 species, u32 x, u32 y, u32 delay, u32 color)
{
    MessageLoader *msgLoader = MessageLoader_Init(MESSAGE_LOADER_NARC_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_SPECIES_NAME, HEAP_ID_APPLICATION);
    Strbuf *buf = MessageLoader_GetNewStrbuf(msgLoader, species);

    Text_AddPrinterWithParamsAndColor(window, 0, buf, x, y, delay, color, NULL);

    Strbuf_Free(buf);
    MessageLoader_Free(msgLoader);
}

static u32 DebugMonValue_Get(DebugMon *mon, u8 statID)
{
    return mon->stats[statID];
}

static void DebugMonValue_Set(DebugMon *mon, u32 value, u8 statID)
{
    switch (statID) {
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
