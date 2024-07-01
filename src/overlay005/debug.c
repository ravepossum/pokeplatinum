#include "overlay005/debug.h"
#include "overlay005/debug_mon_menu.h"
#include "strbuf.h"
#include "message.h"
#include "field_system.h"
#include "overlay084/struct_ov84_02240FA8.h"
#include "overlay061/struct_ov61_0222C884.h"
#include "struct_defs/struct_0203D8AC.h"
#include "constants/heap.h"
#include "constants/charcode.h"
#include "constants/map_object.h"
#include "sys_task.h"
#include "unk_02013A04.h"
#include "unk_02018340.h"
#include "unk_0200112C.h"
#include "unk_020021B0.h"
#include "unk_0200F174.h"
#include "unk_0203D1B8.h"
#include "unk_0206B70C.h"
#include "unk_020508D4.h"
#include "unk_0200DA60.h"
#include "unk_0201D670.h"
#include "party.h"
#include "field_map_change.h"
#include "overlay006/ov6_02243258.h"
#include "field/field_system.h"
#include "core_sys.h"
#include "constants/narc.h"
#include "constants/pokemon.h"
#include "consts/pokemon.h"
#include "consts/species.h"
#include "consts/moves.h"
#include "consts/items.h"
#include "struct_defs/struct_02090800.h"
#include "unk_02092494.h"
#include "move_table.h"

static void DebugMenu_Free(void *data);
static void CB_DebugMenu_Exit(SysTask *task, void *data);
static void DebugMenu_Exit(SysTask *task, void *data);
static void Task_DebugMenu_Exit (SysTask *task, void *data);
static void	DebugMenu_List_Init(DebugMenu *menu, const DebugMenuItem *list);
static ResourceMetadata* DebugMenu_CreateList(int arcID, const DebugMenuItem *list, int count);
static ResourceMetadata* DebugMenu_CreateSubList(int count);
static void DebugMenu_SetWindow(BGL *bgl);
static DebugMenu* DebugMenu_CreateMultichoice(FieldSystem *sys, int arcID, const DebugMenuItem *list, int count, SysTaskFunc taskFunc);
static void Task_DebugMenu_HandleInput(SysTask *task, void *data);
static void	CB_DebugMenu_ListHeader(BmpList* bmpList, u32 param, u8 y);


static void DebugMenu_Fly(SysTask *task, void *data);
static void DebugMenu_Fly_CreateTask(FieldSystem *sys);
static void Task_DebugMenu_Fly(SysTask *task, void *data);

static void DebugMenu_GiveMon(SysTask *task, void *data);
static void DebugMenu_GiveMon_CreateTask(FieldSystem *sys);
static void Task_DebugMenu_GiveMon(SysTask *task, void *data);

static const UnkStruct_ov61_0222C884 DebugMenu_List_WindowTemplate = {
	3, // BG3
	0, // x
	5, // y
	13, // width
	18, // height
	13, // palette
	8 // baseblock or something?
};

static const UnkStruct_ov84_02240FA8 DebugMenu_List_Header = {
	NULL,				
	NULL,				
	NULL,	
	NULL,				
	0,					
	8,					
	2,					
	12,					
	1,					
	8,		
	// font colors			
	11, // white		
	15, // black		
	2, // black shadow	
	0,					
	0,					
	1, // multichoice LR scroll
	0, // system font			
	0,					
	NULL,				
};

static const DebugMenuItem DebugMenu_ItemList[] = {
	{DEBUG_HEADER,		DEBUG_MENU_DUMMY_FUNCTION},
	{DEBUG_FLY,			(u32)DebugMenu_Fly},
	{DEBUG_GIVE_MON,	(u32)DebugMenu_GiveMon},
};

void DebugMenu_Init (FieldSystem *sys)
{
	DebugMenu *menu = DebugMenu_CreateMultichoice(sys, DEBUG_MENU_MESSAGE_BANK, DebugMenu_ItemList, NELEMS(DebugMenu_ItemList), NULL);
	menu->callback = NULL;

	// field system hold sequence
	sub_0203D128();
}

static void DebugMenu_Free (void* data)
{
	DebugMenu *menu = (DebugMenu*)data;

	// window off
	sub_0201ACF4(menu->window);

	// delete menu list?
	sub_02013A3C(menu->menuList);

	// menu list exit?
	sub_02001384(menu->bmpList, &(menu->debugList), &(menu->cursor));

	BGL_DeleteWindow(menu->window);
	Heap_FreeToHeap(menu->window);
}

static void CB_DebugMenu_Exit (SysTask *task, void *data)
{
	DebugMenu_Free(data);
	Heap_FreeToHeap(data);
	SysTask_Done(task);
}

static void DebugMenu_Exit (SysTask *task, void *data)
{
	DebugMenu_Free(data);
	SysTask_SetCallback(task, Task_DebugMenu_Exit);
}

static void Task_DebugMenu_Exit (SysTask *task, void *data)
{
	Heap_FreeToHeap(data);
	SysTask_Done(task);
	// hold sequence end?
	sub_0203D140();
}

static DebugMenu* DebugMenu_CreateMultichoice (FieldSystem *sys, int arcID, const DebugMenuItem *list, int count, SysTaskFunc taskFunc)
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
	DebugMenu_SetWindow(menu->sys->unk_08);

	// allocate window?
	menu->window = sub_0201A778(HEAP_ID_FIELD, 1);

	// some kind of add window - from template?
	sub_0201A8D4(menu->sys->unk_08, menu->window, &DebugMenu_List_WindowTemplate);

	if (list != NULL) {
		menu->menuList = DebugMenu_CreateList(arcID, list, count);
	}

	UnkStruct_ov84_02240FA8 listHeader = DebugMenu_List_Header;
	listHeader.unk_00 = menu->menuList;
	listHeader.unk_0C = menu->window;
	listHeader.unk_10 = count;

	// set list
	menu->bmpList = sub_0200112C(&listHeader, menu->debugList, menu->cursor, HEAP_ID_FIELD);

	return menu;
}

static ResourceMetadata* DebugMenu_CreateList (int arcID, const DebugMenuItem *list, int count)
{
	// create list
	ResourceMetadata *menuList = sub_02013A04(count, HEAP_ID_FIELD);
	MessageLoader *msgLoader = MessageLoader_Init(MESSAGE_LOADER_BANK_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, arcID, HEAP_ID_FIELD);

	for (int i = 0; i < count; i++) {
		// add string from narc?
		sub_02013A4C(menuList, msgLoader, list[i].index, list[i].function);
	}
	MessageLoader_Free(msgLoader);

	return menuList;
}

static void	DebugMenu_List_Init (DebugMenu *menu, const DebugMenuItem *list)
{
	if (menu->debugList == 0 
		&& menu->cursor == 0
		&& list != NULL
		&& list[menu->debugList].function == DEBUG_MENU_DUMMY_FUNCTION) {

		menu->debugList = 0;
		menu->cursor = 1;
	}
}

static void DebugMenu_SetWindow (BGL *bgl)
{
	// set window gfx or something?
	sub_0200DAA4(bgl, 3, 473, 11, 0, HEAP_ID_FIELD);
}

static void Task_DebugMenu_HandleInput (SysTask* task, void* data)
{
	DebugMenu *menu = (DebugMenu*)data;
	// multichoice main?
	s32 choice = sub_02001288(menu->bmpList);
	SysTaskFunc	taskFunc;

	if (gCoreSys.pressedKeys & PAD_BUTTON_A) {
		if (choice) {
			taskFunc = (SysTaskFunc)choice;
			taskFunc(task, data);
		}
		return;
	}

	if (gCoreSys.pressedKeys & PAD_BUTTON_B) {
		if (menu->callback != NULL) {
			void (*taskFunc)(FieldSystem*) = menu->callback;
			CB_DebugMenu_Exit(task, data);
			taskFunc(menu->sys);
		} else {
			DebugMenu_Exit(task, data);
		}
	}
}

// Fly section

static void DebugMenu_Fly (SysTask *task, void *data)
{
	DebugMenu *menu = (DebugMenu*)data;
	DebugMenu_Fly_CreateTask(menu->sys);	
	CB_DebugMenu_Exit(task, data);
}

static void DebugMenu_Fly_CreateTask (FieldSystem *sys)
{
	DebugFly *fly = Heap_AllocFromHeap(HEAP_ID_APPLICATION, sizeof(DebugFly));
	MI_CpuClear8(fly, sizeof(DebugFly));
	fly->sys = sys;
	SysTask_Start(Task_DebugMenu_Fly, fly, 0);
}

static void Task_DebugMenu_Fly (SysTask *task, void *data)
{
	DebugFly *fly = (DebugFly*)data;
	UnkStruct_0203D8AC* map;

	switch(fly->sequence) {
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

		map = (UnkStruct_0203D8AC*)fly->data;
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
		if(!ScreenWipe_Done()){
			return;
		}
		
		map = (UnkStruct_0203D8AC*)fly->data;
		if(!map->unk_10){
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
		
		map = (UnkStruct_0203D8AC*)fly->data;

		// map change via fly
		sub_02053AB4(fly->sys, map->unk_1C, (-1), map->unk_14*32+16, map->unk_18*32+16, DIR_SOUTH);
		break;
	case 6:
		Heap_FreeToHeap(fly->data);
		Heap_FreeToHeap(fly);
		SysTask_Done(task);
		return;
	}

	fly->sequence++;
}

// GiveMon section

static void DebugMenu_GiveMon (SysTask *task, void *data)
{
	DebugMenu *menu = (DebugMenu*)data;
	CB_DebugMenu_Exit(task, data);
	DebugMenu_GiveMon_CreateTask(menu->sys);
}

static void DebugMenu_GiveMon_CreateTask (FieldSystem *sys)
{
	DebugMonMenu *monMenu = (DebugMonMenu*) SysTask_GetParam(SysTask_StartAndAllocateParam(Task_DebugMenu_GiveMon, sizeof(DebugMonMenu), 0, HEAP_ID_APPLICATION));
	monMenu->sys = sys;
	monMenu->state = 0;
	monMenu->mode = DEBUG_MON_MENU_MODE_GIVE;
	monMenu->msgLoader = MessageLoader_Init(MESSAGE_LOADER_BANK_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, DEBUG_MON_MENU_MESSAGE_BANK, HEAP_ID_APPLICATION);
	monMenu->strTemplate = StringTemplate_Default(HEAP_ID_APPLICATION);
	// create cursor?
	monMenu->cursor = sub_020149F0(HEAP_ID_APPLICATION);

	// get BGL?
	BGL *bgl = sub_0203D170(sys);

	BGL_AddWindow(bgl, &monMenu->titleWindow, 3, 1, 1, 30, 4, 13, 1);
	BGL_AddWindow(bgl, &monMenu->mainWindow, 3, 1, 7, 30, 16, 13, 1 + 30 * 4);

	// set window gfx or something
	sub_0200DAA4(bgl, 3, 1+30*4+30*18, 14, 0, HEAP_ID_APPLICATION);

	Window_Show(&monMenu->titleWindow, 1, 1 + 30 * 4 + 30 * 18, 14);
	Window_Show(&monMenu->mainWindow, 1, 1 + 30 * 4 + 30 * 18, 14);

	DebugMonMenu_Init(monMenu);
}

static void Task_DebugMenu_GiveMon(SysTask *task, void *data)
{
	DebugMonMenu *monMenu = (DebugMonMenu*)data;

	switch(monMenu->state) {
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
		if(gCoreSys.pressedKeys & (PAD_BUTTON_X | PAD_BUTTON_Y)) {
			monMenu->state = 0;
		}
		break;
	}
}
