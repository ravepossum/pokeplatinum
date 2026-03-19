#ifndef POKEPLATINUM_PARTY_MENU_MAIN_H
#define POKEPLATINUM_PARTY_MENU_MAIN_H

#include "constants/heap.h"

#include "applications/party_menu/defs.h"

#include "overlay_manager.h"

#define PARTY_MENU_SELECTION_INELIGIBLE 0
#define PARTY_MENU_SELECTION_ELIGIBLE   1
#define PARTY_MENU_SELECTION_ENTERED    2

enum PartyMenuStates {
    PARTY_MENU_STATE_0,
    PARTY_MENU_STATE_1,
    PARTY_MENU_STATE_2,
    PARTY_MENU_STATE_3,
    PARTY_MENU_STATE_4,
    PARTY_MENU_STATE_5,
    PARTY_MENU_STATE_6,
    PARTY_MENU_STATE_7,
    PARTY_MENU_STATE_8,
    PARTY_MENU_STATE_9,
    PARTY_MENU_STATE_10,
    PARTY_MENU_STATE_11,
    PARTY_MENU_STATE_12,
    PARTY_MENU_STATE_13,
    PARTY_MENU_STATE_14,
    PARTY_MENU_STATE_15,
    PARTY_MENU_STATE_16,
    PARTY_MENU_STATE_17,
    PARTY_MENU_STATE_18,
    PARTY_MENU_STATE_19,
    PARTY_MENU_STATE_20,
    PARTY_MENU_STATE_21,
    PARTY_MENU_STATE_22,
    PARTY_MENU_STATE_23,
    PARTY_MENU_STATE_24,
    PARTY_MENU_STATE_25,
    PARTY_MENU_STATE_26,
    PARTY_MENU_STATE_27,
    PARTY_MENU_STATE_28,
    PARTY_MENU_STATE_29,
    PARTY_MENU_STATE_30,
    PARTY_MENU_STATE_31,
    PARTY_MENU_STATE_32,
    PARTY_MENU_STATE_33
};

typedef void (*PartyMenuAction)(PartyMenuApplication *application, int *partyMenuState);

extern const ApplicationManagerTemplate gPokemonPartyAppTemplate;

void PartyMenu_UpdateFormChangeGraphicsMode(PartyMenuApplication *application, BOOL isTeardown);
u8 sub_0207EF04(PartyMenuApplication *application, u8 param1);
u8 PartyMenu_LoadMember(PartyMenuApplication *application, u8 slot);
const u16 *sub_0207F248(PartyMenuApplication *application);
void PartyMenu_UpdateSlotPalette(PartyMenuApplication *application, u8 slot);
void sub_0207FD68(PartyMenuApplication *application, u8 partySlot);
u8 PartyMenu_CheckEligibility(PartyMenuApplication *application, u8 slot);
u8 PartyMenu_CheckBattleHallEligibility(PartyMenuApplication *application, u8 slot);
u8 PartyMenu_CheckBattleCastleEligibility(PartyMenuApplication *application, u8 slot);
u8 PartyMenu_GetMemberPanelAnim(u8 menuType, u8 slot);
u8 GetElementIndex(u16 param0);
u32 sub_02081930(void);
u32 sub_02081934(void);
u32 sub_02081938(void);
u32 sub_0208193C(void);
void PartyMenu_LoadMemberPanelTilemaps(enum HeapID heapID, u16 *leadMember, u16 *backMembers, u16 *noMember);
void PartyMenu_SetupFormChangeAnim(PartyMenuApplication *application);
void PartyMenu_TeardownFormChangeAnim(PartyMenuApplication *application);

#endif // POKEPLATINUM_PARTY_MENU_MAIN_H
