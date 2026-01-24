#ifndef POKEPLATINUM_GAME_START_H
#define POKEPLATINUM_GAME_START_H

#include "overlay_manager.h"
#include "savedata.h"

extern const ApplicationManagerTemplate gGameStartLoadSaveAppTemplate;
extern const ApplicationManagerTemplate gGameStartNewSaveAppTemplate;
extern const ApplicationManagerTemplate gGameStartRowanIntroAppTemplate;

void InitializeNewSave(enum HeapID heapID, SaveData *saveData, BOOL setTrainerID);

#endif // POKEPLATINUM_GAME_START_H
