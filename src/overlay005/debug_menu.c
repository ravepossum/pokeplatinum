#include "overlay005/debug_menu.h"

#include "constants/charcode.h"
#include "constants/heap.h"
#include "constants/map_object.h"
#include "constants/narc.h"
#include "constants/pokemon.h"
#include "generated/items.h"
#include "generated/moves.h"
#include "generated/sdat.h"
#include "generated/species.h"
#include "generated/text_banks.h"

#include "struct_defs/struct_02090800.h"

#include "applications/town_map/main.h"
#include "field/field_system.h"
#include "overlay005/debug_mon_menu.h"
#include "overlay006/hm_cut_in.h"

#include "camera.h"
#include "field_map_change.h"
#include "field_system.h"
#include "message.h"
#include "move_table.h"
#include "party.h"
#include "render_window.h"
#include "screen_fade.h"
#include "string_gf.h"
#include "sys_task.h"
#include "system.h"
#include "text.h"
#include "unk_0203A7D8.h"
#include "unk_0203D1B8.h"
#include "unk_02092494.h"
#include "vars_flags.h"

static void DebugMenu_Free(DebugMenu *menu);
static void DebugMenu_Close(SysTask *task, DebugMenu *menu);
static void DebugMenu_ExitToField(SysTask *task, DebugMenu *menu);
static void Task_DebugMenu_ExitToField(SysTask *task, void *data);
static StringList *DebugMenu_CreateList(int arcID, const DebugMenuItem *list, int count);
static DebugMenu *DebugMenu_CreateMultichoice(FieldSystem *fieldSystem, int arcID, const DebugMenuItem *list, int count, SysTaskFunc taskFunc);
static void Task_DebugMenu_HandleInput(SysTask *task, void *data);

static void DebugFunction_Fly(SysTask *task, DebugMenu *menu);
static void DebugMenu_Fly_CreateTask(FieldSystem *fieldSystem);
static void Task_DebugMenu_Fly(SysTask *task, void *data);

static void DebugFunction_CreateMon(SysTask *task, DebugMenu *menu);
static void DebugFunction_EditMon(SysTask *task, DebugMenu *menu);
static void DebugMenu_CreateOrEditMon_CreateTask(FieldSystem *fieldSystem, enum DebugMonMenuMode mode);
static void Task_DebugMenu_CreateOrEditMon(SysTask *task, void *data);

static void DebugFunction_AdjustCamera(SysTask *task, DebugMenu *menu);
static void DebugMenu_AdjustCamera_CreateTask(FieldSystem *fieldSystem, DebugMenu *menu);
static void Task_DebugMenu_AdjustCamera(SysTask *task, void *data);

static void DebugFunction_ToggleCollision(SysTask *task, DebugMenu *menu);
static void DebugFunction_ExecuteFunction(SysTask *task, DebugMenu *menu);

static const WindowTemplate DebugMenu_List_WindowTemplate = {
    .bgLayer = BG_LAYER_MAIN_3,
    .tilemapLeft = 1,
    .tilemapTop = 6,
    .width = 13,
    .height = 17,
    .palette = 13,
    .baseTile = 8,
};

static const ListMenuTemplate DebugMenu_List_Header = {
    .choices = NULL,
    .cursorCallback = NULL,
    .printCallback = NULL,
    .window = NULL,
    .count = 0,
    .maxDisplay = 8,
    .headerXOffset = 2,
    .textXOffset = 12,
    .cursorXOffset = 1,
    .yOffset = 8,
    .textColorFg = 11, // white
    .textColorBg = 15, // black
    .textColorShadow = 2, // black shadow
    .letterSpacing = 0,
    .lineSpacing = 0,
    .pagerMode = PAGER_MODE_LEFT_RIGHT_PAD,
    .fontID = FONT_SYSTEM,
    .cursorType = 0,
    .parent = NULL,
};

static const DebugMenuItem DebugMenu_ItemList[] = {
    // clang-format off
    { DEBUG_ITEM_FLY,               DebugFunction_Fly },
    { DEBUG_ITEM_CREATE_MON,        DebugFunction_CreateMon },
    { DEBUG_ITEM_EDIT_MON,          DebugFunction_EditMon },
    { DEBUG_ITEM_TOGGLE_COLLISION,  DebugFunction_ToggleCollision },
    { DEBUG_ITEM_ADJUST_CAMERA,     DebugFunction_AdjustCamera },
    { DEBUG_ITEM_EXECUTE_FUNCTION,  DebugFunction_ExecuteFunction },
    // clang-format on
};

void DebugMenu_Init(FieldSystem *fieldSystem)
{
    DebugMenu *menu = DebugMenu_CreateMultichoice(fieldSystem, TEXT_BANK_UNK_0328, DebugMenu_ItemList, NELEMS(DebugMenu_ItemList), NULL);
    FieldSystem_PauseProcessing();
}

static void DebugMenu_Free(DebugMenu *menu)
{
    Window_EraseStandardFrame(menu->window, TRUE);
    Window_ClearAndCopyToVRAM(menu->window);
    StringList_Free(menu->stringList);
    ListMenu_Free(menu->listMenu, &(menu->listPos), &(menu->cursor));
    Window_Remove(menu->window);
    Heap_Free(menu->window);
}

static void DebugMenu_Close(SysTask *task, DebugMenu *menu)
{
    DebugMenu_Free(menu);
    Heap_Free(menu);
    SysTask_Done(task);
}

static void DebugMenu_ExitToField(SysTask *task, DebugMenu *menu)
{
    DebugMenu_Free(menu);
    SysTask_SetCallback(task, Task_DebugMenu_ExitToField);
}

static void Task_DebugMenu_ExitToField(SysTask *task, void *data)
{
    Heap_Free(data);
    SysTask_Done(task);
    FieldSystem_ResumeProcessing();
}

static DebugMenu *DebugMenu_CreateMultichoice(FieldSystem *fieldSystem, int arcID, const DebugMenuItem *list, int count, SysTaskFunc taskFunc)
{
    DebugMenu *menu = Heap_Alloc(HEAP_ID_FIELD1, sizeof(DebugMenu));

    if (menu == NULL) {
        return NULL;
    }

    MI_CpuClear8(menu, sizeof(DebugMenu));

    menu->fieldSystem = fieldSystem;

    SysTask *taskPtr = NULL;
    if (taskFunc == NULL) {
        taskPtr = SysTask_Start(Task_DebugMenu_HandleInput, menu, 0);
    } else {
        taskPtr = SysTask_Start(taskFunc, menu, 0);
    }

    menu->listPos = 0;
    menu->cursor = 0;

    LoadStandardWindowGraphics(menu->fieldSystem->bgConfig, BG_LAYER_MAIN_3, 660, 11, STANDARD_WINDOW_SYSTEM, HEAP_ID_FIELD1);
    menu->window = Window_New(HEAP_ID_FIELD1, 1);
    Window_AddFromTemplate(menu->fieldSystem->bgConfig, menu->window, &DebugMenu_List_WindowTemplate);
    Window_DrawStandardFrame(menu->window, TRUE, 660, 11);

    if (list != NULL) {
        menu->stringList = DebugMenu_CreateList(arcID, list, count);
    }

    ListMenuTemplate listHeader = DebugMenu_List_Header;
    listHeader.choices = menu->stringList;
    listHeader.window = menu->window;
    listHeader.count = count;

    menu->listMenu = ListMenu_New(&listHeader, menu->listPos, menu->cursor, HEAP_ID_FIELD1);

    return menu;
}

static StringList *DebugMenu_CreateList(int arcID, const DebugMenuItem *list, int count)
{
    StringList *stringList = StringList_New(count, HEAP_ID_FIELD1);
    MessageLoader *msgLoader = MessageLoader_Init(MSG_LOADER_PRELOAD_ENTIRE_BANK, NARC_INDEX_MSGDATA__PL_MSG, arcID, HEAP_ID_FIELD1);

    for (int i = 0; i < count; i++) {
        StringList_AddFromMessageBank(stringList, msgLoader, list[i].index + 1, list[i].function);
    }
    MessageLoader_Free(msgLoader);

    return stringList;
}

static void Task_DebugMenu_HandleInput(SysTask *task, void *data)
{
    DebugMenu *menu = (DebugMenu *)data;
    s32 choice = ListMenu_ProcessInput(menu->listMenu);
    SysTaskFunc taskFunc;

    if (JOY_NEW(PAD_BUTTON_A)) {
        if (choice) {
            taskFunc = (SysTaskFunc)choice;
            taskFunc(task, data);
        }
        return;
    }

    if (JOY_NEW(PAD_BUTTON_B)) {
        DebugMenu_ExitToField(task, menu);
    }
}

// Fly section

static void DebugFunction_Fly(SysTask *task, DebugMenu *menu)
{
    DebugMenu_Fly_CreateTask(menu->fieldSystem);
    DebugMenu_Close(task, menu);
}

static void DebugMenu_Fly_CreateTask(FieldSystem *fieldSystem)
{
    DebugFly *fly = Heap_Alloc(HEAP_ID_APPLICATION, sizeof(DebugFly));
    MI_CpuClear8(fly, sizeof(DebugFly));
    fly->fieldSystem = fieldSystem;
    SysTask_Start(Task_DebugMenu_Fly, fly, 0);
}

static void Task_DebugMenu_Fly(SysTask *task, void *data)
{
    DebugFly *fly = (DebugFly *)data;
    TownMapContext *mapCtx;

    switch (fly->state) {
    case DEBUG_FLY_FADE_OUT:
        StartScreenFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, COLOR_BLACK, 6, 1, HEAP_ID_APPLICATION);
        break;
    case DEBUG_FLY_WAIT_FADE_AND_INIT:
        if (!IsScreenFadeDone()) {
            return;
        }

        fly->mapCtx = Heap_Alloc(HEAP_ID_APPLICATION, sizeof(TownMapContext));

        TownMapContext_Init(fly->fieldSystem, fly->mapCtx, TOWN_MAP_MODE_FLY);

        mapCtx = fly->mapCtx;
        mapCtx->debugActive = TRUE;

        FieldSystem_OpenTownMap(fly->fieldSystem, fly->mapCtx);
        break;
    case DEBUG_FLY_START_MAP:
        if (FieldSystem_IsRunningApplication(fly->fieldSystem)) {
            return;
        }

        FieldSystem_StartFieldMap(fly->fieldSystem);
        break;
    case DEBUG_FLY_FADE_IN:
        if (!FieldSystem_IsRunningFieldMap(fly->fieldSystem)) {
            return;
        }

        StartScreenFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, COLOR_BLACK, 6, 1, HEAP_ID_APPLICATION);
        break;
    case DEBUG_FLY_WAIT_FADE_AND_CUTIN:
        if (!IsScreenFadeDone()) {
            return;
        }

        mapCtx = fly->mapCtx;
        if (!mapCtx->flyLocationSelected) {
            fly->state = DEBUG_FLY_EXIT;
            return;
        }

        Pokemon *mon = Party_GetPokemonBySlotIndex(SaveData_GetParty(fly->fieldSystem->saveData), 0);
        fly->taskCutIn = SysTask_HMCutIn_New(fly->fieldSystem, TRUE, mon, PlayerAvatar_Gender(fly->fieldSystem->playerAvatar));

        break;
    case DEBUG_FLY_WAIT_CUTIN_AND_START_FLY:
        if (!CheckHMCutInFinished(fly->taskCutIn)) {
            return;
        }

        SysTask_HMCutIn_SetTaskDone(fly->taskCutIn);

        mapCtx = fly->mapCtx;

        // get warp data for heal spot?
        u16 warpID = sub_0203A8A0(mapCtx->flyLocationMapHeader, mapCtx->flyLocationX, mapCtx->flyLocationZ);

        Location destLoc;
        Location_InitFly(warpID, &destLoc);

        FieldTask_StartMapChangeFly(fly->fieldSystem, destLoc.mapId, (-1), destLoc.x, destLoc.z, DIR_SOUTH);
        break;
    case DEBUG_FLY_EXIT:
        Heap_Free(fly->mapCtx);
        Heap_Free(fly);
        SysTask_Done(task);
        return;
    }

    fly->state++;
}

// CreateMon and EditMon section

static void DebugFunction_CreateMon(SysTask *task, DebugMenu *menu)
{
    DebugMenu_Close(task, menu);
    DebugMenu_CreateOrEditMon_CreateTask(menu->fieldSystem, DEBUG_MON_MENU_MODE_CREATE);
}

static void DebugFunction_EditMon(SysTask *task, DebugMenu *menu)
{
    DebugMenu_Close(task, menu);
    DebugMenu_CreateOrEditMon_CreateTask(menu->fieldSystem, DEBUG_MON_MENU_MODE_EDIT);
}

static void DebugMenu_CreateOrEditMon_CreateTask(FieldSystem *fieldSystem, enum DebugMonMenuMode mode)
{
    DebugMonMenu *monMenu = (DebugMonMenu *)SysTask_GetParam(SysTask_StartAndAllocateParam(Task_DebugMenu_CreateOrEditMon, sizeof(DebugMonMenu), 0, HEAP_ID_APPLICATION));
    monMenu->fieldSystem = fieldSystem;
    monMenu->state = DMM_STATE_DRAW_MENU;
    monMenu->mode = mode;
    monMenu->msgLoader = MessageLoader_Init(MSG_LOADER_PRELOAD_ENTIRE_BANK, NARC_INDEX_MSGDATA__PL_MSG, DEBUG_MON_MENU_MESSAGE_BANK, HEAP_ID_APPLICATION);
    monMenu->strTemplate = StringTemplate_Default(HEAP_ID_APPLICATION);
    monMenu->cursor = ColoredArrow_New(HEAP_ID_APPLICATION);
    monMenu->partySlot = 0;

    BgConfig *bgConfig = FieldSystem_GetBgConfig(fieldSystem);

    Window_Add(bgConfig, &monMenu->titleWindow, BG_LAYER_MAIN_3, 1, 1, 30, 4, 13, 1);
    Window_Add(bgConfig, &monMenu->mainWindow, BG_LAYER_MAIN_3, 1, 7, 30, 16, 13, 1 + 30 * 4);

    LoadStandardWindowGraphics(bgConfig, BG_LAYER_MAIN_3, 1 + 30 * 4 + 30 * 18, 14, STANDARD_WINDOW_SYSTEM, HEAP_ID_APPLICATION);

    Window_DrawStandardFrame(&monMenu->titleWindow, TRUE, 1 + 30 * 4 + 30 * 18, 14);
    Window_DrawStandardFrame(&monMenu->mainWindow, TRUE, 1 + 30 * 4 + 30 * 18, 14);

    DebugMonMenu_Init(monMenu);
}

static void Task_DebugMenu_CreateOrEditMon(SysTask *task, void *data)
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

// Adjust Camera section

static void DebugFunction_AdjustCamera(SysTask *task, DebugMenu *menu)
{
    DebugMenu_AdjustCamera_CreateTask(menu->fieldSystem, menu);
    DebugMenu_Close(task, menu);
}

static void DebugMenu_AdjustCamera_CreateTask(FieldSystem *fieldSystem, DebugMenu *menu)
{
    menu->data = Camera_GetFOV(menu->fieldSystem->camera);
    SysTask *camTask = SysTask_Start(Task_DebugMenu_AdjustCamera, menu, 0);
}

#define tCameraFOV menu->data

#define CAMERA_FOV_INCREMENT   100
#define CAMERA_ANGLE_INCREMENT 800

static void Task_DebugMenu_AdjustCamera(SysTask *task, void *data)
{
    DebugMenu *menu = (DebugMenu *)data;
    Camera *cam = menu->fieldSystem->camera;
    CameraAngle angle = { 0, 0, 0, 0 };

    if (JOY_HELD(PAD_BUTTON_R)) {
        tCameraFOV += CAMERA_FOV_INCREMENT;
    } else if (JOY_HELD(PAD_BUTTON_L)) {
        tCameraFOV -= CAMERA_FOV_INCREMENT;
    }

    if (JOY_HELD(PAD_KEY)) {
        if (JOY_HELD(PAD_KEY_UP)) {
            angle.x = -CAMERA_ANGLE_INCREMENT;
        } else if (JOY_HELD(PAD_KEY_DOWN)) {
            angle.x = CAMERA_ANGLE_INCREMENT;
        }

        if (JOY_HELD(PAD_KEY_LEFT)) {
            angle.y = -CAMERA_ANGLE_INCREMENT;
        } else if (JOY_HELD(PAD_KEY_RIGHT)) {
            angle.y = CAMERA_ANGLE_INCREMENT;
        }

        Camera_AdjustAngleAroundTarget(&angle, cam);
    }

    Camera_SetFOV(tCameraFOV, cam);

    if (JOY_HELD(PAD_BUTTON_START)) {
        SysTask_Done(task);
        FieldSystem_ResumeProcessing();
    }
}

#undef tCameraFOV

// Smaller functionality

static void DebugFunction_ToggleCollision(SysTask *task, DebugMenu *menu)
{
    VarsFlags *varsFlags = SaveData_GetVarsFlags(menu->fieldSystem->saveData);

    if (VarsFlags_CheckFlag(varsFlags, DEBUG_FLAG_NO_COLLISION)) {
        VarsFlags_ClearFlag(varsFlags, DEBUG_FLAG_NO_COLLISION);
        Sound_PlayEffect(SEQ_SE_DP_PC_LOGOFF);
    } else {
        VarsFlags_SetFlag(varsFlags, DEBUG_FLAG_NO_COLLISION);
        Sound_PlayEffect(SEQ_SE_DP_PC_LOGIN);
    }

    DebugMenu_ExitToField(task, menu);
}

// shell function to run any arbitrary code you need
static void DebugFunction_ExecuteFunction(SysTask *task, DebugMenu *menu)
{
    // debug function start

    // debug function end
    DebugMenu_ExitToField(task, menu);
}
