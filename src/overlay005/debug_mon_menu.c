#include "overlay005/debug_mon_menu.h"

#include "constants/battle/condition.h"
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

#include "struct_defs/struct_02090800.h"

#include "applications/town_map/main.h"
#include "field/field_system.h"
#include "overlay005/debug_menu.h"
#include "overlay006/hm_cut_in.h"

#include "field_map_change.h"
#include "field_system.h"
#include "message.h"
#include "move_table.h"
#include "party.h"
#include "pc_boxes.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "string_gf.h"
#include "sys_task.h"
#include "system.h"
#include "text.h"
#include "unk_0203D1B8.h"
#include "unk_02092494.h"

#include "res/text/bank/debug_mon_menu.h"

#define DEBUG_MON_MENU_MAX_PAGES 8
#define LINE_SPACING             16
#define DEBUG_MON_PAGE_END       0xFF

static void Task_DebugMonMenu_Main(SysTask *task, void *data);
static void DebugMonMenu_Init(DebugMonMenu *monMenu);
static void DebugMonMenu_HandleInput(DebugMonMenu *monMenu);
static void DebugMonMenu_HandleValueInput(DebugMonMenu *monMenu);
static void DebugMonMenu_DisplayPageAndCursor(DebugMonMenu *monMenu);
static void DebugMonMenu_WaitButtonPress(DebugMonMenu *monMenu, int buttonMask);
static void DebugMonMenu_Free(DebugMonMenu *monMenu);
static void DebugMonMenu_PrintString(Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color);
static void DebugMonMenu_SetTrainerMemo(DebugMonMenu *monMenu, BOOL playerIsOT);
static void DebugMonMenu_DisplayCursor(DebugMonMenu *monMenu, u8 mode);
static void DebugMonMenu_ChangeValue(DebugMonMenu *monMenu, enum DebugMonValueDirection dir);
static void DebugMonMenu_DisplayPage(DebugMonMenu *monMenu);
static void DebugMonMenu_DisplayValues(DebugMonMenu *monMenu);
static BOOL DebugMonMenu_AddMon(DebugMonMenu *monMenu);
static void DebugMonMenu_SetEditMon(DebugMonMenu *monMenu, u8 partySlot);

static void DebugMon_InitStats(DebugMon *mon);
static void DebugMon_CalcInitialStats(DebugMon *mon);
static void DebugMon_CalcFullStats(DebugMonMenu *monMenu, DebugMon *mon);
static void DebugMon_CalcBaseStats(DebugMon *mon);
static void DebugMon_CalcStatsFromExp(DebugMon *mon);
static void DebugMon_SetStatsFromMonData(DebugMon *mon);
static void DebugMon_SetMonDataFromStats(DebugMon *mon);
static void DebugMon_SetMoveAtPosition(Pokemon *mon, u16 moveID, u16 movePos);

static void DebugMonStat_Display(DebugMonMenu *monMenu, u8 statID, u32 color, u8 y);
static void DebugMonStat_PrintStr(Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color);
static void DebugMonStat_PrintStrExpanded(DebugMonMenu *monMenu, Window *window, u32 entryID, u32 x, u32 y, u32 delay, u32 color);
static void DebugMonStat_PrintNum(DebugMonMenu *monMenu, Window *window, u32 num, u32 digits, u32 x, u32 y, u32 delay, u32 color);
static u32 DebugMonStat_GetColor(DebugMonMenu *monMenu, u8 digit, u32 color);
static void DebugMonStat_PrintSpeciesName(Window *window, u32 species, u32 x, u32 y, u32 delay, u32 color);
static u32 DebugMonStat_Get(DebugMon *mon, u8 statID);
static void DebugMonStat_Set(DebugMon *mon, u32 value, u8 statID);

static const DebugMonStatBounds sBoundsSpeciesStats = { 1, SPECIES_EGG - 1, 3 };
static const DebugMonStatBounds sBoundsLevel = { 1, MAX_POKEMON_LEVEL, 3 };
static const DebugMonStatBounds sBoundsExpPoints = { 1, 2000000, 7 };
static const DebugMonStatBounds sBoundsOTID = { 0, 0xFFFFFFFF, 10 };
static const DebugMonStatBounds sBoundsPersonality = { 0, 0xFFFFFFFF, 10 };
static const DebugMonStatBounds sBoundsGender = { 0, 2, 0 };
static const DebugMonStatBounds sBoundsNature = { 0, 24, 0 };
static const DebugMonStatBounds sBoundsMoves = { 0, MAX_MOVES, 3 };
static const DebugMonStatBounds sBoundsHeldItem = { 0, MAX_ITEMS, 3 };
static const DebugMonStatBounds sBoundsAbility = { 0, 1, 0 };
static const DebugMonStatBounds sBoundsIVs = { 0, MAX_IVS_SINGLE_STAT, 2 };
static const DebugMonStatBounds sBoundsEVs = { 0, MAX_EVS_SINGLE_STAT, 3 };
static const DebugMonStatBounds sBoundsContestStats = { 0, 255, 3 };
static const DebugMonStatBounds sBoundsSheen = { 0, 255, 3 };
static const DebugMonStatBounds sBoundsFriendship = { 0, 255, 3 };
static const DebugMonStatBounds sBoundsPokerus = { 0, 255, 3 };
static const DebugMonStatBounds sBoundsIsEgg = { 0, 1, 0 };
static const DebugMonStatBounds sBoundsStatus = { 0, 6, 0 };
static const DebugMonStatBounds sBoundsIsFatefulEncounter = { 0, 1, 0 };
static const DebugMonStatBounds sBoundsLocation = { 0, 9999, 4 };
static const DebugMonStatBounds sBoundsMetLevel = { 0, MAX_POKEMON_LEVEL, 3 };
static const DebugMonStatBounds sBoundsMetGame = { 1, 15, 2 };
static const DebugMonStatBounds sBoundsPokeball = { 1, 16, 2 };
static const DebugMonStatBounds sBoundsMetRegion = { 1, 8, 1 };
static const DebugMonStatBounds sBoundsForm = { 0, 255, 3 };
static const DebugMonStatBounds sBoundsYear = { 0, 99, 2 };
static const DebugMonStatBounds sBoundsMonth = { 1, 12, 2 };
static const DebugMonStatBounds sBoundsDay = { 1, 31, 2 };
static const DebugMonStatBounds sBoundsHasNickname = { 0, 1, 0 };

static const DebugMonStat sDebugMonStats[MAX_DEBUG_MON_STAT] = {
    [DEBUG_MON_SPECIES] = { DebugMonMenu_Text_Species, &sBoundsSpeciesStats },
    [DEBUG_MON_LEVEL] = { DebugMonMenu_Text_Level, &sBoundsLevel },
    [DEBUG_MON_EXP] = { DebugMonMenu_Text_ExpPoints, &sBoundsExpPoints },
    [DEBUG_MON_OT_ID] = { DebugMonMenu_Text_OTID, &sBoundsOTID },
    [DEBUG_MON_PERSONALITY] = { DebugMonMenu_Text_Personality, &sBoundsPersonality },
    [DEBUG_MON_GENDER] = { DebugMonMenu_Text_Gender, &sBoundsGender },
    [DEBUG_MON_NATURE] = { DebugMonMenu_Text_Nature, &sBoundsNature },
    [DEBUG_MON_MOVE1] = { DebugMonMenu_Text_Move1, &sBoundsMoves },
    [DEBUG_MON_MOVE2] = { DebugMonMenu_Text_Move2, &sBoundsMoves },
    [DEBUG_MON_MOVE3] = { DebugMonMenu_Text_Move3, &sBoundsMoves },
    [DEBUG_MON_MOVE4] = { DebugMonMenu_Text_Move4, &sBoundsMoves },
    [DEBUG_MON_HELD_ITEM] = { DebugMonMenu_Text_HeldItem, &sBoundsHeldItem },
    [DEBUG_MON_ABILITY] = { DebugMonMenu_Text_Ability, &sBoundsAbility },
    [DEBUG_MON_HP_IV] = { DebugMonMenu_Text_HP_IV, &sBoundsIVs },
    [DEBUG_MON_HP_EV] = { DebugMonMenu_Text_HP_EV, &sBoundsEVs },
    [DEBUG_MON_ATK_IV] = { DebugMonMenu_Text_Attack_IV, &sBoundsIVs },
    [DEBUG_MON_ATK_EV] = { DebugMonMenu_Text_Attack_EV, &sBoundsEVs },
    [DEBUG_MON_DEF_IV] = { DebugMonMenu_Text_Defense_IV, &sBoundsIVs },
    [DEBUG_MON_DEF_EV] = { DebugMonMenu_Text_Defense_EV, &sBoundsEVs },
    [DEBUG_MON_SPEED_IV] = { DebugMonMenu_Text_Speed_IV, &sBoundsIVs },
    [DEBUG_MON_SPEED_EV] = { DebugMonMenu_Text_Speed_EV, &sBoundsEVs },
    [DEBUG_MON_SPATK_IV] = { DebugMonMenu_Text_SpAtk_IV, &sBoundsIVs },
    [DEBUG_MON_SPATK_EV] = { DebugMonMenu_Text_SpAtk_EV, &sBoundsEVs },
    [DEBUG_MON_SPDEF_IV] = { DebugMonMenu_Text_SpDef_IV, &sBoundsIVs },
    [DEBUG_MON_SPDEF_EV] = { DebugMonMenu_Text_SpDef_EV, &sBoundsEVs },
    [DEBUG_MON_COOL] = { DebugMonMenu_Text_Cool, &sBoundsContestStats },
    [DEBUG_MON_BEAUTY] = { DebugMonMenu_Text_Beauty, &sBoundsContestStats },
    [DEBUG_MON_CUTE] = { DebugMonMenu_Text_Cute, &sBoundsContestStats },
    [DEBUG_MON_SMART] = { DebugMonMenu_Text_Smart, &sBoundsContestStats },
    [DEBUG_MON_TOUGH] = { DebugMonMenu_Text_Tough, &sBoundsContestStats },
    [DEBUG_MON_SHEEN] = { DebugMonMenu_Text_Sheen, &sBoundsSheen },
    [DEBUG_MON_FRIENDSHIP] = { DebugMonMenu_Text_Friendship, &sBoundsFriendship },
    [DEBUG_MON_POKERUS] = { DebugMonMenu_Text_Pokerus, &sBoundsPokerus },
    [DEBUG_MON_IS_EGG] = { DebugMonMenu_Text_Egg, &sBoundsIsEgg },
    [DEBUG_MON_STATUS] = { DebugMonMenu_Text_Status, &sBoundsStatus },
    [DEBUG_MON_MAX_HP] = { DebugMonMenu_Text_HP, &sBoundsSpeciesStats },
    [DEBUG_MON_ATK] = { DebugMonMenu_Text_Attack, &sBoundsSpeciesStats },
    [DEBUG_MON_DEF] = { DebugMonMenu_Text_Defense, &sBoundsSpeciesStats },
    [DEBUG_MON_SPEED] = { DebugMonMenu_Text_Speed, &sBoundsSpeciesStats },
    [DEBUG_MON_SP_ATK] = { DebugMonMenu_Text_SpAtk, &sBoundsSpeciesStats },
    [DEBUG_MON_SP_DEF] = { DebugMonMenu_Text_SpDef, &sBoundsSpeciesStats },
    [DEBUG_MON_FATEFUL_ENCOUNTER] = { DebugMonMenu_Text_FatefulEncounter, &sBoundsIsFatefulEncounter },
    [DEBUG_MON_EGG_LOCATION] = { DebugMonMenu_Text_EggLocation, &sBoundsLocation },
    [DEBUG_MON_MET_LEVEL] = { DebugMonMenu_Text_MetLevel, &sBoundsMetLevel },
    [DEBUG_MON_MET_GAME] = { DebugMonMenu_Text_MetGame, &sBoundsMetGame },
    [DEBUG_MON_POKEBALL] = { DebugMonMenu_Text_Pokeball, &sBoundsPokeball },
    [DEBUG_MON_FORM] = { DebugMonMenu_Text_Form, &sBoundsForm },
    [DEBUG_MON_EGG_YEAR] = { DebugMonMenu_Text_Year, &sBoundsYear },
    [DEBUG_MON_EGG_MONTH] = { DebugMonMenu_Text_Month, &sBoundsMonth },
    [DEBUG_MON_EGG_DAY] = { DebugMonMenu_Text_Day, &sBoundsDay },
    [DEBUG_MON_MET_LOCATION] = { DebugMonMenu_Text_MetLocation, &sBoundsLocation },
    [DEBUG_MON_MET_YEAR] = { DebugMonMenu_Text_Year, &sBoundsYear },
    [DEBUG_MON_MET_MONTH] = { DebugMonMenu_Text_Month, &sBoundsMonth },
    [DEBUG_MON_MET_DAY] = { DebugMonMenu_Text_Day, &sBoundsDay },
    [DEBUG_MON_HAS_NICKNAME] = { DebugMonMenu_Text_Nickname, &sBoundsHasNickname },
};

static const u32 sDebugMonIncrements[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
};

static const u32 sStatusConditionTable[][2] = {
    { DebugMonMenu_Text_Normal, MON_CONDITION_NONE },
    { DebugMonMenu_Text_Sleep, MON_CONDITION_SLEEP_0 },
    { DebugMonMenu_Text_Poison, MON_CONDITION_POISON },
    { DebugMonMenu_Text_Burned, MON_CONDITION_BURN },
    { DebugMonMenu_Text_Frozen, MON_CONDITION_FREEZE },
    { DebugMonMenu_Text_Paralyzed, MON_CONDITION_PARALYSIS },
    { DebugMonMenu_Text_BadlyPoisoned, MON_CONDITION_TOXIC },
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
    DEBUG_MON_STATUS,
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
    DEBUG_MON_BATTLE_STATS_1,
    DEBUG_MON_PAGE_END,
};

static const u8 sDebugMonPage5[] = {
    DEBUG_MON_SPEED_IV,
    DEBUG_MON_SPEED_EV,
    DEBUG_MON_SPATK_IV,
    DEBUG_MON_SPATK_EV,
    DEBUG_MON_SPDEF_IV,
    DEBUG_MON_SPDEF_EV,
    DEBUG_MON_BATTLE_STATS_2,
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
    DEBUG_MON_FORM,
    DEBUG_MON_PAGE_END,
};

static const u8 sDebugMonPage8[] = {
    DEBUG_MON_EGG_LOCATION,
    DEBUG_MON_EGG_YEAR,
    DEBUG_MON_EGG_MONTH,
    DEBUG_MON_EGG_DAY,
    DEBUG_MON_MET_LOCATION,
    DEBUG_MON_MET_YEAR,
    DEBUG_MON_MET_MONTH,
    DEBUG_MON_MET_DAY,
    DEBUG_MON_PAGE_END,
};

static const DebugMonMenuPage sDebugMonMenuPages[DEBUG_MON_MENU_MAX_PAGES] = {
    { sDebugMonPage1, 5 },
    { sDebugMonPage2, 6 },
    { sDebugMonPage3, 6 },
    { sDebugMonPage4, 6 },
    { sDebugMonPage5, 6 },
    { sDebugMonPage6, 6 },
    { sDebugMonPage7, 4 },
    { sDebugMonPage8, 8 }
};

void DebugMonMenu_New(FieldSystem *fieldSystem, enum DebugMonMenuMode mode)
{
    DebugMonMenu *monMenu = (DebugMonMenu *)SysTask_GetParam(SysTask_StartAndAllocateParam(Task_DebugMonMenu_Main, sizeof(DebugMonMenu), 0, HEAP_ID_APPLICATION));
    monMenu->fieldSystem = fieldSystem;
    monMenu->state = DMM_STATE_DRAW_MENU;
    monMenu->mode = mode;
    monMenu->msgLoader = MessageLoader_Init(MSG_LOADER_PRELOAD_ENTIRE_BANK, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_DEBUG_MON_MENU, HEAP_ID_APPLICATION);
    monMenu->strTemplate = StringTemplate_Default(HEAP_ID_APPLICATION);
    monMenu->arrow = ColoredArrow_New(HEAP_ID_APPLICATION);
    monMenu->partySlot = 0;

    BgConfig *bgConfig = FieldSystem_GetBgConfig(fieldSystem);

    Window_Add(bgConfig, &monMenu->titleWindow, BG_LAYER_MAIN_3, 1, 1, 30, 4, 13, 1);
    Window_Add(bgConfig, &monMenu->mainWindow, BG_LAYER_MAIN_3, 1, 7, 30, 16, 13, 121);

    LoadStandardWindowGraphics(bgConfig, BG_LAYER_MAIN_3, 661, 14, STANDARD_WINDOW_SYSTEM, HEAP_ID_APPLICATION);

    Window_DrawStandardFrame(&monMenu->titleWindow, TRUE, 661, 14);
    Window_DrawStandardFrame(&monMenu->mainWindow, TRUE, 661, 14);

    DebugMonMenu_Init(monMenu);
}

static void Task_DebugMonMenu_Main(SysTask *task, void *data)
{
    DebugMonMenu *monMenu = (DebugMonMenu *)data;

    switch (monMenu->state) {
    case DMM_STATE_DRAW_MENU:
        DebugMonMenu_DisplayPageAndCursor(monMenu);
        break;
    case DMM_STATE_HANDLE_INPUT:
        DebugMonMenu_HandleInput(monMenu);
        break;
    case DMM_STATE_HANDLE_VALUE_INPUT:
        DebugMonMenu_HandleValueInput(monMenu);
        break;
    case DMM_STATE_WAIT_A_BUTTON_PRESS:
        DebugMonMenu_WaitButtonPress(monMenu, PAD_BUTTON_A);
        break;
    case DMM_STATE_EXIT_MENU:
        DebugMonMenu_Free(monMenu);
        SysTask_FinishAndFreeParam(task);
        FieldSystem_ResumeProcessing();
        break;
    case DMM_STATE_WAIT_XY_BUTTON_PRESS:
        DebugMonMenu_WaitButtonPress(monMenu, PAD_BUTTON_X | PAD_BUTTON_Y);
        break;
    }
}

static void DebugMonMenu_HandleInput(DebugMonMenu *monMenu)
{
    if (JOY_NEW(PAD_BUTTON_A)) {
        Sound_PlayEffect(SEQ_SE_CONFIRM);
        monMenu->mon.statBackup = monMenu->mon.stats[sDebugMonMenuPages[monMenu->page].page[monMenu->cursor]];
        monMenu->digits = 0;
        DebugMonMenu_DisplayValues(monMenu);
        monMenu->state = DMM_STATE_HANDLE_VALUE_INPUT;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_B)) {
        Sound_PlayEffect(SEQ_SE_CONFIRM);
        monMenu->state = DMM_STATE_EXIT_MENU;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_START)) {
        Sound_PlayEffect(SEQ_SE_CONFIRM);
        if (monMenu->mode != DEBUG_MON_MENU_MODE_EDIT) {
            DebugMon_CalcFullStats(monMenu, &monMenu->mon);
        } else {
            DebugMon_SetMonDataFromStats(&monMenu->mon);
        }

        BOOL monAddedToPC = DebugMonMenu_AddMon(monMenu);

        Window_FillRectWithColor(&monMenu->mainWindow, DEBUG_COLOR_WHITE, 24, 64, 216, 32);

        if (monAddedToPC) {
            DebugMonMenu_PrintString(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_MonAddedToPC, 32, 72, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);
        } else {
            if (monMenu->mode != DEBUG_MON_MENU_MODE_EDIT) {
                DebugMonMenu_PrintString(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_MonAddedToParty, 32, 72, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);
            } else {
                DebugMonMenu_PrintString(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_MonEdited, 32, 72, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);
            }
        }

        monMenu->state = DMM_STATE_WAIT_A_BUTTON_PRESS;
        return;
    }

    if (JOY_NEW(PAD_KEY_UP)) {
        Sound_PlayEffect(SEQ_SE_DP_SELECT78);
        DebugMonMenu_DisplayCursor(monMenu, DMV_DIR_DECREASE);
        return;
    }

    if (JOY_NEW(PAD_KEY_DOWN)) {
        Sound_PlayEffect(SEQ_SE_DP_SELECT78);
        DebugMonMenu_DisplayCursor(monMenu, DMV_DIR_INCREASE);
        return;
    }

    if (JOY_NEW(PAD_BUTTON_L) && monMenu->page > 0) {
        Sound_PlayEffect(SEQ_SE_DP_SELECT5);
        monMenu->page--;
        monMenu->cursor = 0;
        monMenu->state = DMM_STATE_DRAW_MENU;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_R) && monMenu->page < DEBUG_MON_MENU_MAX_PAGES - 1) {
        Sound_PlayEffect(SEQ_SE_DP_SELECT5);
        monMenu->page++;
        monMenu->cursor = 0;
        monMenu->state = DMM_STATE_DRAW_MENU;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_X)) {
        if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
            Sound_PlayEffect(SEQ_SE_DP_SELECT5);
            int partyCount = Party_GetCurrentCount(SaveData_GetParty(monMenu->fieldSystem->saveData));

            if (monMenu->partySlot == 0) {
                monMenu->partySlot = partyCount - 1;
            } else {
                monMenu->partySlot--;
            }

            DebugMonMenu_SetEditMon(monMenu, monMenu->partySlot);
            monMenu->state = DMM_STATE_DRAW_MENU;
        } else {
            Sound_PlayEffect(SEQ_SE_CONFIRM);
            DebugMonMenu_SetTrainerMemo(monMenu, TRUE);
            monMenu->state = DMM_STATE_WAIT_XY_BUTTON_PRESS;
        }
        return;
    }

    if (JOY_NEW(PAD_BUTTON_Y)) {
        if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
            Sound_PlayEffect(SEQ_SE_DP_SELECT5);
            int partyCount = Party_GetCurrentCount(SaveData_GetParty(monMenu->fieldSystem->saveData));

            if (monMenu->partySlot == partyCount - 1) {
                monMenu->partySlot = 0;
            } else {
                monMenu->partySlot++;
            }

            DebugMonMenu_SetEditMon(monMenu, monMenu->partySlot);
            monMenu->state = DMM_STATE_DRAW_MENU;
        } else {
            Sound_PlayEffect(SEQ_SE_CONFIRM);
            DebugMonMenu_SetTrainerMemo(monMenu, TRUE);
            monMenu->state = DMM_STATE_WAIT_XY_BUTTON_PRESS;
        }
    }
}

static void DebugMonMenu_HandleValueInput(DebugMonMenu *monMenu)
{
    u8 statID = sDebugMonMenuPages[monMenu->page].page[monMenu->cursor];

    if (JOY_NEW(PAD_BUTTON_A)) {
        Sound_PlayEffect(SEQ_SE_CONFIRM);
        monMenu->state = DMM_STATE_DRAW_MENU;
        return;
    }

    if (JOY_NEW(PAD_BUTTON_B)) {
        Sound_PlayEffect(SEQ_SE_CONFIRM);
        monMenu->mon.stats[statID] = monMenu->mon.statBackup;
        monMenu->state = DMM_STATE_DRAW_MENU;
        return;
    }

    if (JOY_REPEAT(PAD_KEY_UP)) {
        Sound_PlayEffect(SEQ_SE_DP_SELECT78);
        DebugMonMenu_ChangeValue(monMenu, DMV_DIR_INCREASE);
        DebugMonMenu_DisplayValues(monMenu);
        return;
    }

    if (JOY_REPEAT(PAD_KEY_DOWN)) {
        Sound_PlayEffect(SEQ_SE_DP_SELECT78);
        DebugMonMenu_ChangeValue(monMenu, DMV_DIR_DECREASE);
        DebugMonMenu_DisplayValues(monMenu);
        return;
    }

    if (sDebugMonStats[statID].bounds->digits != 0) {
        if (JOY_NEW(PAD_KEY_LEFT) && monMenu->digits < sDebugMonStats[statID].bounds->digits - 1) {
            Sound_PlayEffect(SEQ_SE_DP_SELECT78);
            monMenu->digits++;
            DebugMonMenu_DisplayValues(monMenu);
            return;
        }
        if (JOY_NEW(PAD_KEY_RIGHT) && monMenu->digits > 0) {
            Sound_PlayEffect(SEQ_SE_DP_SELECT78);
            monMenu->digits--;
            DebugMonMenu_DisplayValues(monMenu);
        }
    }
}

static void DebugMonMenu_WaitButtonPress(DebugMonMenu *monMenu, int buttonMask)
{
    if (JOY_NEW(buttonMask)) {
        monMenu->state = DMM_STATE_DRAW_MENU;
    }
}

static void DebugMonMenu_Init(DebugMonMenu *monMenu)
{
    Window_FillTilemap(&monMenu->titleWindow, DEBUG_COLOR_WHITE);
    if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
        DebugMonMenu_PrintString(&monMenu->titleWindow, monMenu->msgLoader, DebugMonMenu_Text_EditInstructions, 0, 0, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);
    } else {
        DebugMonMenu_PrintString(&monMenu->titleWindow, monMenu->msgLoader, DebugMonMenu_Text_CreateInstructions, 0, 0, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);
    }

    monMenu->mon.data = Pokemon_New(HEAP_ID_APPLICATION);

    if (monMenu->mode == DEBUG_MON_MENU_MODE_CREATE) {
        DebugMon_InitStats(&monMenu->mon);
    } else if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
        DebugMonMenu_SetEditMon(monMenu, monMenu->partySlot);
    }

    monMenu->page = 0;
    monMenu->cursor = 0;
}

static void DebugMonMenu_Free(DebugMonMenu *monMenu)
{
    Heap_Free(monMenu->mon.data);

    Window_EraseStandardFrame(&monMenu->titleWindow, TRUE);
    Window_EraseStandardFrame(&monMenu->mainWindow, FALSE);

    Window_ClearAndCopyToVRAM(&monMenu->titleWindow);
    Window_ClearAndCopyToVRAM(&monMenu->mainWindow);

    Window_Remove(&monMenu->titleWindow);
    Window_Remove(&monMenu->mainWindow);

    MessageLoader_Free(monMenu->msgLoader);
    StringTemplate_Free(monMenu->strTemplate);
    ColoredArrow_Free(monMenu->arrow);
}

static void DebugMonMenu_PrintString(Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    String *buf = MessageLoader_GetNewString(msgLoader, entryID);
    Text_AddPrinterWithParamsAndColor(window, 0, buf, x, y, delay, color, NULL);
    String_Free(buf);
}

#define DISPLAY_INFO_PRINT(info)                                                                                                                                                \
    {                                                                                                                                                                           \
        if (info.unk_04 != NULL) {                                                                                                                                              \
            Text_AddPrinterWithParamsAndColor(&monMenu->mainWindow, FONT_SYSTEM, info.unk_04, 8, (info.unk_00 - 1) * LINE_SPACING, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK, NULL); \
        }                                                                                                                                                                       \
    }

static void DebugMonMenu_SetTrainerMemo(DebugMonMenu *monMenu, BOOL playerIsOT)
{
    if (monMenu->mode != DEBUG_MON_MENU_MODE_EDIT) {
        DebugMon_CalcFullStats(monMenu, &monMenu->mon);
    } else {
        DebugMon_SetMonDataFromStats(&monMenu->mon);
    }

    Window_FillTilemap(&monMenu->mainWindow, DEBUG_COLOR_WHITE);

    // get display info?
    PokemonInfoDisplayStruct *infoDisplay = sub_02092494(monMenu->mon.data, playerIsOT, HEAP_ID_APPLICATION);

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
    case DMV_DIR_NONE:
        ColoredArrow_Print(monMenu->arrow, &monMenu->mainWindow, 0, monMenu->cursor * LINE_SPACING);
        Window_ScheduleCopyToVRAM(&monMenu->mainWindow);
        break;

    case DMV_DIR_INCREASE:
        if (monMenu->cursor < sDebugMonMenuPages[monMenu->page].count - 1) {
            Window_FillRectWithColor(&monMenu->mainWindow, DEBUG_COLOR_WHITE, 0, monMenu->cursor * LINE_SPACING, 12, 16);
            monMenu->cursor++;
            ColoredArrow_Print(monMenu->arrow, &monMenu->mainWindow, 0, monMenu->cursor * LINE_SPACING);
            Window_ScheduleCopyToVRAM(&monMenu->mainWindow);
        }
        break;

    case DMV_DIR_DECREASE:
        if (monMenu->cursor > 0) {
            Window_FillRectWithColor(&monMenu->mainWindow, DEBUG_COLOR_WHITE, 0, monMenu->cursor * LINE_SPACING, 12, 16);
            monMenu->cursor--;
            ColoredArrow_Print(monMenu->arrow, &monMenu->mainWindow, 0, monMenu->cursor * LINE_SPACING);
            Window_ScheduleCopyToVRAM(&monMenu->mainWindow);
        }
        break;
    }
}

static void DebugMonMenu_ChangeValue(DebugMonMenu *monMenu, enum DebugMonValueDirection dir)
{
    u32 currentValue = DebugMonStat_Get(&monMenu->mon, sDebugMonMenuPages[monMenu->page].page[monMenu->cursor]);
    u32 minValue = sDebugMonStats[sDebugMonMenuPages[monMenu->page].page[monMenu->cursor]].bounds->min;
    u32 maxValue = sDebugMonStats[sDebugMonMenuPages[monMenu->page].page[monMenu->cursor]].bounds->max;
    u32 valueChange = sDebugMonIncrements[monMenu->digits];

    switch (dir) {
    case DMV_DIR_INCREASE:
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

    case DMV_DIR_DECREASE:
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

    DebugMonStat_Set(&monMenu->mon, currentValue, sDebugMonMenuPages[monMenu->page].page[monMenu->cursor]);
}

static void DebugMonMenu_DisplayPageAndCursor(DebugMonMenu *monMenu)
{
    DebugMonMenu_DisplayPage(monMenu);
    DebugMonMenu_DisplayCursor(monMenu, DMV_DIR_NONE);
    monMenu->state = DMM_STATE_HANDLE_INPUT;
}

static void DebugMonMenu_DisplayPage(DebugMonMenu *monMenu)
{
    DebugMon *mon = &monMenu->mon;
    Window_FillTilemap(&monMenu->mainWindow, DEBUG_COLOR_WHITE);

    u32 i = 0;
    while (sDebugMonMenuPages[monMenu->page].page[i] != DEBUG_MON_PAGE_END) {
        DebugMonStat_Display(monMenu, sDebugMonMenuPages[monMenu->page].page[i], DEBUG_TEXT_BLACK, i * LINE_SPACING);
        i++;
    }

    Window_LoadTiles(&monMenu->mainWindow);
}

static void DebugMonMenu_DisplayValues(DebugMonMenu *monMenu)
{
    DebugMon *mon = &monMenu->mon;
    Window_FillTilemap(&monMenu->mainWindow, DEBUG_COLOR_WHITE);

    u32 i = 0;
    while (sDebugMonMenuPages[monMenu->page].page[i] != DEBUG_MON_PAGE_END) {
        if (i == monMenu->cursor) {
            DebugMonStat_Display(monMenu, sDebugMonMenuPages[monMenu->page].page[i], DEBUG_TEXT_RED, i * LINE_SPACING);
        } else {
            DebugMonStat_Display(monMenu, sDebugMonMenuPages[monMenu->page].page[i], DEBUG_TEXT_BLACK, i * LINE_SPACING);
        }

        i++;
    }

    Window_LoadTiles(&monMenu->mainWindow);
}

// return value == if mon added to PC
static BOOL DebugMonMenu_AddMon(DebugMonMenu *monMenu)
{
    Party *party;

    if (monMenu->mode == DEBUG_MON_MENU_MODE_CREATE) {
        party = SaveData_GetParty(monMenu->fieldSystem->saveData);

        if (Party_AddPokemon(party, monMenu->mon.data)) {
            return FALSE;
        }

        PCBoxes_TryStoreBoxMon(SaveData_GetPCBoxes(monMenu->fieldSystem->saveData), Pokemon_GetBoxPokemon(monMenu->mon.data));

        return TRUE;
    } else if (monMenu->mode == DEBUG_MON_MENU_MODE_EDIT) {
        party = SaveData_GetParty(monMenu->fieldSystem->saveData);

        String *buf;

        if (monMenu->mon.stats[DEBUG_MON_IS_EGG]) {
            buf = MessageLoader_GetNewString(monMenu->msgLoader, DebugMonMenu_Text_DebugEggName);
            Pokemon_SetValue(monMenu->mon.data, MON_DATA_NICKNAME_STRING, buf);
            String_Free(buf);
        }

        Pokemon *mon = Party_GetPokemonBySlotIndex(party, monMenu->partySlot);

        buf = String_Init(16, HEAP_ID_APPLICATION);
        Pokemon_GetValue(mon, MON_DATA_OT_NAME_STRING, buf);
        Pokemon_SetValue(monMenu->mon.data, MON_DATA_OT_NAME_STRING, buf);
        String_Free(buf);

        u8 gender = Pokemon_GetValue(mon, MON_DATA_OT_GENDER, NULL);
        Pokemon_SetValue(monMenu->mon.data, MON_DATA_OT_GENDER, &gender);
        Party_AddPokemonBySlotIndex(party, monMenu->partySlot, monMenu->mon.data);
        return FALSE;
    }

    return FALSE;
}

static void DebugMonMenu_SetEditMon(DebugMonMenu *monMenu, u8 partySlot)
{
    Party *party = SaveData_GetParty(monMenu->fieldSystem->saveData);
    Pokemon *changeMon = Party_GetPokemonBySlotIndex(party, partySlot);
    MI_CpuCopy8(changeMon, monMenu->mon.data, Pokemon_GetStructSize());
    DebugMon_SetStatsFromMonData(&monMenu->mon);
}

static void DebugMon_InitStats(DebugMon *mon)
{
    for (u32 i = 0; i < MAX_DEBUG_MON_STAT; i++) {
        mon->stats[i] = 0;
    }

    mon->stats[DEBUG_MON_SPECIES] = SPECIES_BULBASAUR;
    mon->stats[DEBUG_MON_PERSONALITY] = (LCRNG_Next() << 16) | LCRNG_Next();
    mon->stats[DEBUG_MON_LEVEL] = 1;
    mon->stats[DEBUG_MON_EXP] = 1;
    mon->stats[DEBUG_MON_MET_LEVEL] = 1;
    mon->stats[DEBUG_MON_MET_GAME] = GAME_VERSION;
    mon->stats[DEBUG_MON_POKEBALL] = ITEM_POKE_BALL;
    mon->stats[DEBUG_MON_FORM] = 0;
    mon->stats[DEBUG_MON_EGG_YEAR] = 0;
    mon->stats[DEBUG_MON_EGG_MONTH] = 1;
    mon->stats[DEBUG_MON_EGG_DAY] = 1;
    mon->stats[DEBUG_MON_MET_YEAR] = 0;
    mon->stats[DEBUG_MON_MET_MONTH] = 1;
    mon->stats[DEBUG_MON_MET_DAY] = 1;

    DebugMon_CalcInitialStats(mon);
}

static inline u32 CombineIVs(DebugMon *mon)
{
    return ((mon->stats[DEBUG_MON_HP_IV] & 0x1F) << 0)
        | ((mon->stats[DEBUG_MON_ATK_IV] & 0x1F) << 5)
        | ((mon->stats[DEBUG_MON_DEF_IV] & 0x1F) << 10)
        | ((mon->stats[DEBUG_MON_SPEED_IV] & 0x1F) << 15)
        | ((mon->stats[DEBUG_MON_SPATK_IV] & 0x1F) << 20)
        | ((mon->stats[DEBUG_MON_SPDEF_IV] & 0x1F) << 25);
}

static void DebugMon_CalcInitialStats(DebugMon *mon)
{
    u32 monIVs = CombineIVs(mon);

    Pokemon_InitWith(mon->data, mon->stats[DEBUG_MON_SPECIES], mon->stats[DEBUG_MON_LEVEL], monIVs, TRUE, mon->stats[DEBUG_MON_PERSONALITY], 1, mon->stats[DEBUG_MON_OT_ID]);

    mon->stats[DEBUG_MON_GENDER] = Pokemon_GetGender(mon->data);
    mon->stats[DEBUG_MON_NATURE] = Pokemon_GetNature(mon->data);
    mon->stats[DEBUG_MON_MOVE1] = Pokemon_GetValue(mon->data, MON_DATA_MOVE1, NULL);
    mon->stats[DEBUG_MON_MOVE2] = Pokemon_GetValue(mon->data, MON_DATA_MOVE2, NULL);
    mon->stats[DEBUG_MON_MOVE3] = Pokemon_GetValue(mon->data, MON_DATA_MOVE3, NULL);
    mon->stats[DEBUG_MON_MOVE4] = Pokemon_GetValue(mon->data, MON_DATA_MOVE4, NULL);
    mon->stats[DEBUG_MON_EXP] = Pokemon_GetValue(mon->data, MON_DATA_EXPERIENCE, NULL);
    mon->stats[DEBUG_MON_MAX_HP] = Pokemon_GetValue(mon->data, MON_DATA_MAX_HP, NULL);
    mon->stats[DEBUG_MON_ATK] = Pokemon_GetValue(mon->data, MON_DATA_ATK, NULL);
    mon->stats[DEBUG_MON_DEF] = Pokemon_GetValue(mon->data, MON_DATA_DEF, NULL);
    mon->stats[DEBUG_MON_SPEED] = Pokemon_GetValue(mon->data, MON_DATA_SPEED, NULL);
    mon->stats[DEBUG_MON_SP_ATK] = Pokemon_GetValue(mon->data, MON_DATA_SP_ATK, NULL);
    mon->stats[DEBUG_MON_SP_DEF] = Pokemon_GetValue(mon->data, MON_DATA_SP_DEF, NULL);
}

static void DebugMon_CalcFullStats(DebugMonMenu *monMenu, DebugMon *mon)
{
    u32 monIVs = CombineIVs(mon);

    Pokemon_InitWith(mon->data, mon->stats[DEBUG_MON_SPECIES], mon->stats[DEBUG_MON_LEVEL], monIVs, TRUE, mon->stats[DEBUG_MON_PERSONALITY], 1, mon->stats[DEBUG_MON_OT_ID]);

    if (mon->stats[DEBUG_MON_MOVE1]) {
        DebugMon_SetMoveAtPosition(mon->data, mon->stats[DEBUG_MON_MOVE1], 0);
    }

    if (mon->stats[DEBUG_MON_MOVE2]) {
        DebugMon_SetMoveAtPosition(mon->data, mon->stats[DEBUG_MON_MOVE2], 1);
    }

    if (mon->stats[DEBUG_MON_MOVE3]) {
        DebugMon_SetMoveAtPosition(mon->data, mon->stats[DEBUG_MON_MOVE3], 2);
    }

    if (mon->stats[DEBUG_MON_MOVE4]) {
        DebugMon_SetMoveAtPosition(mon->data, mon->stats[DEBUG_MON_MOVE4], 3);
    }

    Pokemon_SetValue(mon->data, MON_DATA_EXPERIENCE, &mon->stats[DEBUG_MON_EXP]);
    Pokemon_SetValue(mon->data, MON_DATA_HP_IV, &mon->stats[DEBUG_MON_HP_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_HP_EV, &mon->stats[DEBUG_MON_HP_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_ATK_IV, &mon->stats[DEBUG_MON_ATK_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_ATK_EV, &mon->stats[DEBUG_MON_ATK_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_DEF_IV, &mon->stats[DEBUG_MON_DEF_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_DEF_EV, &mon->stats[DEBUG_MON_DEF_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPEED_IV, &mon->stats[DEBUG_MON_SPEED_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPEED_EV, &mon->stats[DEBUG_MON_SPEED_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPATK_IV, &mon->stats[DEBUG_MON_SPATK_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPATK_EV, &mon->stats[DEBUG_MON_SPATK_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPDEF_IV, &mon->stats[DEBUG_MON_SPDEF_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPDEF_EV, &mon->stats[DEBUG_MON_SPDEF_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_COOL, &mon->stats[DEBUG_MON_COOL]);
    Pokemon_SetValue(mon->data, MON_DATA_BEAUTY, &mon->stats[DEBUG_MON_BEAUTY]);
    Pokemon_SetValue(mon->data, MON_DATA_CUTE, &mon->stats[DEBUG_MON_CUTE]);
    Pokemon_SetValue(mon->data, MON_DATA_SMART, &mon->stats[DEBUG_MON_SMART]);
    Pokemon_SetValue(mon->data, MON_DATA_TOUGH, &mon->stats[DEBUG_MON_TOUGH]);
    Pokemon_SetValue(mon->data, MON_DATA_SHEEN, &mon->stats[DEBUG_MON_SHEEN]);
    Pokemon_SetValue(mon->data, MON_DATA_FRIENDSHIP, &mon->stats[DEBUG_MON_FRIENDSHIP]);
    Pokemon_SetValue(mon->data, MON_DATA_POKERUS, &mon->stats[DEBUG_MON_POKERUS]);
    Pokemon_SetValue(mon->data, MON_DATA_HELD_ITEM, &mon->stats[DEBUG_MON_HELD_ITEM]);

    u32 ability = SpeciesData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], SPECIES_DATA_ABILITY_1 + mon->stats[DEBUG_MON_ABILITY]);
    Pokemon_SetValue(mon->data, MON_DATA_ABILITY, &ability);

    Pokemon_SetValue(mon->data, MON_DATA_FATEFUL_ENCOUNTER, &mon->stats[DEBUG_MON_FATEFUL_ENCOUNTER]);
    Pokemon_SetValue(mon->data, MON_DATA_HAS_NICKNAME, &mon->stats[DEBUG_MON_HAS_NICKNAME]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_LEVEL, &mon->stats[DEBUG_MON_MET_LEVEL]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_GAME, &mon->stats[DEBUG_MON_MET_GAME]);
    Pokemon_SetValue(mon->data, MON_DATA_POKEBALL, &mon->stats[DEBUG_MON_POKEBALL]);
    Pokemon_SetValue(mon->data, MON_DATA_FORM, &mon->stats[DEBUG_MON_FORM]);
    Pokemon_SetValue(mon->data, MON_DATA_STATUS, &sStatusConditionTable[mon->stats[DEBUG_MON_STATUS]][1]);

    if (mon->stats[DEBUG_MON_IS_EGG]) {
        mon->stats[DEBUG_MON_EGG_LOCATION] = 0;
        mon->stats[DEBUG_MON_EGG_YEAR] = 0;
        mon->stats[DEBUG_MON_EGG_MONTH] = 0;
        mon->stats[DEBUG_MON_EGG_DAY] = 0;

        if (mon->stats[DEBUG_MON_MET_LOCATION] == 0) {
            mon->stats[DEBUG_MON_MET_LOCATION] = 1;
        }
    }

    Pokemon_SetValue(mon->data, MON_DATA_EGG_LOCATION, &mon->stats[DEBUG_MON_EGG_LOCATION]);
    Pokemon_SetValue(mon->data, MON_DATA_EGG_YEAR, &mon->stats[DEBUG_MON_EGG_YEAR]);
    Pokemon_SetValue(mon->data, MON_DATA_EGG_MONTH, &mon->stats[DEBUG_MON_EGG_MONTH]);
    Pokemon_SetValue(mon->data, MON_DATA_EGG_DAY, &mon->stats[DEBUG_MON_EGG_DAY]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_LOCATION, &mon->stats[DEBUG_MON_MET_LOCATION]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_YEAR, &mon->stats[DEBUG_MON_MET_YEAR]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_MONTH, &mon->stats[DEBUG_MON_MET_MONTH]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_DAY, &mon->stats[DEBUG_MON_MET_DAY]);
    Pokemon_SetValue(mon->data, MON_DATA_IS_EGG, &mon->stats[DEBUG_MON_IS_EGG]);

    String *buf;

    if (mon->stats[DEBUG_MON_IS_EGG]) {
        buf = MessageLoader_GetNewString(monMenu->msgLoader, DebugMonMenu_Text_DebugEggName);
        Pokemon_SetValue(mon->data, MON_DATA_NICKNAME_STRING, buf);
        String_Free(buf);
    }

    buf = MessageLoader_GetNewString(monMenu->msgLoader, DebugMonMenu_Text_DebugOT);
    Pokemon_SetValue(mon->data, MON_DATA_OT_NAME_STRING, buf);
    String_Free(buf);

    Pokemon_CalcLevelAndStats(mon->data);
}

static void DebugMon_CalcBaseStats(DebugMon *mon)
{
    Pokemon_SetValue(mon->data, MON_DATA_HP_IV, &mon->stats[DEBUG_MON_HP_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_HP_EV, &mon->stats[DEBUG_MON_HP_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_ATK_IV, &mon->stats[DEBUG_MON_ATK_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_ATK_EV, &mon->stats[DEBUG_MON_ATK_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_DEF_IV, &mon->stats[DEBUG_MON_DEF_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_DEF_EV, &mon->stats[DEBUG_MON_DEF_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPEED_IV, &mon->stats[DEBUG_MON_SPEED_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPEED_EV, &mon->stats[DEBUG_MON_SPEED_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPATK_IV, &mon->stats[DEBUG_MON_SPATK_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPATK_EV, &mon->stats[DEBUG_MON_SPATK_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPDEF_IV, &mon->stats[DEBUG_MON_SPDEF_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPDEF_EV, &mon->stats[DEBUG_MON_SPDEF_EV]);

    Pokemon_CalcLevelAndStats(mon->data);

    mon->stats[DEBUG_MON_MAX_HP] = Pokemon_GetValue(mon->data, MON_DATA_MAX_HP, NULL);
    mon->stats[DEBUG_MON_ATK] = Pokemon_GetValue(mon->data, MON_DATA_ATK, NULL);
    mon->stats[DEBUG_MON_DEF] = Pokemon_GetValue(mon->data, MON_DATA_DEF, NULL);
    mon->stats[DEBUG_MON_SPEED] = Pokemon_GetValue(mon->data, MON_DATA_SPEED, NULL);
    mon->stats[DEBUG_MON_SP_ATK] = Pokemon_GetValue(mon->data, MON_DATA_SP_ATK, NULL);
    mon->stats[DEBUG_MON_SP_DEF] = Pokemon_GetValue(mon->data, MON_DATA_SP_DEF, NULL);
}

static void DebugMon_CalcStatsFromExp(DebugMon *mon)
{
    u32 nullData = 0;
    Pokemon_SetValue(mon->data, MON_DATA_EXPERIENCE, &mon->stats[DEBUG_MON_EXP]);
    Pokemon_SetValue(mon->data, MON_DATA_MOVE1, &nullData);
    Pokemon_SetValue(mon->data, MON_DATA_MOVE2, &nullData);
    Pokemon_SetValue(mon->data, MON_DATA_MOVE3, &nullData);
    Pokemon_SetValue(mon->data, MON_DATA_MOVE4, &nullData);

    Pokemon_CalcLevelAndStats(mon->data);
    Pokemon_SetDefaultMoves(mon->data);

    mon->stats[DEBUG_MON_LEVEL] = Pokemon_GetValue(mon->data, MON_DATA_LEVEL, NULL);
    mon->stats[DEBUG_MON_MOVE1] = Pokemon_GetValue(mon->data, MON_DATA_MOVE1, NULL);
    mon->stats[DEBUG_MON_MOVE2] = Pokemon_GetValue(mon->data, MON_DATA_MOVE2, NULL);
    mon->stats[DEBUG_MON_MOVE3] = Pokemon_GetValue(mon->data, MON_DATA_MOVE3, NULL);
    mon->stats[DEBUG_MON_MOVE4] = Pokemon_GetValue(mon->data, MON_DATA_MOVE4, NULL);

    mon->stats[DEBUG_MON_MAX_HP] = Pokemon_GetValue(mon->data, MON_DATA_MAX_HP, NULL);
    mon->stats[DEBUG_MON_ATK] = Pokemon_GetValue(mon->data, MON_DATA_ATK, NULL);
    mon->stats[DEBUG_MON_DEF] = Pokemon_GetValue(mon->data, MON_DATA_DEF, NULL);
    mon->stats[DEBUG_MON_SPEED] = Pokemon_GetValue(mon->data, MON_DATA_SPEED, NULL);
    mon->stats[DEBUG_MON_SP_ATK] = Pokemon_GetValue(mon->data, MON_DATA_SP_ATK, NULL);
    mon->stats[DEBUG_MON_SP_DEF] = Pokemon_GetValue(mon->data, MON_DATA_SP_DEF, NULL);
}

static void DebugMon_SetStatsFromMonData(DebugMon *mon)
{
    mon->stats[DEBUG_MON_SPECIES] = Pokemon_GetValue(mon->data, MON_DATA_SPECIES, NULL);
    mon->stats[DEBUG_MON_LEVEL] = Pokemon_GetValue(mon->data, MON_DATA_LEVEL, NULL);
    mon->stats[DEBUG_MON_EXP] = Pokemon_GetValue(mon->data, MON_DATA_EXPERIENCE, NULL);
    mon->stats[DEBUG_MON_OT_ID] = Pokemon_GetValue(mon->data, MON_DATA_OT_ID, NULL);
    mon->stats[DEBUG_MON_PERSONALITY] = Pokemon_GetValue(mon->data, MON_DATA_PERSONALITY, NULL);
    mon->stats[DEBUG_MON_GENDER] = Pokemon_GetValue(mon->data, MON_DATA_GENDER, NULL);
    mon->stats[DEBUG_MON_NATURE] = Pokemon_GetNature(mon->data);
    mon->stats[DEBUG_MON_MOVE1] = Pokemon_GetValue(mon->data, MON_DATA_MOVE1, NULL);
    mon->stats[DEBUG_MON_MOVE2] = Pokemon_GetValue(mon->data, MON_DATA_MOVE2, NULL);
    mon->stats[DEBUG_MON_MOVE3] = Pokemon_GetValue(mon->data, MON_DATA_MOVE3, NULL);
    mon->stats[DEBUG_MON_MOVE4] = Pokemon_GetValue(mon->data, MON_DATA_MOVE4, NULL);
    mon->stats[DEBUG_MON_HELD_ITEM] = Pokemon_GetValue(mon->data, MON_DATA_HELD_ITEM, NULL);
    mon->stats[DEBUG_MON_ABILITY] = Pokemon_GetValue(mon->data, MON_DATA_ABILITY, NULL);

    if (mon->stats[DEBUG_MON_ABILITY] == SpeciesData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], SPECIES_DATA_ABILITY_1)) {
        mon->stats[DEBUG_MON_ABILITY] = 0;
    } else {
        mon->stats[DEBUG_MON_ABILITY] = 1;
    }

    mon->stats[DEBUG_MON_HP_IV] = Pokemon_GetValue(mon->data, MON_DATA_HP_IV, NULL);
    mon->stats[DEBUG_MON_HP_EV] = Pokemon_GetValue(mon->data, MON_DATA_HP_EV, NULL);
    mon->stats[DEBUG_MON_ATK_IV] = Pokemon_GetValue(mon->data, MON_DATA_ATK_IV, NULL);
    mon->stats[DEBUG_MON_ATK_EV] = Pokemon_GetValue(mon->data, MON_DATA_ATK_EV, NULL);
    mon->stats[DEBUG_MON_DEF_IV] = Pokemon_GetValue(mon->data, MON_DATA_DEF_IV, NULL);
    mon->stats[DEBUG_MON_DEF_EV] = Pokemon_GetValue(mon->data, MON_DATA_DEF_EV, NULL);
    mon->stats[DEBUG_MON_SPEED_IV] = Pokemon_GetValue(mon->data, MON_DATA_SPEED_IV, NULL);
    mon->stats[DEBUG_MON_SPEED_EV] = Pokemon_GetValue(mon->data, MON_DATA_SPEED_EV, NULL);
    mon->stats[DEBUG_MON_SPATK_IV] = Pokemon_GetValue(mon->data, MON_DATA_SPATK_IV, NULL);
    mon->stats[DEBUG_MON_SPATK_EV] = Pokemon_GetValue(mon->data, MON_DATA_SPATK_EV, NULL);
    mon->stats[DEBUG_MON_SPDEF_IV] = Pokemon_GetValue(mon->data, MON_DATA_SPDEF_IV, NULL);
    mon->stats[DEBUG_MON_SPDEF_EV] = Pokemon_GetValue(mon->data, MON_DATA_SPDEF_EV, NULL);
    mon->stats[DEBUG_MON_COOL] = Pokemon_GetValue(mon->data, MON_DATA_COOL, NULL);
    mon->stats[DEBUG_MON_BEAUTY] = Pokemon_GetValue(mon->data, MON_DATA_BEAUTY, NULL);
    mon->stats[DEBUG_MON_CUTE] = Pokemon_GetValue(mon->data, MON_DATA_CUTE, NULL);
    mon->stats[DEBUG_MON_SMART] = Pokemon_GetValue(mon->data, MON_DATA_SMART, NULL);
    mon->stats[DEBUG_MON_TOUGH] = Pokemon_GetValue(mon->data, MON_DATA_TOUGH, NULL);
    mon->stats[DEBUG_MON_SHEEN] = Pokemon_GetValue(mon->data, MON_DATA_SHEEN, NULL);
    mon->stats[DEBUG_MON_FRIENDSHIP] = Pokemon_GetValue(mon->data, MON_DATA_FRIENDSHIP, NULL);
    mon->stats[DEBUG_MON_POKERUS] = Pokemon_GetValue(mon->data, MON_DATA_POKERUS, NULL);
    mon->stats[DEBUG_MON_IS_EGG] = Pokemon_GetValue(mon->data, MON_DATA_IS_EGG, NULL);

    u32 statusCondition = Pokemon_GetValue(mon->data, MON_DATA_STATUS, NULL);
    mon->stats[DEBUG_MON_STATUS] = MON_CONDITION_NONE;
    for (u32 i = 0; i < 7; i++) {
        if (statusCondition == sStatusConditionTable[i][1]) {
            mon->stats[DEBUG_MON_STATUS] = i;
            break;
        }
    }

    mon->stats[DEBUG_MON_MAX_HP] = Pokemon_GetValue(mon->data, MON_DATA_MAX_HP, NULL);
    mon->stats[DEBUG_MON_ATK] = Pokemon_GetValue(mon->data, MON_DATA_ATK, NULL);
    mon->stats[DEBUG_MON_DEF] = Pokemon_GetValue(mon->data, MON_DATA_DEF, NULL);
    mon->stats[DEBUG_MON_SPEED] = Pokemon_GetValue(mon->data, MON_DATA_SPEED, NULL);
    mon->stats[DEBUG_MON_SP_ATK] = Pokemon_GetValue(mon->data, MON_DATA_SP_ATK, NULL);
    mon->stats[DEBUG_MON_SP_DEF] = Pokemon_GetValue(mon->data, MON_DATA_SP_DEF, NULL);
    mon->stats[DEBUG_MON_FATEFUL_ENCOUNTER] = Pokemon_GetValue(mon->data, MON_DATA_FATEFUL_ENCOUNTER, NULL);
    mon->stats[DEBUG_MON_HAS_NICKNAME] = Pokemon_GetValue(mon->data, MON_DATA_HAS_NICKNAME, NULL);
    mon->stats[DEBUG_MON_MET_LEVEL] = Pokemon_GetValue(mon->data, MON_DATA_MET_LEVEL, NULL);
    mon->stats[DEBUG_MON_MET_GAME] = Pokemon_GetValue(mon->data, MON_DATA_MET_GAME, NULL);
    mon->stats[DEBUG_MON_POKEBALL] = Pokemon_GetValue(mon->data, MON_DATA_POKEBALL, NULL);
    mon->stats[DEBUG_MON_FORM] = Pokemon_GetValue(mon->data, MON_DATA_FORM, NULL);
    mon->stats[DEBUG_MON_EGG_LOCATION] = Pokemon_GetValue(mon->data, MON_DATA_EGG_LOCATION, NULL);
    mon->stats[DEBUG_MON_EGG_YEAR] = Pokemon_GetValue(mon->data, MON_DATA_EGG_YEAR, NULL);
    mon->stats[DEBUG_MON_EGG_MONTH] = Pokemon_GetValue(mon->data, MON_DATA_EGG_MONTH, NULL);
    mon->stats[DEBUG_MON_EGG_DAY] = Pokemon_GetValue(mon->data, MON_DATA_EGG_DAY, NULL);
    mon->stats[DEBUG_MON_MET_LOCATION] = Pokemon_GetValue(mon->data, MON_DATA_MET_LOCATION, NULL);
    mon->stats[DEBUG_MON_MET_YEAR] = Pokemon_GetValue(mon->data, MON_DATA_MET_YEAR, NULL);
    mon->stats[DEBUG_MON_MET_MONTH] = Pokemon_GetValue(mon->data, MON_DATA_MET_MONTH, NULL);
    mon->stats[DEBUG_MON_MET_DAY] = Pokemon_GetValue(mon->data, MON_DATA_MET_DAY, NULL);
}

static void DebugMon_SetMonDataFromStats(DebugMon *mon)
{
    Pokemon_SetValue(mon->data, MON_DATA_SPECIES, &mon->stats[DEBUG_MON_SPECIES]);
    Pokemon_SetValue(mon->data, MON_DATA_LEVEL, &mon->stats[DEBUG_MON_LEVEL]);
    Pokemon_SetValue(mon->data, MON_DATA_EXPERIENCE, &mon->stats[DEBUG_MON_EXP]);
    Pokemon_SetValue(mon->data, MON_DATA_OT_ID, &mon->stats[DEBUG_MON_OT_ID]);
    Pokemon_SetValue(mon->data, MON_DATA_PERSONALITY, &mon->stats[DEBUG_MON_PERSONALITY]);
    Pokemon_SetValue(mon->data, MON_DATA_GENDER, &mon->stats[DEBUG_MON_GENDER]);

    if (mon->stats[DEBUG_MON_MOVE1]) {
        DebugMon_SetMoveAtPosition(mon->data, mon->stats[DEBUG_MON_MOVE1], 0);
    }
    if (mon->stats[DEBUG_MON_MOVE2]) {
        DebugMon_SetMoveAtPosition(mon->data, mon->stats[DEBUG_MON_MOVE2], 1);
    }
    if (mon->stats[DEBUG_MON_MOVE3]) {
        DebugMon_SetMoveAtPosition(mon->data, mon->stats[DEBUG_MON_MOVE3], 2);
    }
    if (mon->stats[DEBUG_MON_MOVE4]) {
        DebugMon_SetMoveAtPosition(mon->data, mon->stats[DEBUG_MON_MOVE4], 3);
    }

    Pokemon_SetValue(mon->data, MON_DATA_HELD_ITEM, &mon->stats[DEBUG_MON_HELD_ITEM]);

    int ability = SpeciesData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], SPECIES_DATA_ABILITY_1 + mon->stats[DEBUG_MON_ABILITY]);
    Pokemon_SetValue(mon->data, MON_DATA_ABILITY, &ability);

    Pokemon_SetValue(mon->data, MON_DATA_HP_IV, &mon->stats[DEBUG_MON_HP_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_HP_EV, &mon->stats[DEBUG_MON_HP_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_ATK_IV, &mon->stats[DEBUG_MON_ATK_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_ATK_EV, &mon->stats[DEBUG_MON_ATK_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_DEF_IV, &mon->stats[DEBUG_MON_DEF_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_DEF_EV, &mon->stats[DEBUG_MON_DEF_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPEED_IV, &mon->stats[DEBUG_MON_SPEED_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPEED_EV, &mon->stats[DEBUG_MON_SPEED_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPATK_IV, &mon->stats[DEBUG_MON_SPATK_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPATK_EV, &mon->stats[DEBUG_MON_SPATK_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPDEF_IV, &mon->stats[DEBUG_MON_SPDEF_IV]);
    Pokemon_SetValue(mon->data, MON_DATA_SPDEF_EV, &mon->stats[DEBUG_MON_SPDEF_EV]);
    Pokemon_SetValue(mon->data, MON_DATA_COOL, &mon->stats[DEBUG_MON_COOL]);
    Pokemon_SetValue(mon->data, MON_DATA_BEAUTY, &mon->stats[DEBUG_MON_BEAUTY]);
    Pokemon_SetValue(mon->data, MON_DATA_CUTE, &mon->stats[DEBUG_MON_CUTE]);
    Pokemon_SetValue(mon->data, MON_DATA_SMART, &mon->stats[DEBUG_MON_SMART]);
    Pokemon_SetValue(mon->data, MON_DATA_TOUGH, &mon->stats[DEBUG_MON_TOUGH]);
    Pokemon_SetValue(mon->data, MON_DATA_SHEEN, &mon->stats[DEBUG_MON_SHEEN]);
    Pokemon_SetValue(mon->data, MON_DATA_FRIENDSHIP, &mon->stats[DEBUG_MON_FRIENDSHIP]);
    Pokemon_SetValue(mon->data, MON_DATA_POKERUS, &mon->stats[DEBUG_MON_POKERUS]);
    Pokemon_SetValue(mon->data, MON_DATA_IS_EGG, &mon->stats[DEBUG_MON_IS_EGG]);

    Pokemon_SetValue(mon->data, MON_DATA_STATUS, &sStatusConditionTable[mon->stats[DEBUG_MON_STATUS]][1]);

    Pokemon_SetValue(mon->data, MON_DATA_MAX_HP, &mon->stats[DEBUG_MON_MAX_HP]);
    Pokemon_SetValue(mon->data, MON_DATA_ATK, &mon->stats[DEBUG_MON_ATK]);
    Pokemon_SetValue(mon->data, MON_DATA_DEF, &mon->stats[DEBUG_MON_DEF]);
    Pokemon_SetValue(mon->data, MON_DATA_SPEED, &mon->stats[DEBUG_MON_SPEED]);
    Pokemon_SetValue(mon->data, MON_DATA_SP_ATK, &mon->stats[DEBUG_MON_SP_ATK]);
    Pokemon_SetValue(mon->data, MON_DATA_SP_DEF, &mon->stats[DEBUG_MON_SP_DEF]);
    Pokemon_SetValue(mon->data, MON_DATA_FATEFUL_ENCOUNTER, &mon->stats[DEBUG_MON_FATEFUL_ENCOUNTER]);
    Pokemon_SetValue(mon->data, MON_DATA_HAS_NICKNAME, &mon->stats[DEBUG_MON_HAS_NICKNAME]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_LEVEL, &mon->stats[DEBUG_MON_MET_LEVEL]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_GAME, &mon->stats[DEBUG_MON_MET_GAME]);
    Pokemon_SetValue(mon->data, MON_DATA_POKEBALL, &mon->stats[DEBUG_MON_POKEBALL]);
    Pokemon_SetValue(mon->data, MON_DATA_FORM, &mon->stats[DEBUG_MON_FORM]);

    if (mon->stats[DEBUG_MON_IS_EGG]) {
        mon->stats[DEBUG_MON_EGG_LOCATION] = 0;
        mon->stats[DEBUG_MON_EGG_YEAR] = 0;
        mon->stats[DEBUG_MON_EGG_MONTH] = 0;
        mon->stats[DEBUG_MON_EGG_DAY] = 0;
        if (mon->stats[MON_DATA_MET_LOCATION] == 0) {
            mon->stats[MON_DATA_MET_LOCATION] = 1;
        }
    } else {
        Pokemon_SetValue(mon->data, MON_DATA_EGG_LOCATION, &mon->stats[DEBUG_MON_EGG_LOCATION]);
        Pokemon_SetValue(mon->data, MON_DATA_EGG_YEAR, &mon->stats[DEBUG_MON_EGG_YEAR]);
        Pokemon_SetValue(mon->data, MON_DATA_EGG_MONTH, &mon->stats[DEBUG_MON_EGG_MONTH]);
        Pokemon_SetValue(mon->data, MON_DATA_EGG_DAY, &mon->stats[DEBUG_MON_EGG_DAY]);
    }

    Pokemon_SetValue(mon->data, MON_DATA_MET_LOCATION, &mon->stats[DEBUG_MON_MET_LOCATION]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_YEAR, &mon->stats[DEBUG_MON_MET_YEAR]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_MONTH, &mon->stats[DEBUG_MON_MET_MONTH]);
    Pokemon_SetValue(mon->data, MON_DATA_MET_DAY, &mon->stats[DEBUG_MON_MET_DAY]);

    Pokemon_CalcLevelAndStats(mon->data);
}

static void DebugMon_SetMoveAtPosition(Pokemon *mon, u16 moveID, u16 movePos)
{
    u8 pp = MoveTable_LoadParam(moveID, MOVEATTRIBUTE_PP);
    Pokemon_SetValue(mon, MON_DATA_MOVE1 + movePos, &moveID);
    Pokemon_SetValue(mon, MON_DATA_MOVE1_PP + movePos, &pp);
}

static void DebugMonStat_Display(DebugMonMenu *monMenu, u8 statID, u32 color, u8 y)
{
    DebugMon *mon = &monMenu->mon;

    if (statID == DEBUG_MON_BATTLE_STATS_1) {
        DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_HP, 12, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_GREEN);
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[DEBUG_MON_MAX_HP], 3, 36, y + LINE_SPACING, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);

        DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_Attack, 84, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_GREEN);
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[DEBUG_MON_ATK], 3, 108, y + LINE_SPACING, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);

        DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_Defense, 156, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_GREEN);
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[DEBUG_MON_DEF], 3, 180, y + LINE_SPACING, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);
        return;
    }

    if (statID == DEBUG_MON_BATTLE_STATS_2) {
        DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_Speed, 12, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_GREEN);
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[DEBUG_MON_SPEED], 3, 36, y + LINE_SPACING, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);

        DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_SpAtk, 84, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_GREEN);
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[DEBUG_MON_SP_ATK], 3, 108, y + LINE_SPACING, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);

        DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_SpDef, 156, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_GREEN);
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[DEBUG_MON_SP_DEF], 3, 180, y + LINE_SPACING, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);
        return;
    }

    DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, sDebugMonStats[statID].label, 12, y, TEXT_SPEED_NO_TRANSFER, color);

    u8 digits = sDebugMonStats[statID].bounds->digits;

    switch (statID) {
    case DEBUG_MON_SPECIES:
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[statID], digits, 108, y, TEXT_SPEED_NO_TRANSFER, color);
        DebugMonStat_PrintSpeciesName(&monMenu->mainWindow, mon->stats[statID], 140, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);
        break;

    case DEBUG_MON_GENDER:
        switch (mon->stats[statID]) {
        case GENDER_MALE:
            DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_SymbolMale, 108, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);
            break;
        case GENDER_FEMALE:
            DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_SymbolFemale, 108, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);
            break;
        case GENDER_NONE:
            DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_None, 108, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);
            break;
        }
        break;

    case DEBUG_MON_MOVE1:
    case DEBUG_MON_MOVE2:
    case DEBUG_MON_MOVE3:
    case DEBUG_MON_MOVE4:
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[statID], digits, 108, y, TEXT_SPEED_NO_TRANSFER, color);
        StringTemplate_SetMoveName(monMenu->strTemplate, 0, mon->stats[statID]);
        DebugMonStat_PrintStrExpanded(monMenu, &monMenu->mainWindow, DebugMonMenu_Template_MoveName, 140, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    case DEBUG_MON_HELD_ITEM:
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[statID], digits, 108, y, TEXT_SPEED_NO_TRANSFER, color);
        StringTemplate_SetItemName(monMenu->strTemplate, 0, mon->stats[statID]);
        DebugMonStat_PrintStrExpanded(monMenu, &monMenu->mainWindow, DebugMonMenu_Template_HeldItem, 140, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    case DEBUG_MON_ABILITY:
        StringTemplate_SetAbilityName(monMenu->strTemplate, 0, SpeciesData_GetSpeciesValue(mon->stats[DEBUG_MON_SPECIES], SPECIES_DATA_ABILITY_1 + mon->stats[statID]));
        DebugMonStat_PrintStrExpanded(monMenu, &monMenu->mainWindow, DebugMonMenu_Template_Ability, 108, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    case DEBUG_MON_STATUS:
        DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, sStatusConditionTable[mon->stats[statID]][0], 108, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);
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
    case DEBUG_MON_EGG_LOCATION:
    case DEBUG_MON_MET_LEVEL:
    case DEBUG_MON_MET_GAME:
    case DEBUG_MON_POKEBALL:
    case DEBUG_MON_FORM:
    case DEBUG_MON_EGG_YEAR:
    case DEBUG_MON_EGG_MONTH:
    case DEBUG_MON_EGG_DAY:
    case DEBUG_MON_MET_LOCATION:
    case DEBUG_MON_MET_YEAR:
    case DEBUG_MON_MET_MONTH:
    case DEBUG_MON_MET_DAY:
        DebugMonStat_PrintNum(monMenu, &monMenu->mainWindow, mon->stats[statID], digits, 108, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    case DEBUG_MON_IS_EGG:
    case DEBUG_MON_FATEFUL_ENCOUNTER:
    case DEBUG_MON_HAS_NICKNAME:
        if (mon->stats[statID]) {
            DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_On, 108, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);
        } else {
            DebugMonStat_PrintStr(&monMenu->mainWindow, monMenu->msgLoader, DebugMonMenu_Text_Off, 108, y, TEXT_SPEED_NO_TRANSFER, DEBUG_TEXT_BLACK);
        }
        break;

    case DEBUG_MON_NATURE:
        StringTemplate_SetNatureName(monMenu->strTemplate, 0, mon->stats[statID]);
        DebugMonStat_PrintStrExpanded(monMenu, &monMenu->mainWindow, DebugMonMenu_Template_Nature, 108, y, TEXT_SPEED_NO_TRANSFER, color);
        break;

    default:
        break;
    }
}

static void DebugMonStat_PrintStr(Window *window, MessageLoader *msgLoader, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    String *buf = MessageLoader_GetNewString(msgLoader, entryID);
    Text_AddPrinterWithParamsAndColor(window, FONT_SYSTEM, buf, x, y, delay, color, NULL);
    String_Free(buf);
}

static void DebugMonStat_PrintStrExpanded(DebugMonMenu *monMenu, Window *window, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    String *buf = MessageLoader_GetNewString(monMenu->msgLoader, entryID);
    String *bufExp = String_Init(128, HEAP_ID_APPLICATION);

    StringTemplate_Format(monMenu->strTemplate, bufExp, buf);

    Text_AddPrinterWithParamsAndColor(window, FONT_SYSTEM, bufExp, x, y, delay, color, NULL);

    String_Free(buf);
    String_Free(bufExp);
}

static void DebugMonStat_PrintNum(DebugMonMenu *monMenu, Window *window, u32 num, u32 digits, u32 x, u32 y, u32 delay, u32 color)
{
    String *buf = MessageLoader_GetNewString(monMenu->msgLoader, DebugMonMenu_Template_Number);
    String *bufExp = String_Init(32, HEAP_ID_APPLICATION);

    u32 i;
    u32 tmp = 1;

    for (i = 0; i < digits - 1; i++) {
        tmp *= 10;
    }

    u32 size = 0;
    for (i = digits; i >= 1; i--) {
        StringTemplate_SetNumber(monMenu->strTemplate, 0, num / tmp, 1, PADDING_MODE_ZEROES, CHARSET_MODE_EN);
        StringTemplate_Format(monMenu->strTemplate, bufExp, buf);

        Text_AddPrinterWithParamsAndColor(window, FONT_SYSTEM, bufExp, x + size, y, TEXT_SPEED_NO_TRANSFER, DebugMonStat_GetColor(monMenu, i - 1, color), NULL);
        num %= tmp;
        tmp /= 10;
        size += Font_CalcStringWidth(0, bufExp, 0);
    }

    if (delay == TEXT_SPEED_INSTANT) {
        Window_ScheduleCopyToVRAM(window);
    }

    String_Free(buf);
    String_Free(bufExp);
}

static u32 DebugMonStat_GetColor(DebugMonMenu *monMenu, u8 digit, u32 color)
{
    if (color == DEBUG_TEXT_RED) {
        if (digit == monMenu->digits) {
            return DEBUG_TEXT_BLUE;
        } else {
            return DEBUG_TEXT_RED;
        }
    }
    return DEBUG_TEXT_BLACK;
}

static void DebugMonStat_PrintSpeciesName(Window *window, u32 species, u32 x, u32 y, u32 delay, u32 color)
{
    MessageLoader *msgLoader = MessageLoader_Init(MSG_LOADER_LOAD_ON_DEMAND, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_SPECIES_NAME, HEAP_ID_APPLICATION);
    String *buf = MessageLoader_GetNewString(msgLoader, species);

    Text_AddPrinterWithParamsAndColor(window, FONT_SYSTEM, buf, x, y, delay, color, NULL);

    String_Free(buf);
    MessageLoader_Free(msgLoader);
}

static u32 DebugMonStat_Get(DebugMon *mon, u8 statID)
{
    return mon->stats[statID];
}

static void DebugMonStat_Set(DebugMon *mon, u32 value, u8 statID)
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
