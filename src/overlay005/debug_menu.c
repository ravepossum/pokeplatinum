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

#include "bag.h"
#include "camera.h"
#include "debug.h"
#include "field_map_change.h"
#include "field_system.h"
#include "item.h"
#include "message.h"
#include "move_table.h"
#include "party.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "string_gf.h"
#include "sys_task.h"
#include "system.h"
#include "text.h"
#include "unk_0203A7D8.h"
#include "unk_0203D1B8.h"
#include "unk_02092494.h"
#include "vars_flags.h"
#include "vram_transfer.h"

#include "res/text/bank/debug_menu.h"

// if these are lifted from bag.c into bag.h, these constants should be removed
#define BAG_MAX_QUANTITY_ITEM 999
#define BAG_MAX_QUANTITY_TMHM 99

// same here, if it does get one I guess
#define FLAGS_START 0

#define ITEM_ICON_CHAR 49407
#define ITEM_ICON_PLTT 49404

#define MAX_SUBMENU_DIGITS    5
#define MAX_SUBMENU_SPRITES   4
#define MAX_SUBMENU_RESOURCES 4
#define SUBMENU_VALUE_SET_MIN -1

static void DebugMenu_Free(DebugMenu *menu);
static void DebugMenu_Close(SysTask *task, DebugMenu *menu);
static void DebugMenu_ExitToField(SysTask *task, DebugMenu *menu);
static void Task_DebugMenu_ExitToField(SysTask *task, void *data);
static StringList *DebugMenu_CreateList(const DebugMenuItem *list, int count);
static void DebugMenu_CreateSubList(DebugMenu *menu, const DebugMenuItem *list, int count, PrintCallback printCB);
static void DebugMenu_CloseSubList(DebugMenu *menu);
static DebugMenu *DebugMenu_CreateMultichoice(FieldSystem *fieldSystem, const DebugMenuItem *list, int count, SysTaskFunc taskFunc);
static void Task_DebugMenu_HandleInput(SysTask *task, void *data);
static DebugSubMenu *DebugMenu_CreateSubMenu(DebugMenu *menu, enum DebugSubMenuType type, int initialValue);
static DebugListNode *DebugListNode_New(DebugMenu *menu, const DebugMenuItem *list);
static void *DebugListNode_Free(DebugListNode *node);
static void *DebugListNode_FreeAll(DebugListNode *node);

static void DebugSubMenu_Close(SysTask *task, DebugSubMenu *subMenu);
static void DebugSubMenu_Free(DebugSubMenu *subMenu);
static void DebugSubMenu_ExitToField(SysTask *task, DebugSubMenu *subMenu);
static void DebugSubMenu_ChangeType(DebugSubMenu *subMenu, enum DebugSubMenuType type, int initialValue);
static void Task_DebugSubMenu_HandleInput(SysTask *task, void *data);
static void DebugSubMenu_PrintString(DebugSubMenu *subMenu, u32 entryID, u32 x, u32 y, u32 delay, u32 color);
static void DebugSubMenu_InitSpriteResMan(DebugSubMenu *subMenu);

static void DebugFunction_Fly(SysTask *task, DebugMenu *menu);
static void DebugMenu_Fly_CreateTask(FieldSystem *fieldSystem);
static void Task_DebugMenu_Fly(SysTask *task, void *data);

static void DebugFunction_CreateMon(SysTask *task, DebugMenu *menu);
static void DebugFunction_EditMon(SysTask *task, DebugMenu *menu);

static void DebugFunction_AddItem(SysTask *task, DebugMenu *menu);
static void SubMenuChoice_AddItem(DebugSubMenu *subMenu);
static void SubMenuRender_AddItem(DebugSubMenu *subMenu);
static void AddItem_InitSprite(DebugSubMenu *subMenu);
static void AddItem_UpdateItemIcon(DebugSubMenu *subMenu);
static void SubMenuChoice_ItemQuantity(DebugSubMenu *subMenu);
static void SubMenuRender_ItemQuantity(DebugSubMenu *subMenu);

static void DebugFunction_AdjustCamera(SysTask *task, DebugMenu *menu);
static void DebugMenu_AdjustCamera_CreateTask(FieldSystem *fieldSystem, DebugMenu *menu);
static void Task_DebugMenu_AdjustCamera(SysTask *task, void *data);

static void DebugFunction_FlagVarList(SysTask *task, DebugMenu *menu);
static void FlagVarList_PrintCB(ListMenu *menu, u32 index, u8 onInit);
static void ToggleDebugFlag(VarsFlags *varsFlags, u16 flagID);
static void DebugFunction_ToggleCollision(SysTask *task, DebugMenu *menu);
static void DebugFunction_ToggleTrainerSee(SysTask *task, DebugMenu *menu);
static void DebugFunction_SetFlag(SysTask *task, DebugMenu *menu);
static void SubMenuChoice_SetFlag(DebugSubMenu *subMenu);
static void SubMenuRender_SetFlag(DebugSubMenu *subMenu);
static void DebugFunction_SetVar(SysTask *task, DebugMenu *menu);
static void SubMenuChoice_SelectVar(DebugSubMenu *subMenu);
static void SubMenuRender_SelectVar(DebugSubMenu *subMenu);
static void SubMenuChoice_VarValue(DebugSubMenu *subMenu);
static void SubMenuRender_VarValue(DebugSubMenu *subMenu);

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

static const WindowTemplate DebugMenu_SubMenu_WindowTemplate = {
    .bgLayer = BG_LAYER_MAIN_3,
    .tilemapLeft = 18,
    .tilemapTop = 1,
    .width = 12,
    .height = 7,
    .palette = 13,
    .baseTile = 229,
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
    .yOffset = 0,
    .textColorFg = DEBUG_COLOR_BLACK,
    .textColorBg = DEBUG_COLOR_WHITE,
    .textColorShadow = 2, // black shadow
    .letterSpacing = 0,
    .lineSpacing = 0,
    .pagerMode = PAGER_MODE_LEFT_RIGHT_PAD,
    .fontID = FONT_SYSTEM,
    .cursorType = 0,
    .parent = NULL,
};

static const DebugMenuItem sMainMenuItems[DEBUG_MAIN_ITEM_COUNT] = {
    [DEBUG_ITEM_FLY] = {
        .function = DebugFunction_Fly,
        .name = DebugMenu_ItemName_Fly,
    },
    [DEBUG_ITEM_CREATE_MON] = {
        .function = DebugFunction_CreateMon,
        .name = DebugMenu_ItemName_CreateMon,
    },
    [DEBUG_ITEM_EDIT_MON] = {
        .function = DebugFunction_EditMon,
        .name = DebugMenu_ItemName_EditMon,
    },
    [DEBUG_ITEM_ADD_ITEM] = {
        .function = DebugFunction_AddItem,
        .name = DebugMenu_ItemName_AddItem,
    },
    [DEBUG_ITEM_FLAG_VAR_LIST] = {
        .function = DebugFunction_FlagVarList,
        .name = DebugMenu_ItemName_FlagVarList,
    },
    [DEBUG_ITEM_ADJUST_CAMERA] = {
        .function = DebugFunction_AdjustCamera,
        .name = DebugMenu_ItemName_AdjustCamera,
    },
    [DEBUG_ITEM_EXECUTE_FUNCTION] = {
        .function = DebugFunction_ExecuteFunction,
        .name = DebugMenu_ItemName_ExecuteFunction,
    },
};

static const DebugMenuItem sFlagVarItems[DEBUG_FLAG_VAR_ITEM_COUNT] = {
    [DEBUG_ITEM_TOGGLE_COLLISION] = {
        .function = DebugFunction_ToggleCollision,
        .name = DebugMenu_ItemName_ToggleCollision,
    },
    [DEBUG_ITEM_TOGGLE_TRAINER_SEE] = {
        .function = DebugFunction_ToggleTrainerSee,
        .name = DebugMenu_ItemName_ToggleTrainerSee,
    },
    [DEBUG_ITEM_SET_FLAG] = {
        .function = DebugFunction_SetFlag,
        .name = DebugMenu_ItemName_SetFlag,
    },
    [DEBUG_ITEM_SET_VAR] = {
        .function = DebugFunction_SetVar,
        .name = DebugMenu_ItemName_SetVar,
    },
};

static const DebugSubMenuConfig sSubMenuConfigs[DEBUG_SUBMENU_TYPE_COUNT] = {
    [DEBUG_SUBMENU_ADD_ITEM] = {
        .choiceFunc = SubMenuChoice_AddItem,
        .renderFunc = SubMenuRender_AddItem,
        .closeOnChoice = FALSE,
        .preserveSprite = FALSE,
        .min = ITEM_NONE + 1,
        .max = MAX_ITEMS - 1,
    },
    [DEBUG_SUBMENU_ITEM_QUANTITY] = {
        .choiceFunc = SubMenuChoice_ItemQuantity,
        .renderFunc = SubMenuRender_ItemQuantity,
        .closeOnChoice = TRUE,
        .preserveSprite = TRUE,
        .min = 1,
        .max = BAG_MAX_QUANTITY_ITEM,
    },
    [DEBUG_SUBMENU_SET_FLAG] = {
        .choiceFunc = SubMenuChoice_SetFlag,
        .renderFunc = SubMenuRender_SetFlag,
        .closeOnChoice = FALSE,
        .preserveSprite = FALSE,
        .min = FLAGS_START,
        .max = NUM_FLAGS - 1,
    },
    [DEBUG_SUBMENU_SELECT_VAR] = {
        .choiceFunc = SubMenuChoice_SelectVar,
        .renderFunc = SubMenuRender_SelectVar,
        .closeOnChoice = FALSE,
        .preserveSprite = FALSE,
        .min = VARS_START,
        .max = VARS_END - 1,
    },
    [DEBUG_SUBMENU_VAR_VALUE] = {
        .choiceFunc = SubMenuChoice_VarValue,
        .renderFunc = SubMenuRender_VarValue,
        .closeOnChoice = TRUE,
        .preserveSprite = FALSE,
        .min = 0,
        .max = 0xFFFF,
    },
};

void DebugMenu_Init(FieldSystem *fieldSystem)
{
    DebugMenu *menu = DebugMenu_CreateMultichoice(fieldSystem, sMainMenuItems, NELEMS(sMainMenuItems), NULL);
    FieldSystem_PauseProcessing();
}

static void DebugMenu_Free(DebugMenu *menu)
{
    Window_EraseStandardFrame(menu->window, TRUE);
    Window_ClearAndCopyToVRAM(menu->window);
    DebugListNode_FreeAll(menu->listNode);
    Window_Remove(menu->window);
    Heap_Free(menu->window);
}

static void DebugSubMenu_Free(DebugSubMenu *subMenu)
{

    if (subMenu->sprite != NULL) {
        Sprite_DeleteAndFreeResources(subMenu->sprite);
    }
    SpriteResourceManager_Cleanup(&subMenu->spriteResMan);

    MessageLoader_Free(subMenu->msgLoader);
    StringTemplate_Free(subMenu->template);

    Window_EraseStandardFrame(subMenu->window, TRUE);
    Window_ClearAndCopyToVRAM(subMenu->window);
    Window_Remove(subMenu->window);

    Heap_Free(subMenu->window);
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

static DebugListNode *DebugListNode_New(DebugMenu *menu, const DebugMenuItem *list)
{
    DebugListNode *node = Heap_Alloc(HEAP_ID_FIELD1, sizeof(DebugListNode));
    node->prev = NULL;
    node->items = list;
    node->debugMenu = menu;
    return node;
}

static void *DebugListNode_Free(DebugListNode *node)
{
    ListMenu_Free(node->listMenu, NULL, NULL);
    StringList_Free(node->stringList);
}

static void *DebugListNode_FreeAll(DebugListNode *node)
{
    DebugListNode *tmpNode;
    while (node != NULL) {
        tmpNode = node;
        node = node->prev;
        DebugListNode_Free(tmpNode);
        Heap_Free(tmpNode);
    }
}

static DebugMenu *DebugMenu_CreateMultichoice(FieldSystem *fieldSystem, const DebugMenuItem *list, int count, SysTaskFunc taskFunc)
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

    menu->listNode = DebugListNode_New(menu, list);
    menu->listNode->stringList = DebugMenu_CreateList(list, count);

    MI_CpuCopy8((void *)&DebugMenu_List_Header, (void *)&(menu->listNode->template), sizeof(ListMenuTemplate));
    menu->listNode->template.choices = menu->listNode->stringList;
    menu->listNode->template.window = menu->window;
    menu->listNode->template.count = count;

    menu->listNode->listMenu = ListMenu_New(&(menu->listNode->template), menu->listPos, menu->cursor, HEAP_ID_FIELD1);

    return menu;
}

static StringList *DebugMenu_CreateList(const DebugMenuItem *itemList, int count)
{
    StringList *stringList = StringList_New(count, HEAP_ID_FIELD1);
    MessageLoader *msgLoader = MessageLoader_Init(MSG_LOADER_PRELOAD_ENTIRE_BANK, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_DEBUG_MENU, HEAP_ID_FIELD1);

    for (int i = 0; i < count; i++) {
        StringList_AddFromMessageBank(stringList, msgLoader, itemList[i].name, i);
    }
    MessageLoader_Free(msgLoader);

    return stringList;
}

static void DebugMenu_CreateSubList(DebugMenu *menu, const DebugMenuItem *list, int count, PrintCallback printCB)
{
    Window_FillTilemap(menu->window, DEBUG_COLOR_WHITE);

    DebugListNode *newNode = DebugListNode_New(menu, list);
    newNode->prev = menu->listNode;
    newNode->stringList = DebugMenu_CreateList(sFlagVarItems, NELEMS(sFlagVarItems));

    MI_CpuCopy8((void *)&DebugMenu_List_Header, (void *)&(newNode->template), sizeof(ListMenuTemplate));
    newNode->template.choices = newNode->stringList;
    newNode->template.window = menu->window;
    newNode->template.count = count;
    newNode->template.parent = newNode;
    if (printCB != NULL) {
        newNode->template.printCallback = printCB;
    }

    newNode->listMenu = ListMenu_New(&(newNode->template), menu->listPos, menu->cursor, HEAP_ID_FIELD1);

    menu->listNode = newNode;
}

static void DebugMenu_CloseSubList(DebugMenu *menu)
{
    Window_FillTilemap(menu->window, DEBUG_COLOR_WHITE);

    DebugListNode *tmpNode = menu->listNode;
    menu->listNode = menu->listNode->prev;
    DebugListNode_Free(tmpNode);
    Heap_Free(tmpNode);

    ListMenu_Draw(menu->listNode->listMenu);
}

static void Task_DebugMenu_HandleInput(SysTask *task, void *data)
{
    DebugMenu *menu = (DebugMenu *)data;
    s32 choice = ListMenu_ProcessInput(menu->listNode->listMenu);

    if (JOY_NEW(PAD_BUTTON_A)) {
        Sound_PlayEffect(SEQ_SE_CONFIRM);
        DebugFunction debugFunc = menu->listNode->items[choice].function;
        debugFunc(task, data);
        return;
    }

    if (JOY_NEW(PAD_BUTTON_B)) {
        Sound_PlayEffect(SEQ_SE_CONFIRM);
        if (menu->listNode->prev == NULL) {
            DebugMenu_ExitToField(task, menu);
        } else {
            DebugMenu_CloseSubList(menu);
        }
        return;
    }
    
    if (JOY_NEW(PAD_KEY)) {
        Sound_PlayEffect(SEQ_SE_DP_SELECT78);
    }
}

static DebugSubMenu *DebugMenu_CreateSubMenu(DebugMenu *menu, enum DebugSubMenuType type, int initialValue)
{
    DebugSubMenu *subMenu = Heap_Alloc(HEAP_ID_FIELD1, sizeof(DebugSubMenu));
    FieldSystem *fieldSystem = menu->fieldSystem;

    if (subMenu == NULL) {
        return NULL;
    }

    MI_CpuClear8(subMenu, sizeof(DebugSubMenu));

    const DebugSubMenuConfig *config = &sSubMenuConfigs[type];
    subMenu->type = type;
    subMenu->debugMenu = menu;
    subMenu->msgLoader = MessageLoader_Init(MSG_LOADER_LOAD_ON_DEMAND, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_DEBUG_MENU, HEAP_ID_FIELD1);
    subMenu->template = StringTemplate_Default(HEAP_ID_FIELD1);
    subMenu->value = (initialValue == SUBMENU_VALUE_SET_MIN) ? config->min : initialValue;
    subMenu->sprite = NULL;

    SpriteResourceCapacities capacities = {
        .asStruct = {
            .charCapacity = MAX_SUBMENU_RESOURCES,
            .plttCapacity = MAX_SUBMENU_RESOURCES,
            .cellCapacity = MAX_SUBMENU_RESOURCES,
            .animCapacity = MAX_SUBMENU_RESOURCES,
            .mcellCapacity = 0,
            .manimCapacity = 0,
        },
    };

    SpriteResourceManager_SetCapacities(&subMenu->spriteResMan, &capacities, MAX_SUBMENU_SPRITES, HEAP_ID_FIELD2);
    GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, TRUE);

    SysTask *task = SysTask_Start(Task_DebugSubMenu_HandleInput, subMenu, 0);

    subMenu->window = Window_New(HEAP_ID_FIELD1, 1);
    Window_AddFromTemplate(fieldSystem->bgConfig, subMenu->window, &DebugMenu_SubMenu_WindowTemplate);
    Window_DrawStandardFrame(subMenu->window, TRUE, 660, 11);

    Window_FillTilemap(subMenu->window, DEBUG_COLOR_WHITE);
    config->renderFunc(subMenu);
    Window_CopyToVRAM(subMenu->window);

    return subMenu;
}

static const int sPowersOfTen[MAX_SUBMENU_DIGITS] = {
    1,
    10,
    100,
    1000,
    10000,
};

static void Task_DebugSubMenu_HandleInput(SysTask *task, void *data)
{
    DebugSubMenu *subMenu = (DebugSubMenu *)data;
    const DebugSubMenuConfig *config = &sSubMenuConfigs[subMenu->type];

    if (JOY_NEW(PAD_KEY_UP)) {
        subMenu->value += sPowersOfTen[subMenu->digits];
        if (subMenu->value > config->max) {
            subMenu->value = config->max;
        }
    } else if (JOY_NEW(PAD_KEY_DOWN)) {
        subMenu->value -= sPowersOfTen[subMenu->digits];
        if (subMenu->value < config->min) {
            subMenu->value = config->min;
        }
    } else if (JOY_NEW(PAD_KEY_LEFT)) {
        if (subMenu->digits > 0) {
            subMenu->digits--;
        }
    } else if (JOY_NEW(PAD_KEY_RIGHT)) {
        if (subMenu->digits < (MAX_SUBMENU_DIGITS - 1)) {
            subMenu->digits++;
        }
    } else if (JOY_NEW(PAD_BUTTON_A)) {
        if (config->choiceFunc != NULL) {
            config->choiceFunc(subMenu);
            if (config->closeOnChoice) {
                DebugSubMenu_ExitToField(task, subMenu);
            }
        }
    } else if (JOY_NEW(PAD_BUTTON_B)) {
        DebugSubMenu_Close(task, subMenu);
    }

    if (JOY_NEW(PAD_KEY)) {
        config->renderFunc(subMenu);
    }

    SpriteList_Update(subMenu->spriteResMan.spriteList);
}

static void DebugSubMenu_ChangeType(DebugSubMenu *subMenu, enum DebugSubMenuType type, int initialValue)
{
    // subMenu->data is explicitly preserved
    const DebugSubMenuConfig *config = &sSubMenuConfigs[type];
    subMenu->type = type;
    subMenu->value = (initialValue == SUBMENU_VALUE_SET_MIN) ? config->min : initialValue;
    subMenu->digits = 0;
    if (subMenu->sprite != NULL && !config->preserveSprite) {
        Sprite_DeleteAndFreeResources(subMenu->sprite);
        subMenu->sprite = NULL;
    }
    config->renderFunc(subMenu);
}

static void DebugSubMenu_PrintString(DebugSubMenu *subMenu, u32 entryID, u32 x, u32 y, u32 delay, u32 color)
{
    String *buf = MessageLoader_GetNewString(subMenu->msgLoader, entryID);
    String *bufExp = String_Init(128, HEAP_ID_FIELD1);
    StringTemplate_Format(subMenu->template, bufExp, buf);
    Text_AddPrinterWithParamsAndColor(subMenu->window, FONT_SYSTEM, bufExp, x, y, delay, color, NULL);
    String_Free(buf);
    String_Free(bufExp);
}

static void DebugSubMenu_Close(SysTask *task, DebugSubMenu *subMenu)
{
    SysTask_Start(Task_DebugMenu_HandleInput, subMenu->debugMenu, 0);
    DebugSubMenu_Free(subMenu);
    Heap_Free(subMenu);
    SysTask_Done(task);
}

static void DebugSubMenu_ExitToField(SysTask *task, DebugSubMenu *subMenu)
{
    DebugMenu_Free(subMenu->debugMenu);
    Heap_Free(subMenu->debugMenu);
    DebugSubMenu_Free(subMenu);
    Heap_Free(subMenu);
    SysTask_Done(task);
    FieldSystem_ResumeProcessing();
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
    DebugMonMenu_New(menu->fieldSystem, DEBUG_MON_MENU_MODE_CREATE);
}

static void DebugFunction_EditMon(SysTask *task, DebugMenu *menu)
{
    DebugMenu_Close(task, menu);
    DebugMonMenu_New(menu->fieldSystem, DEBUG_MON_MENU_MODE_EDIT);
}

// Add Item

static void DebugFunction_AddItem(SysTask *task, DebugMenu *menu)
{
    DebugMenu_CreateSubMenu(menu, DEBUG_SUBMENU_ADD_ITEM, SUBMENU_VALUE_SET_MIN);
    SysTask_Done(task);
}

static void SubMenuChoice_AddItem(DebugSubMenu *subMenu)
{
    subMenu->data[0] = subMenu->value;
    DebugSubMenu_ChangeType(subMenu, DEBUG_SUBMENU_ITEM_QUANTITY, SUBMENU_VALUE_SET_MIN);
}

static void SubMenuRender_AddItem(DebugSubMenu *subMenu)
{
    if (subMenu->sprite == NULL) {
        AddItem_InitSprite(subMenu);
    }

    Window_FillTilemap(subMenu->window, DEBUG_COLOR_WHITE);
    StringTemplate_SetNumber(subMenu->template, 0, subMenu->value, MAX_SUBMENU_DIGITS, PADDING_MODE_ZEROES, CHARSET_MODE_EN);
    StringTemplate_SetItemName(subMenu->template, 1, subMenu->value);
    StringTemplate_SetNumber(subMenu->template, 2, sPowersOfTen[subMenu->digits], MAX_SUBMENU_DIGITS, PADDING_MODE_NONE, CHARSET_MODE_EN);
    DebugSubMenu_PrintString(subMenu, DebugSubMenu_Template_AddItem, 0, 0, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);

    AddItem_UpdateItemIcon(subMenu);
}

static void AddItem_InitSprite(DebugSubMenu *subMenu)
{
    NARC *narc = NARC_ctor(NARC_INDEX_ITEMTOOL__ITEMDATA__ITEM_ICON, HEAP_ID_FIELD2);
    SpriteResourceManager_LoadTiles(&subMenu->spriteResMan, narc, Item_FileID(ITEM_NONE, ITEM_FILE_TYPE_ICON), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, ITEM_ICON_CHAR);
    SpriteResourceManager_LoadPalette(&subMenu->spriteResMan, narc, Item_FileID(ITEM_NONE, ITEM_FILE_TYPE_PALETTE), FALSE, PLTT_1, NNS_G2D_VRAM_TYPE_2DMAIN, ITEM_ICON_PLTT);
    SpriteResourceManager_LoadCell(&subMenu->spriteResMan, narc, Item_IconNCERFile(), FALSE, ITEM_ICON_CHAR);
    SpriteResourceManager_LoadAnimation(&subMenu->spriteResMan, narc, Item_IconNANRFile(), FALSE, ITEM_ICON_CHAR);
    NARC_dtor(narc);

    const SpriteTemplate itemIconTemplate = {
        .x = 230,
        .y = 57,
        .z = 0,
        .animIdx = 0,
        .priority = 0,
        .plttIdx = 0,
        .vramType = NNS_G2D_VRAM_TYPE_2DMAIN,
        .resources = { ITEM_ICON_CHAR, ITEM_ICON_PLTT, ITEM_ICON_CHAR, ITEM_ICON_CHAR, 0, 0 },
        .bgPriority = 0,
        .vramTransfer = FALSE,
    };

    subMenu->sprite = SpriteResourceManager_CreateManagedSprite(&subMenu->spriteResMan, &itemIconTemplate);
}

static void AddItem_UpdateItemIcon(DebugSubMenu *subMenu)
{
    SpriteResource *spriteRes = SpriteResourceCollection_Find(subMenu->spriteResMan.resourceCollections[SPRITE_RESOURCE_CHAR], ITEM_ICON_CHAR);
    SpriteResourceCollection_ModifyTiles(subMenu->spriteResMan.resourceCollections[SPRITE_RESOURCE_CHAR], spriteRes, NARC_INDEX_ITEMTOOL__ITEMDATA__ITEM_ICON, Item_FileID(subMenu->value, ITEM_FILE_TYPE_ICON), FALSE, HEAP_ID_FIELD2);
    SpriteTransfer_RetransferCharData(spriteRes);

    spriteRes = SpriteResourceCollection_Find(subMenu->spriteResMan.resourceCollections[SPRITE_RESOURCE_PLTT], ITEM_ICON_PLTT);
    SpriteResourceCollection_ModifyPalette(subMenu->spriteResMan.resourceCollections[SPRITE_RESOURCE_PLTT], spriteRes, NARC_INDEX_ITEMTOOL__ITEMDATA__ITEM_ICON, Item_FileID(subMenu->value, ITEM_FILE_TYPE_PALETTE), FALSE, HEAP_ID_FIELD2);
    SpriteTransfer_ReplacePlttData(spriteRes);
}

static void SubMenuChoice_ItemQuantity(DebugSubMenu *subMenu)
{
    Bag *bag = SaveData_GetBag(subMenu->debugMenu->fieldSystem->saveData);
    u32 item = subMenu->data[0];
    u32 quantity = subMenu->value;
    u32 actualMax = BAG_MAX_QUANTITY_ITEM;

    if (Item_TMHMNumber(item) != ITEM_NONE) {
        actualMax = BAG_MAX_QUANTITY_TMHM;
    }

    u32 currentQuantity = Bag_GetItemQuantity(bag, item, HEAP_ID_FIELD1);

    if ((currentQuantity + quantity) > actualMax) {
        Bag_TryAddItem(bag, item, actualMax - currentQuantity, HEAP_ID_FIELD1);
    } else {
        Bag_TryAddItem(bag, item, quantity, HEAP_ID_FIELD1);
    }
}

static void SubMenuRender_ItemQuantity(DebugSubMenu *subMenu)
{
    Window_FillTilemap(subMenu->window, DEBUG_COLOR_WHITE);
    StringTemplate_SetNumber(subMenu->template, 0, subMenu->value, MAX_SUBMENU_DIGITS, PADDING_MODE_ZEROES, CHARSET_MODE_EN);
    StringTemplate_SetNumber(subMenu->template, 1, sPowersOfTen[subMenu->digits], MAX_SUBMENU_DIGITS, PADDING_MODE_NONE, CHARSET_MODE_EN);
    DebugSubMenu_PrintString(subMenu, DebugSubMenu_Template_ItemQuantity, 0, 0, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);
}

// Flag var list section

static void DebugFunction_FlagVarList(SysTask *task, DebugMenu *menu)
{
    DebugMenu_CreateSubList(menu, sFlagVarItems, NELEMS(sFlagVarItems), FlagVarList_PrintCB);
}

static void FlagVarList_PrintCB(ListMenu *menu, u32 index, u8 onInit)
{
    DebugListNode *node = (DebugListNode *)ListMenu_GetAttribute(menu, LIST_MENU_PARENT);
    VarsFlags *varsFlags = SaveData_GetVarsFlags(node->debugMenu->fieldSystem->saveData);
    int textState = DEBUG_TEXT_STATE_DEFAULT;

    switch (index) {
    case DEBUG_ITEM_TOGGLE_COLLISION:
        textState = VarsFlags_CheckFlag(varsFlags, FLAG_DEBUG_NO_COLLISION);
        break;
    case DEBUG_ITEM_TOGGLE_TRAINER_SEE:
        textState = VarsFlags_CheckFlag(varsFlags, FLAG_DEBUG_NO_TRAINER_SEE);
        break;
    }

    switch (textState) {
    case DEBUG_TEXT_STATE_INACTIVE:
        ListMenu_SetAltTextColors(menu, 3, 15, 4);
        break;
    case DEBUG_TEXT_STATE_ACTIVE:
        ListMenu_SetAltTextColors(menu, 5, 15, 6);
        break;
    case DEBUG_TEXT_STATE_DEFAULT:
        ListMenu_SetAltTextColors(menu, 1, 15, 2);
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

// Flag toggles

static void ToggleDebugFlag(VarsFlags *varsFlags, u16 flagID)
{
    if (VarsFlags_CheckFlag(varsFlags, flagID)) {
        VarsFlags_ClearFlag(varsFlags, flagID);
        Sound_PlayEffect(SEQ_SE_DP_PC_LOGOFF);
    } else {
        VarsFlags_SetFlag(varsFlags, flagID);
        Sound_PlayEffect(SEQ_SE_DP_PC_LOGIN);
    }
}

static void DebugFunction_ToggleCollision(SysTask *task, DebugMenu *menu)
{
    ToggleDebugFlag(SaveData_GetVarsFlags(menu->fieldSystem->saveData), FLAG_DEBUG_NO_COLLISION);
    DebugMenu_ExitToField(task, menu);
}

static void DebugFunction_ToggleTrainerSee(SysTask *task, DebugMenu *menu)
{
    ToggleDebugFlag(SaveData_GetVarsFlags(menu->fieldSystem->saveData), FLAG_DEBUG_NO_TRAINER_SEE);
    DebugMenu_ExitToField(task, menu);
}

// Set Flag

static void DebugFunction_SetFlag(SysTask *task, DebugMenu *menu)
{
    DebugMenu_CreateSubMenu(menu, DEBUG_SUBMENU_SET_FLAG, SUBMENU_VALUE_SET_MIN);
    SysTask_Done(task);
}

static void SubMenuChoice_SetFlag(DebugSubMenu *subMenu)
{
    VarsFlags *varsFlags = SaveData_GetVarsFlags(subMenu->debugMenu->fieldSystem->saveData);
    if (VarsFlags_CheckFlag(varsFlags, subMenu->value)) {
        VarsFlags_ClearFlag(varsFlags, subMenu->value);
    } else {
        VarsFlags_SetFlag(varsFlags, subMenu->value);
    }
    sSubMenuConfigs[subMenu->type].renderFunc(subMenu);
}

static void SubMenuRender_SetFlag(DebugSubMenu *subMenu)
{
    Window_FillTilemap(subMenu->window, DEBUG_COLOR_WHITE);

    String *string;
    VarsFlags *varsFlags = SaveData_GetVarsFlags(subMenu->debugMenu->fieldSystem->saveData);
    if (VarsFlags_CheckFlag(varsFlags, subMenu->value)) {
        string = MessageLoader_GetNewString(subMenu->msgLoader, DebugMenu_Text_True);
    } else {
        string = MessageLoader_GetNewString(subMenu->msgLoader, DebugMenu_Text_False);
    }

    // this should probably display in hex instead but I can't be assed to do that right now
    StringTemplate_SetNumber(subMenu->template, 0, subMenu->value, MAX_SUBMENU_DIGITS, PADDING_MODE_ZEROES, CHARSET_MODE_EN);
    StringTemplate_SetString(subMenu->template, 1, string, 0, 0, 0);
    StringTemplate_SetNumber(subMenu->template, 2, sPowersOfTen[subMenu->digits], MAX_SUBMENU_DIGITS, PADDING_MODE_NONE, CHARSET_MODE_EN);
    DebugSubMenu_PrintString(subMenu, DebugSubMenu_Template_SetFlag, 0, 0, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);

    String_Free(string);
}

// Set Var

static void DebugFunction_SetVar(SysTask *task, DebugMenu *menu)
{
    DebugMenu_CreateSubMenu(menu, DEBUG_SUBMENU_SELECT_VAR, SUBMENU_VALUE_SET_MIN);
    SysTask_Done(task);
}

static void SubMenuChoice_SelectVar(DebugSubMenu *subMenu)
{
    VarsFlags *varsFlags = SaveData_GetVarsFlags(subMenu->debugMenu->fieldSystem->saveData);
    u16 *var = VarsFlags_GetVarAddress(varsFlags, subMenu->value);

    subMenu->data[0] = subMenu->value;
    DebugSubMenu_ChangeType(subMenu, DEBUG_SUBMENU_VAR_VALUE, *var);
}

static void SubMenuRender_SelectVar(DebugSubMenu *subMenu)
{
    Window_FillTilemap(subMenu->window, DEBUG_COLOR_WHITE);

    VarsFlags *varsFlags = SaveData_GetVarsFlags(subMenu->debugMenu->fieldSystem->saveData);
    u16 *var = VarsFlags_GetVarAddress(varsFlags, subMenu->value);

    // this should probably display in hex instead but I can't be assed to do that right now
    StringTemplate_SetNumber(subMenu->template, 0, subMenu->value, MAX_SUBMENU_DIGITS, PADDING_MODE_ZEROES, CHARSET_MODE_EN);
    StringTemplate_SetNumber(subMenu->template, 1, *var, MAX_SUBMENU_DIGITS, PADDING_MODE_ZEROES, CHARSET_MODE_EN);
    StringTemplate_SetNumber(subMenu->template, 2, sPowersOfTen[subMenu->digits], MAX_SUBMENU_DIGITS, PADDING_MODE_NONE, CHARSET_MODE_EN);
    DebugSubMenu_PrintString(subMenu, DebugSubMenu_Template_SelectVar, 0, 0, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);
}

static void SubMenuChoice_VarValue(DebugSubMenu *subMenu)
{
    u16 varIndex = subMenu->data[0];
    VarsFlags *varsFlags = SaveData_GetVarsFlags(subMenu->debugMenu->fieldSystem->saveData);
    u16 *var = VarsFlags_GetVarAddress(varsFlags, varIndex);
    *var = subMenu->value;
}

static void SubMenuRender_VarValue(DebugSubMenu *subMenu)
{
    u16 varIndex = subMenu->data[0];
    Window_FillTilemap(subMenu->window, DEBUG_COLOR_WHITE);

    StringTemplate_SetNumber(subMenu->template, 0, varIndex, MAX_SUBMENU_DIGITS, PADDING_MODE_ZEROES, CHARSET_MODE_EN);
    StringTemplate_SetNumber(subMenu->template, 1, subMenu->value, MAX_SUBMENU_DIGITS, PADDING_MODE_ZEROES, CHARSET_MODE_EN);
    StringTemplate_SetNumber(subMenu->template, 2, sPowersOfTen[subMenu->digits], MAX_SUBMENU_DIGITS, PADDING_MODE_NONE, CHARSET_MODE_EN);
    DebugSubMenu_PrintString(subMenu, DebugSubMenu_Template_VarValue, 0, 0, TEXT_SPEED_INSTANT, DEBUG_TEXT_BLACK);
}

// Execute function
// shell function to run any arbitrary code you need
static void DebugFunction_ExecuteFunction(SysTask *task, DebugMenu *menu)
{
    // debug function start

    // debug function end
    DebugMenu_ExitToField(task, menu);
}
