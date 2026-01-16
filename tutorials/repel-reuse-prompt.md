# Add Repel Reuse Prompt
*Author: RavePossum*

The handy prompt that asks you if you want to use another repel after one wears off wasn't added until B2W2. Let's add it in Platinum for some convenience.

## Store last used repel
First things first, we'll need a way to store the last repel used by the player in order to reuse that same repel type when it wears off. We'll need to repurpose an unused variable for this:

`generated/vars_flags.txt`:
```diff
VAR_OBJ_GFX_ID_E
VAR_OBJ_GFX_ID_F
VAR_PLAYER_STARTER
- VAR_UNUSED_0x4031
+ VAR_LAST_USED_REPEL
VAR_UNK_0x4032
VAR_VS_SEEKER_BATTERY_LEVEL
VAR_VS_SEEKER_STEP_COUNT
```

`src/applications/bag/main.c`:
```diff
#include "touch_screen.h"
#include "trainer_info.h"
#include "unk_020393C8.h"
#include "unk_0208C098.h"
#include "vram_transfer.h"
+ #include "vars_flags.h"
+ #include "generated/vars_flags.h"

#include "res/graphics/bag/bag_graphics.naix.h"

...

static String *TryUseRepel(BagController *controller, u16 item)
{
    if (SpecialEncounter_RepelStepsEmpty(GetSpecialEncounter(controller)) == FALSE) {
        controller->selectedItemCount = 0;
        return MessageLoader_GetNewString(controller->bagStringsLoader, Bag_Text_RepelEffectsLinger);
    }

    u32 stepCount = Item_LoadParam(item, ITEM_PARAM_HOLD_EFFECT_PARAM, HEAP_ID_BAG);
    SetRepelSteps(controller, stepCount);
    controller->selectedItemCount = 1;
    Sound_PlayEffect(SEQ_SE_DP_CARD2);

+   u16 *lastUsedRepelVar = VarsFlags_GetVarAddress(SaveData_GetVarsFlags(controller->bagCtx->saveData), VAR_LAST_USED_REPEL);
+   *lastUsedRepelVar = item;
+
    return MessageLoader_GetNewString(controller->bagStringsLoader, Bag_Text_UsedRepel);
}
```

## Add a script command to reset the repel steps
We'll also need a brand new script command that can reset the repel steps for us in the middle of a script.

`include/scrcmd_item.h`:
```diff
BOOL ScrCmd_GetItemPocket(ScriptContext *ctx);
BOOL ScrCmd_Dummy081(ScriptContext *ctx);
BOOL ScrCmd_Dummy082(ScriptContext *ctx);
+ BOOL ScrCmd_SetRepelSteps(ScriptContext *ctx);

#endif // POKEPLATINUM_SCRCMD_ITEM_H
```

`src/scrcmd_item.c`:
```diff
#include "field_script_context.h"
#include "inlines.h"
#include "item.h"
#include "unk_0205DFC4.h"
+ #include "special_encounter.h"

BOOL ScrCmd_AddItem(ScriptContext *ctx)

...

BOOL ScrCmd_Dummy082(ScriptContext *ctx)
{
    return FALSE;
}

+ BOOL ScrCmd_SetRepelSteps(ScriptContext *ctx)
+ {
+    u16 item = ScriptContext_GetVar(ctx);
+    SaveData *saveData = ctx->fieldSystem->saveData;
+    u8 *repelSteps = SpecialEncounter_GetRepelSteps(SaveData_GetSpecialEncounters(saveData));
+ 
+    u32 stepCount = Item_LoadParam(item, ITEM_PARAM_HOLD_EFFECT_PARAM, HEAP_ID_FIELD2);
+    *repelSteps = stepCount;
+ 
+    return FALSE;
+ }
+

<end of file>
```

`src/scrcmd.c`:
```diff
const ScrCmdFunc Unk_020EAC58[] = {
...

    ScrCmd_BufferBallSealNamePlural,
    ScrCmd_CapitalizeFirstLetter,
    ScrCmd_BufferFloorNumber,
+    ScrCmd_SetRepelSteps,
};

const u32 Unk_020EAB80 = NELEMS(Unk_020EAC58);
```

## Add script macro that calls the new command
We'll need to add a scrcmd macro to invoke the new command in a script:

`asm/macros/scrcmd.inc`:
```diff
    .macro BufferFloorNumber templateArg, floor
    .short 839
    .byte \templateArg
    .byte \floor
    .endm

+   .macro SetRepelSteps itemVar
+   .short 840
+   .short \itemVar
+   .endm
+

    .macro GoToIfLt varID, valueOrVarID, offset
```
**Note: The number after the first `.short` (`840` here) MUST correspond with the script command's index in the script command table (`Unk_020EAC58[]` in the previous step). If you've added other script commands, the `840` should instead represent the current last script command's index + 1.**

## Modify the "repel wore off" common script
Next, we'll modify the common script that gets called when a repel wears off. We'll add in logic to check if the player has another repel of the same type, and if so, prompt them to see if they want to use another one.

```diff
_138C:
    PlayFanfare SEQ_SE_CONFIRM
    LockAll
+   CheckItem VAR_LAST_USED_REPEL, 1, VAR_RESULT
+   GoToIfEq VAR_RESULT, TRUE, CommonScript_UseRepelPrompt
    Message pl_msg_00000213_00079
    WaitABPress
    CloseMessage
    ReleaseAll
    End

+ CommonScript_UseRepelPrompt:
+   Message CommonScript_Text_UseAnotherRepel
+   ShowYesNoMenu VAR_RESULT
+   GoToIfEq VAR_RESULT, MENU_YES, CommonScript_UseRepel
+   CloseMessage
+   ReleaseAll
+   End
+
+ CommonScript_UseRepel:
+   CloseMessage
+   PlayFanfare SEQ_SE_DP_CARD2
+   SetRepelSteps VAR_LAST_USED_REPEL
+   RemoveItem VAR_LAST_USED_REPEL, 1, VAR_RESULT
+   BufferPlayerName 0
+   BufferItemName 1, VAR_LAST_USED_REPEL
+   Message CommonScript_Text_UsedRepel
+   WaitABPress
+   CloseMessage
+   ReleaseAll
+   End
+
_139D:
    PlayFanfare SEQ_SE_CONFIRM
```

## Add new text strings for our new messages
We'll finally just need a couple new text strings at the bottom of the common strings bank for our new messages:

`res/text/common_strings.json`:
```diff
        "It will be fixed if you enter the\n",
        "Hall of Fame again.\r"
      ]
+   },
+   {
+     "id": "CommonScript_Text_UseAnotherRepel",
+     "en_US": [
+       "Repel’s effect wore off...\n",
+       "Use another?\r"
+     ]
+   },
+   {
+     "id": "CommonScript_Text_UsedRepel",
+     "en_US": [
+       "{STRVAR_1 3, 0, 0} used the\n",
+       "{STRVAR_1 8, 1, 0}."
+     ]
    }
  ]
}
```

With all that in place, the game should now prompt you to use another repel of the same type if you have more in your bag.