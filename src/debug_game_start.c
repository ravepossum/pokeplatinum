#include "debug_game_start.h"

#include "constants/map_object.h"
#include "constants/pokemon.h"
#include "generated/badges.h"
#include "generated/genders.h"
#include "generated/map_headers.h"
#include "generated/poketch_apps.h"
#include "generated/text_banks.h"
#include "generated/vars_flags.h"

#include "overlay005/debug_menu.h"

#include "bag.h"
#include "field_overworld_state.h"
#include "game_options.h"
#include "game_start.h"
#include "main.h"
#include "math_util.h"
#include "message.h"
#include "play_time_manager.h"
#include "player_avatar.h"
#include "pokedex.h"
#include "poketch.h"
#include "save_player.h"
#include "savedata_misc.h"
#include "system_flags.h"
#include "system_vars.h"
#include "unk_02054884.h"

#include "constdata/const_020EA10C.h"
#include "res/text/bank/debug_menu.h"

static void InitDebugSave(enum HeapID heapID, SaveData *saveData);
static void InitDebugVarsFlags(VarsFlags *varsFlags);
static void InitDebugTrainerInfo(SaveData *saveData, enum HeapID heapID);
static void InitDebugBag(Bag *bag, enum HeapID heapID);
static void InitDebugParty(SaveData *saveData, enum HeapID heapID);
static void InitDebugMisc(SaveData *saveData);
static int DebugMenu_GameStart_Init(ApplicationManager *appMan, int *state);
static int DebugMenu_GameStart_Main(ApplicationManager *appMan, int *state);
static int DebugMenu_GameStart_Exit(ApplicationManager *appMan, int *state);

const ApplicationManagerTemplate gDebugMenuGameStartAppTemplate = {
    .init = DebugMenu_GameStart_Init,
    .main = DebugMenu_GameStart_Main,
    .exit = DebugMenu_GameStart_Exit,
    .overlayID = FS_OVERLAY_ID_NONE,
};

static int DebugMenu_GameStart_Init(ApplicationManager *appMan, int *state)
{
    Heap_Create(HEAP_ID_APPLICATION, HEAP_ID_GAME_START, HEAP_SIZE_GAME_START);
    InitRNG();
    return TRUE;
}

static int DebugMenu_GameStart_Main(ApplicationManager *appMan, int *state)
{
    SaveData *saveData = ((ApplicationArgs *)ApplicationManager_Args(appMan))->saveData;
    InitDebugSave(HEAP_ID_GAME_START, saveData);
    InitializeNewSave(HEAP_ID_GAME_START, saveData, TRUE);
    PlayTime_Start(SaveData_GetPlayTime(saveData));
    return TRUE;
}

static int DebugMenu_GameStart_Exit(ApplicationManager *appMan, int *state)
{
    Heap_Destroy(HEAP_ID_GAME_START);
    EnqueueApplication(FS_OVERLAY_ID_NONE, &gFieldSystemNewGameTemplate);
    return TRUE;
}

static void InitDebugSave(enum HeapID heapID, SaveData *saveData)
{
    SaveData_Clear(saveData);
    InitDebugVarsFlags(SaveData_GetVarsFlags(saveData));
    InitDebugTrainerInfo(saveData, heapID);
    InitDebugBag(SaveData_GetBag(saveData), heapID);
    InitDebugParty(saveData, heapID);
    InitDebugMisc(saveData);
}

static void InitDebugVarsFlags(VarsFlags *varsFlags)
{
    VarsFlags_SetFlag(varsFlags, FLAG_HAS_POKEDEX);
    // if this flag is set, the new game scripts are slightly altered
    VarsFlags_SetFlag(varsFlags, FLAG_DEBUG_GAME_START);
    SystemFlag_SetBagAcquired(varsFlags);
    SystemVars_SetPlayerStarter(varsFlags, SPECIES_TURTWIG);
}

static void InitDebugTrainerInfo(SaveData *saveData, enum HeapID heapID)
{
    TrainerInfo *trainerInfo = SaveData_GetTrainerInfo(saveData);
    MessageLoader *msgLoader = MessageLoader_Init(MSG_LOADER_LOAD_ON_DEMAND, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_DEBUG_MENU, heapID);
    String *str = MessageLoader_GetNewString(msgLoader, DebugMenu_Text_PlayerName);
    TrainerInfo_SetNameFromString(trainerInfo, str);

    MessageLoader_GetString(msgLoader, DebugMenu_Text_RivalName, str);
    MiscSaveBlock_SetRivalName(SaveData_MiscSaveBlock(saveData), str);

    String_Free(str);
    MessageLoader_Free(msgLoader);

    TrainerInfo_SetGender(trainerInfo, LCRNG_Next() % (GENDER_FEMALE + 1));
    TrainerInfo_SetID(trainerInfo, MTRNG_Next());
    TrainerInfo_SetMoney(trainerInfo, 999999);

    for (u32 i = 0; i < MAX_BADGES; i++) {
        TrainerInfo_SetBadge(trainerInfo, i);
    }
}

static void InitDebugBag(Bag *bag, enum HeapID heapID)
{
    Bag_TryAddItem(bag, ITEM_BICYCLE, 1, heapID);
    Bag_TryAddItem(bag, ITEM_OLD_ROD, 1, heapID);
    Bag_TryAddItem(bag, ITEM_GOOD_ROD, 1, heapID);
    Bag_TryAddItem(bag, ITEM_SUPER_ROD, 1, heapID);
    Bag_TryAddItem(bag, ITEM_POFFIN_CASE, 1, heapID);
    Bag_TryAddItem(bag, ITEM_COIN_CASE, 1, heapID);
    Bag_TryAddItem(bag, ITEM_SEAL_BAG, 1, heapID);
    Bag_TryAddItem(bag, ITEM_PAL_PAD, 1, heapID);
}

static void InitDebugParty(SaveData *saveData, enum HeapID heapID)
{
    Party *party = SaveData_GetParty(saveData);
    Pokemon_GiveMonFromScript(heapID, saveData, SPECIES_BIBAREL, MAX_POKEMON_LEVEL, ITEM_NONE, 0, 0);
    Pokemon_SetMoveSlot(&party->pokemon[0], MOVE_SURF, 0);
    Pokemon_SetMoveSlot(&party->pokemon[0], MOVE_CUT, 1);
    Pokemon_SetMoveSlot(&party->pokemon[0], MOVE_WATERFALL, 2);
    Pokemon_SetMoveSlot(&party->pokemon[0], MOVE_ROCK_CLIMB, 3);

    Pokemon_GiveMonFromScript(heapID, saveData, SPECIES_TROPIUS, MAX_POKEMON_LEVEL, ITEM_NONE, 0, 0);
    Pokemon_SetMoveSlot(&party->pokemon[1], MOVE_FLY, 0);
    Pokemon_SetMoveSlot(&party->pokemon[1], MOVE_DEFOG, 1);
    Pokemon_SetMoveSlot(&party->pokemon[1], MOVE_STRENGTH, 2);
    Pokemon_SetMoveSlot(&party->pokemon[1], MOVE_ROCK_SMASH, 3);
}

static void InitDebugMisc(SaveData *saveData)
{
    FieldOverworldState *fieldState = SaveData_GetFieldOverworldState(saveData);
    Location *playerLoc = FieldOverworldState_GetPlayerLocation(fieldState);
    Location startLoc;
    // 1 here is the fly location for twinleaf
    Location_InitFly(1, &startLoc);
    *playerLoc = startLoc;

    Options *options = SaveData_GetOptions(saveData);
    Options_SetTextSpeed(options, OPTIONS_TEXT_SPEED_FAST);

    Poketch *poketch = SaveData_GetPoketch(saveData);
    Poketch_Enable(poketch);
    for (u32 i = 0; i < POKETCH_APPID_MAX; i++) {
        Poketch_RegisterApp(poketch, i);
    }

    PlayerData *playerData = FieldOverworldState_GetPlayerData(fieldState);
    PlayerData_SetRunningShoes(playerData, TRUE);

    Pokedex *pokedex = SaveData_GetPokedex(saveData);
    Pokedex_ObtainPokedex(pokedex);
}
