#include "overlay005/debug_menu.h"

#include "constants/charcode.h"
#include "constants/heap.h"
#include "constants/map_object.h"
#include "constants/narc.h"
#include "constants/pokemon.h"
#include "consts/items.h"
#include "consts/moves.h"
#include "consts/pokemon.h"
#include "consts/sdat.h"
#include "consts/species.h"

#include "struct_defs/struct_0203D8AC.h"
#include "struct_defs/struct_02090800.h"

#include "field/field_system.h"
#include "overlay005/debug_mon_menu.h"
#include "overlay006/ov6_02243258.h"
#include "text/pl_msg.naix"

#include "camera.h"
#include "core_sys.h"
#include "field_map_change.h"
#include "field_system.h"
#include "message.h"
#include "move_table.h"
#include "party.h"
#include "render_window.h"
#include "strbuf.h"
#include "sys_task.h"
#include "text.h"
#include "unk_0200F174.h"
#include "unk_0203A7D8.h"
#include "unk_0203D1B8.h"
#include "unk_020508D4.h"
#include "unk_0206B70C.h"
#include "unk_02092494.h"
#include "vars_flags.h"

static void DebugMenu_Free(DebugMenu *menu);
static void CB_DebugMenu_Exit(SysTask *task, DebugMenu *menu);
static void DebugMenu_Exit(SysTask *task, DebugMenu *menu);
static void Task_DebugMenu_Exit(SysTask *task, void *data);
static void DebugMenu_List_Init(DebugMenu *menu, const DebugMenuItem *list);
static StringList *DebugMenu_CreateList(int arcID, const DebugMenuItem *list, int count);
static StringList *DebugMenu_CreateSubList(int count);
static DebugMenu *DebugMenu_CreateMultichoice(FieldSystem *sys, int arcID, const DebugMenuItem *list, int count, SysTaskFunc taskFunc);
static void Task_DebugMenu_HandleInput(SysTask *task, void *data);
static void CB_DebugMenu_ListHeader(ListMenu *listMenu, u32 param, u8 y);
static void DebugFunction_Dummy(SysTask *task, DebugMenu *menu);

static void DebugFunction_Fly(SysTask *task, DebugMenu *menu);
static void DebugMenu_Fly_CreateTask(FieldSystem *sys);
static void Task_DebugMenu_Fly(SysTask *task, void *data);

static void DebugFunction_CreateMon(SysTask *task, DebugMenu *menu);
static void DebugFunction_EditMon(SysTask *task, DebugMenu *menu);
static void DebugMenu_CreateOrEditMon_CreateTask(FieldSystem *sys, enum DebugMonMenuMode mode);
static void Task_DebugMenu_CreateOrEditMon(SysTask *task, void *data);

static void DebugFunction_AdjustCamera(SysTask *task, DebugMenu *menu);
static void DebugMenu_AdjustCamera_CreateTask(FieldSystem *sys, DebugMenu *menu);
static void Task_DebugMenu_AdjustCamera(SysTask *task, void *data);

static void DebugFunction_ToggleCollision(SysTask *task, DebugMenu *menu);
static void DebugFunction_ExecuteFunction(SysTask *task, DebugMenu *menu);

static const WindowTemplate DebugMenu_List_WindowTemplate = {
    .bgLayer = BG_LAYER_MAIN_3,
    .tilemapLeft = 0,
    .tilemapTop = 5,
    .width = 13,
    .height = 18,
    .palette = 13,
    .baseTile = 8
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
    .tmp = NULL,
};

static const DebugMenuItem DebugMenu_ItemList[] = {
    // clang-format off
    { DEBUG_ITEM_HEADER,            DebugFunction_Dummy },
    { DEBUG_ITEM_FLY,               DebugFunction_Fly },
    { DEBUG_ITEM_CREATE_MON,        DebugFunction_CreateMon },
    { DEBUG_ITEM_EDIT_MON,          DebugFunction_EditMon },
    { DEBUG_ITEM_TOGGLE_COLLISION,  DebugFunction_ToggleCollision },
    { DEBUG_ITEM_ADJUST_CAMERA,     DebugFunction_AdjustCamera },
    { DEBUG_ITEM_EXECUTE_FUNCTION,  DebugFunction_ExecuteFunction },
    // clang-format on
};

void DebugMenu_Init(FieldSystem *sys)
{
    DebugMenu *menu = DebugMenu_CreateMultichoice(sys, message_bank_unk_0328, DebugMenu_ItemList, NELEMS(DebugMenu_ItemList), NULL);
    menu->callback = NULL;

    // field system hold sequence
    sub_0203D128();
}

static void DebugMenu_Free(DebugMenu *menu)
{
    Window_ClearAndCopyToVRAM(menu->window);
    StringList_Free(menu->stringList);
    ListMenu_Free(menu->listMenu, &(menu->debugList), &(menu->cursor));
    Window_Remove(menu->window);
    Heap_FreeToHeap(menu->window);
}

static void CB_DebugMenu_Exit(SysTask *task, DebugMenu *menu)
{
    DebugMenu_Free(menu);
    Heap_FreeToHeap(menu);
    SysTask_Done(task);
}

static void DebugMenu_Exit(SysTask *task, DebugMenu *menu)
{
    DebugMenu_Free(menu);
    SysTask_SetCallback(task, Task_DebugMenu_Exit);
}

static void Task_DebugMenu_Exit(SysTask *task, void *data)
{
    Heap_FreeToHeap(data);
    SysTask_Done(task);
    // hold sequence end?
    sub_0203D140();
}

static DebugMenu *DebugMenu_CreateMultichoice(FieldSystem *sys, int arcID, const DebugMenuItem *list, int count, SysTaskFunc taskFunc)
{
    DebugMenu *menu = Heap_AllocFromHeap(HEAP_ID_FIELD, sizeof(DebugMenu));

    if (menu == NULL) {
        return NULL;
    }

    MI_CpuClear8(menu, sizeof(DebugMenu));

    menu->sys = sys;

    SysTask *taskPtr = NULL;
    if (taskFunc == NULL) {
        taskPtr = SysTask_Start(Task_DebugMenu_HandleInput, menu, 0);
    } else {
        taskPtr = SysTask_Start(taskFunc, menu, 0);
    }

    DebugMenu_List_Init(menu, list);
    LoadStandardWindowGraphics(menu->sys->bgConfig, BG_LAYER_MAIN_3, 473, 11, STANDARD_WINDOW_SYSTEM, HEAP_ID_FIELD);

    menu->window = Window_New(HEAP_ID_FIELD, 1);
    Window_AddFromTemplate(menu->sys->bgConfig, menu->window, &DebugMenu_List_WindowTemplate);

    if (list != NULL) {
        menu->stringList = DebugMenu_CreateList(arcID, list, count);
    }

    ListMenuTemplate listHeader = DebugMenu_List_Header;
    listHeader.choices = menu->stringList;
    listHeader.window = menu->window;
    listHeader.count = count;

    menu->listMenu = ListMenu_New(&listHeader, menu->debugList, menu->cursor, HEAP_ID_FIELD);

    return menu;
}

static StringList *DebugMenu_CreateList(int arcID, const DebugMenuItem *list, int count)
{
    StringList *stringList = StringList_New(count, HEAP_ID_FIELD);
    MessageLoader *msgLoader = MessageLoader_Init(MESSAGE_LOADER_BANK_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, arcID, HEAP_ID_FIELD);

    for (int i = 0; i < count; i++) {
        StringList_AddFromMessageBank(stringList, msgLoader, list[i].index, list[i].function);
    }
    MessageLoader_Free(msgLoader);

    return stringList;
}

static void DebugMenu_List_Init(DebugMenu *menu, const DebugMenuItem *list)
{
    if (menu->debugList == 0
        && menu->cursor == 0
        && list != NULL
        && list[menu->debugList].function == DebugFunction_Dummy) {

        menu->debugList = 0;
        menu->cursor = 1;
    }
}

static void Task_DebugMenu_HandleInput(SysTask *task, void *data)
{
    DebugMenu *menu = (DebugMenu *)data;
    s32 choice = ListMenu_ProcessInput(menu->listMenu);
    SysTaskFunc taskFunc;

    if (gCoreSys.pressedKeys & PAD_BUTTON_A) {
        if (choice) {
            taskFunc = (SysTaskFunc)choice;
            taskFunc(task, data);
        }
        return;
    }

    if (gCoreSys.pressedKeys & PAD_BUTTON_B) {
        if (menu->callback != NULL) {
            void (*taskFunc)(FieldSystem *) = menu->callback;
            CB_DebugMenu_Exit(task, menu);
            taskFunc(menu->sys);
        } else {
            DebugMenu_Exit(task, menu);
        }
    }
}

static void DebugFunction_Dummy(SysTask *task, DebugMenu *menu)
{
}

// Fly section

static void DebugFunction_Fly(SysTask *task, DebugMenu *menu)
{
    DebugMenu_Fly_CreateTask(menu->sys);
    CB_DebugMenu_Exit(task, menu);
}

static void DebugMenu_Fly_CreateTask(FieldSystem *sys)
{
    DebugFly *fly = Heap_AllocFromHeap(HEAP_ID_APPLICATION, sizeof(DebugFly));
    MI_CpuClear8(fly, sizeof(DebugFly));
    fly->sys = sys;
    SysTask_Start(Task_DebugMenu_Fly, fly, 0);
}

static void Task_DebugMenu_Fly(SysTask *task, void *data)
{
    DebugFly *fly = (DebugFly *)data;
    UnkStruct_0203D8AC *map;

    switch (fly->sequence) {
    case 0:
        fly->wipeFunc = 0;
        // screen wipe start
        sub_0200F174(0, 0, 0, 0x0000, 6, 1, HEAP_ID_APPLICATION);
        break;
    case 1:
        if (!ScreenWipe_Done()) {
            return;
        }

        fly->data = Heap_AllocFromHeap(HEAP_ID_APPLICATION, sizeof(UnkStruct_0203D8AC));

        // map data set
        sub_0206B70C(fly->sys, fly->data, 1);

        map = (UnkStruct_0203D8AC *)fly->data;
        map->debugActive = TRUE;

        // map set process
        sub_0203D884(fly->sys, fly->data);
        break;
    case 2:
        // field event wait for sub process
        if (sub_020509B4(fly->sys)) {
            return;
        }
        // field event set map process
        sub_020509D4(fly->sys);
        break;
    case 3:
        // field event wait for map process to start
        if (!sub_020509DC(fly->sys)) {
            return;
        }

        fly->wipeFunc = 0;
        // start screen wipe
        sub_0200F174(0, 1, 1, 0x0000, 6, 1, HEAP_ID_APPLICATION);
        break;
    case 4:
        if (!ScreenWipe_Done()) {
            return;
        }

        map = (UnkStruct_0203D8AC *)fly->data;
        if (!map->unk_10) {
            fly->sequence = 6;
            return;
        }

        Pokemon *mon = Party_GetPokemonBySlotIndex(Party_GetFromSavedata(fly->sys->saveData), 0);
        // init cut in, get gender
        fly->taskCutIn = ov6_02243F88(fly->sys, 1, mon, PlayerAvatar_Gender(fly->sys->playerAvatar));

        break;
    case 5:
        // check if cut in is finished
        if (ov6_02243FBC(fly->taskCutIn) == FALSE) {
            return;
        }
        // end cut in
        ov6_02243FC8(fly->taskCutIn);

        map = (UnkStruct_0203D8AC *)fly->data;

        // get warp data for heal spot?
        u16 warpID = sub_0203A8A0(map->unk_1C, map->unk_14, map->unk_18);

        Location destloc;
        // set location from warp?
        sub_0203A7F0(warpID, &destloc);

        FieldTask_StartMapChangeFly(fly->sys, destloc.mapId, (-1), destloc.x, destloc.z, DIR_SOUTH);
        break;
    case 6:
        Heap_FreeToHeap(fly->data);
        Heap_FreeToHeap(fly);
        SysTask_Done(task);
        return;
    }

    fly->sequence++;
}

// CreateMon and EditMon section

static void DebugFunction_CreateMon(SysTask *task, DebugMenu *menu)
{
    CB_DebugMenu_Exit(task, menu);
    DebugMenu_CreateOrEditMon_CreateTask(menu->sys, DEBUG_MON_MENU_MODE_CREATE);
}

static void DebugFunction_EditMon(SysTask *task, DebugMenu *menu)
{
    CB_DebugMenu_Exit(task, menu);
    DebugMenu_CreateOrEditMon_CreateTask(menu->sys, DEBUG_MON_MENU_MODE_EDIT);
}

static void DebugMenu_CreateOrEditMon_CreateTask(FieldSystem *sys, enum DebugMonMenuMode mode)
{
    DebugMonMenu *monMenu = (DebugMonMenu *)SysTask_GetParam(SysTask_StartAndAllocateParam(Task_DebugMenu_CreateOrEditMon, sizeof(DebugMonMenu), 0, HEAP_ID_APPLICATION));
    monMenu->sys = sys;
    monMenu->state = 0;
    monMenu->mode = mode;
    monMenu->msgLoader = MessageLoader_Init(MESSAGE_LOADER_BANK_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, DEBUG_MON_MENU_MESSAGE_BANK, HEAP_ID_APPLICATION);
    monMenu->strTemplate = StringTemplate_Default(HEAP_ID_APPLICATION);
    monMenu->cursor = ColoredArrow_New(HEAP_ID_APPLICATION);
    monMenu->partySlot = 0;

    // get bgConfig from fieldsys?
    BgConfig *bgConfig = sub_0203D170(sys);

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
    case 0:
        DebugMonMenu_DisplayPageAndCursor(monMenu);
        break;
    case 1:
        DebugMonMenu_HandleInput(monMenu);
        break;
    case 2:
        DebugMonMenu_HandleValueInput(monMenu);
        break;
    case 3:
        DebugMonMenu_WaitButtonPress(monMenu);
        break;
    case 4:
        DebugMonMenu_Free(monMenu);
        SysTask_FinishAndFreeParam(task);

        // field system hold end
        sub_0203D140();
        break;
    case 5:
        if (gCoreSys.pressedKeys & (PAD_BUTTON_X | PAD_BUTTON_Y)) {
            monMenu->state = 0;
        }
        break;
    }
}

// Adjust Camera section

static void DebugFunction_AdjustCamera(SysTask *task, DebugMenu *menu)
{
    DebugMenu_AdjustCamera_CreateTask(menu->sys, menu);
    CB_DebugMenu_Exit(task, menu);
}

static void DebugMenu_AdjustCamera_CreateTask(FieldSystem *sys, DebugMenu *menu)
{
    menu->data = Camera_GetFOV(menu->sys->camera);
    SysTask *camTask = SysTask_Start(Task_DebugMenu_AdjustCamera, menu, 0);
}

static void Task_DebugMenu_AdjustCamera(SysTask *task, void *data)
{
    DebugMenu *menu = (DebugMenu *)data;
    Camera *cam = menu->sys->camera;
    CameraAngle angle = { 0, 0, 0, 0 };

    if (gCoreSys.heldKeys & PAD_KEY_UP) {
        menu->data += 100;
    } else if (gCoreSys.heldKeys & PAD_KEY_DOWN) {
        menu->data -= 100;
    }

    if (gCoreSys.heldKeys & PAD_KEY_LEFT) {
        angle.y = -800;
        Camera_AdjustAngleAroundTarget(&angle, cam);
    } else if (gCoreSys.heldKeys & PAD_KEY_RIGHT) {
        angle.y = 800;
        Camera_AdjustAngleAroundTarget(&angle, cam);
    }

    Camera_SetFOV(menu->data, cam);

    if (gCoreSys.heldKeys & PAD_BUTTON_START) {
        SysTask_Done(task);
        // field end hold sequence
        sub_0203D140();
    }
}

// Smaller functionality

static void DebugFunction_ToggleCollision(SysTask *task, DebugMenu *menu)
{
    VarsFlags *varsFlags = SaveData_GetVarsFlags(menu->sys->saveData);

    if (VarsFlags_CheckFlag(varsFlags, DEBUG_FLAG_NO_COLLISION)) {
        VarsFlags_ClearFlag(varsFlags, DEBUG_FLAG_NO_COLLISION);
        Sound_PlayEffect(SEQ_SE_DP_PC_LOGOFF);
    } else {
        VarsFlags_SetFlag(varsFlags, DEBUG_FLAG_NO_COLLISION);
        Sound_PlayEffect(SEQ_SE_DP_PC_LOGIN);
    }

    DebugMenu_Exit(task, menu);
}

// shell function to run any arbitrary code you need
static void DebugFunction_ExecuteFunction(SysTask *task, DebugMenu *menu)
{
    // debug function start

    // debug function end
    DebugMenu_Exit(task, menu);
}
