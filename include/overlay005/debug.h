#ifndef POKEPLATINUM_DEBUG_H
#define POKEPLATINUM_DEBUG_H

#include "field/field_system_decl.h"
#include "struct_decls/struct_0200112C_decl.h"
#include "struct_defs/struct_0205AA50.h"
#include "struct_decls/struct_02013A04_decl.h"
#include "sys_task_manager.h"

typedef struct DebugMenu {
	u8 data;
	u16 debugList;
	u16 cursor;
	BmpList *bmpList;
	FieldSystem *sys;
	Window *window;
	ResourceMetadata *menuList;
	void (*callback)(FieldSystem *);
} DebugMenu;

typedef struct DebugMenuItem {
	u32	index;	
	u32	function;
} DebugMenuItem;

typedef struct DebugFly {
	FieldSystem* sys;
	void* data;
	SysTask* taskCutIn;
	int	sequence;
	int	wipeFunc;
} DebugFly;

enum DebugItems {
	DEBUG_HEADER = 1,
	DEBUG_FLY
};

#define DEBUG_KEY 					PAD_BUTTON_R
#define DEBUG_MENU_DUMMY_FUNCTION 	0xFFFFFFFD

void DebugMenu_Init(FieldSystem *sys);

#endif // POKEPLATINUM_DEBUG_H