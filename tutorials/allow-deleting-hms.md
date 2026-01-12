# Allow Deleting HM Moves
*Author: RavePossum*

If we remove the checks that stop you from deleting HM moves, we can allow overwriting them at any time.

Doing so is relatively simple - just make the following changes in the relevant files:

`src/applications/pokemon_summary_screen/main.c`:
```diff
static int HandleInput_SelectMove(PokemonSummaryScreen *summaryScreen)

...

   if (JOY_NEW(PAD_BUTTON_A)) {
        Sound_PlayEffect(SEQ_SE_DP_DECIDE);

-       if (summaryScreen->cursor != LEARNED_MOVES_MAX) {
-           if (Item_IsHMMove(summaryScreen->monData.moves[summaryScreen->cursor]) == TRUE && summaryScreen->data->move != MOVE_NONE) {
-               Sprite_SetDrawFlag2(summaryScreen->sprites[SUMMARY_SPRITE_MOVE_CATEGORY_ICON], FALSE);
-               DrawEmptyHearts(summaryScreen);
-               PokemonSummaryScreen_PrintHMMovesCantBeForgotten(summaryScreen);
-               return SUMMARY_STATE_WAIT_HM_MSG_INPUT;
-           }
-       }

        summaryScreen->data->selectedMoveSlot = summaryScreen->cursor;
        summaryScreen->data->returnMode = SUMMARY_RETURN_SELECT;
        return SUMMARY_STATE_TRANSITION_OUT;
```

`src/battle_party.c`:
```diff
static u8 BattlePartyTask_ConfirmLearnMoveScreen(BattleParty *battleParty)

...

            ov13_02225FCC(battleParty, 29);
        }

-       if (CheckSelectedMoveIsHM(battleParty) == TRUE) {
-           BattlePartyText_PrintHMMovesCantBeForgottenText(battleParty);
-
-           if (battleParty->displayingContestStats == FALSE) {
-               ClearMoveStats(battleParty);
-           } else {
-               ClearMoveContestStats(battleParty);
-           }
-
-           battleParty->queuedState = TASK_STATE_CONFIRM_LEARN_MOVE_SCREEN;
-       } else {
-           battleParty->queuedState = TASK_STATE_EXIT;
-       }
+       battleParty->queuedState = TASK_STATE_EXIT;

        return TASK_STATE_SCREEN_TRANSITION;
    case BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS:
```
