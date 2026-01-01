# Show EVs and IVs in Summary Screen
*Author: RavePossum*

It would be handy for the player if they could check their Pokemon's EVs and IVs directly from the summary screen. Let's allow the player to switch between stat types by pressing A on the skills page of the summary screen.

# Retrieve EV and IV information
First, we'll add some new members to the `PokemonSummaryMonData` struct and populate them with EV/IV information. Head over to `src/applications/pokemon_summary_screen/main.h`, and add the following in the aforementioned struct definition:

```diff
    u16 spAttack;
    u16 spDefense;
    u16 speed;

+   u16 evHP;
+   u16 evAttack;
+   u16 evDefense;
+   u16 evSpAttack;
+   u16 evSpDefense;
+   u16 evSpeed;
+
+   u16 ivHP;
+   u16 ivAttack;
+   u16 ivDefense;
+   u16 ivSpAttack;
+   u16 ivSpDefense;
+   u16 ivSpeed;
+
    u8 ability;
    u8 nature;

```

We'll also need a way for the summary screen to track which set of stats to show. Let's add a member to the `PokemonSummaryScreen` struct for this:

```diff
    u8 ribbonRow;
    u8 ribbonMax;
    u8 ribbonID;

+   u8 skillsState;
} PokemonSummaryScreen;

BOOL PokemonSummaryScreen_ShowContestData(SaveData *saveData);
```

While we're here, we'll also want to add an enumeration somewhere to represent the current state of the stats being shown on the skills page:


```diff
    MAX_CONDITION_RECT
};

+ enum SkillsState {
+   SKILLS_STATE_STATS = 0,
+   SKILLS_STATE_EVS,
+   SKILLS_STATE_IVS,
+   MAX_SKILLS_STATE,
+ };

#define SUMMARY_SUBSCREEN_BUTTON_NONE 0xFF
```

Finally, we'll need to actually retrieve the relevant info while the summary screen populates the data for the Pokemon in the `SetMonDataFromMon()` function in `src/applications/pokemon_summary_screen/main.c`.

```diff
    monData->ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    monData->nature = Pokemon_GetNature(mon);

+   monData->evHP = Pokemon_GetValue(mon, MON_DATA_HP_EV, NULL);
+   monData->evAttack = Pokemon_GetValue(mon, MON_DATA_ATK_EV, NULL);
+   monData->evDefense = Pokemon_GetValue(mon, MON_DATA_DEF_EV, NULL);
+   monData->evSpAttack = Pokemon_GetValue(mon, MON_DATA_SPATK_EV, NULL);
+   monData->evSpDefense = Pokemon_GetValue(mon, MON_DATA_SPDEF_EV, NULL);
+   monData->evSpeed = Pokemon_GetValue(mon, MON_DATA_SPEED_EV, NULL);
+
+   monData->ivHP = Pokemon_GetValue(mon, MON_DATA_HP_IV, NULL);
+   monData->ivAttack = Pokemon_GetValue(mon, MON_DATA_ATK_IV, NULL);
+   monData->ivDefense = Pokemon_GetValue(mon, MON_DATA_DEF_IV, NULL);
+   monData->ivSpAttack = Pokemon_GetValue(mon, MON_DATA_SPATK_IV, NULL);
+   monData->ivSpDefense = Pokemon_GetValue(mon, MON_DATA_SPDEF_IV, NULL);
+   monData->ivSpeed = Pokemon_GetValue(mon, MON_DATA_SPEED_IV, NULL);
+
    u16 i;
    u8 maxPP;
    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
```

# Add New Text Strings To Summary Screen Bank
In order for the player to know which stats are currently shown, we'll need to display their name next to the button prompt at the top of the screen. We'll need to add some relevant strings to the end of the text bank in `res/text/pokemon_summary_screen.json`:
```diff
    {
      "id": "PokemonSummary_Text_RibbonMaxNumber",
      "en_US": "{STRVAR_1 52, 0, 0}"
+   },
+   {
+     "id": "PokemonSummary_Text_EVs",
+     "en_US": "EVs"
+   },
+   {
+     "id": "PokemonSummary_Text_IVs",
+     "en_US": "IVs"
+   },
+   {
+     "id": "PokemonSummary_Text_Stats",
+     "en_US": "Stats"
+   }
  ]
}
```

# Show the A Button on the Skills Page
We'll tell the player they can press the A button to switch between skills type by showing it in the prompt at the top. We'll also want to go ahead and print the first default text string - "Stats". We'll just add a couple function calls to `DrawSkillsPageWindows()` in `src/applications/pokemon_summary_screen/window.c`:
```diff
    MessageLoader_Free(msgLoader);
    PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_ABILITY_DESCRIPTION], SUMMARY_TEXT_BLACK, ALIGN_LEFT);

+   PokemonSummaryScreen_ClearAndPrintButtonPrompt(summaryScreen, PokemonSummary_Text_Stats);
+   PokemonSummaryScreen_UpdateAButtonSprite(summaryScreen, &summaryScreen->staticWindows[SUMMARY_WINDOW_BUTTON_PROMPT]);

    Window_ScheduleCopyToVRAM(&summaryScreen->extraWindows[SUMMARY_WINDOW_HP]);
    Window_ScheduleCopyToVRAM(&summaryScreen->extraWindows[SUMMARY_WINDOW_ATTACK]);
    Window_ScheduleCopyToVRAM(&summaryScreen->extraWindows[SUMMARY_WINDOW_DEFENSE]);
```

# Reset Default States
We should also reset the skills state to the default any time the player changes pages or Pokemon.

In the `SetupPageFromSubscreenButton()` function in `src/applications/pokemon_summary_screen/main.c`:
```diff
        ClearMoveInfoWindows(summaryScreen);
    }

+   // reset skills state when switching pages
+   if (summaryScreen->page == SUMMARY_PAGE_SKILLS) {
+       summaryScreen->skillsState = SKILLS_STATE_STATS;
+   }

    PokemonSummaryScreen_RemoveExtraWindows(summaryScreen);
    summaryScreen->page = page;
    PokemonSummaryScreen_UpdateAButtonSprite(summaryScreen, NULL);
```

And in the `ChangeSummaryMon()` function in the same file:
```diff
    if (summaryScreen->page == SUMMARY_PAGE_INFO) {
        DrawExperienceProgressBar(summaryScreen);
    } else if (summaryScreen->page == SUMMARY_PAGE_SKILLS) {
+       // reset skills state when changing mons
+       summaryScreen->skillsState = SKILLS_STATE_STATS;
        DrawHealthBar(summaryScreen);
    } else if (summaryScreen->page == SUMMARY_PAGE_CONDITION) {
        PokemonSummaryScreen_InitMaxAndDeltaConditionRects(summaryScreen);
```

# Add a Function To Print Different Kinds of Stats
When the player presses the A button, we'll need to redraw all of the skills with the correct stat type. Let's add the following function that reprints the correct stats based on the current state anywhere in `src/applications/pokemon_summary_screen/window.c`:
```c
void PokemonSummaryScreen_PrintSkillsForCurrentState(PokemonSummaryScreen *summaryScreen)
{
    Window_FillTilemap(&summaryScreen->extraWindows[SUMMARY_WINDOW_HP], 0);
    Window_FillTilemap(&summaryScreen->extraWindows[SUMMARY_WINDOW_ATTACK], 0);
    Window_FillTilemap(&summaryScreen->extraWindows[SUMMARY_WINDOW_DEFENSE], 0);
    Window_FillTilemap(&summaryScreen->extraWindows[SUMMARY_WINDOW_SP_ATTACK], 0);
    Window_FillTilemap(&summaryScreen->extraWindows[SUMMARY_WINDOW_SP_DEFENSE], 0);
    Window_FillTilemap(&summaryScreen->extraWindows[SUMMARY_WINDOW_SPEED], 0);

    switch (summaryScreen->skillsState) {
    case SKILLS_STATE_EVS:
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateAttack, summaryScreen->monData.evHP, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_HP], SUMMARY_TEXT_BLACK, ALIGN_CENTER);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateAttack, summaryScreen->monData.evAttack, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_ATTACK], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateDefense, summaryScreen->monData.evDefense, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_DEFENSE], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateSpAttack, summaryScreen->monData.evSpAttack, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_SP_ATTACK], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateSpDefense, summaryScreen->monData.evSpDefense, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_SP_DEFENSE], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateSpeed, summaryScreen->monData.evSpeed, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_SPEED], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        break;
    case SKILLS_STATE_IVS:
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateAttack, summaryScreen->monData.ivHP, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_HP], SUMMARY_TEXT_BLACK, ALIGN_CENTER);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateAttack, summaryScreen->monData.ivAttack, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_ATTACK], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateDefense, summaryScreen->monData.ivDefense, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_DEFENSE], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateSpAttack, summaryScreen->monData.ivSpAttack, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_SP_ATTACK], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateSpDefense, summaryScreen->monData.ivSpDefense, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_SP_DEFENSE], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateSpeed, summaryScreen->monData.ivSpeed, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_SPEED], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        break;
    case SKILLS_STATE_STATS:
        u32 hpWindowWidth = Window_GetWidth(&summaryScreen->extraWindows[SUMMARY_WINDOW_HP]) * 8;

        PrintCurrentAndMaxInfo(summaryScreen, 0, PokemonSummary_Text_Slash, PokemonSummary_Text_TemplateCurrentHp, PokemonSummary_Text_TemplateMaxHp, summaryScreen->monData.curHP, summaryScreen->monData.maxHP, 3, hpWindowWidth / 2, 0);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateAttack, summaryScreen->monData.attack, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_ATTACK], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateDefense, summaryScreen->monData.defense, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_DEFENSE], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateSpAttack, summaryScreen->monData.spAttack, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_SP_ATTACK], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateSpDefense, summaryScreen->monData.spDefense, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_SP_DEFENSE], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        SetAndFormatNumberBuf(summaryScreen, PokemonSummary_Text_TemplateSpeed, summaryScreen->monData.speed, 3, PADDING_MODE_NONE);
        PrintStringToWindow(summaryScreen, &summaryScreen->extraWindows[SUMMARY_WINDOW_SPEED], SUMMARY_TEXT_BLACK, ALIGN_RIGHT);
        break;
    }

    Window_ScheduleCopyToVRAM(&summaryScreen->extraWindows[SUMMARY_WINDOW_HP]);
    Window_ScheduleCopyToVRAM(&summaryScreen->extraWindows[SUMMARY_WINDOW_ATTACK]);
    Window_ScheduleCopyToVRAM(&summaryScreen->extraWindows[SUMMARY_WINDOW_DEFENSE]);
    Window_ScheduleCopyToVRAM(&summaryScreen->extraWindows[SUMMARY_WINDOW_SP_ATTACK]);
    Window_ScheduleCopyToVRAM(&summaryScreen->extraWindows[SUMMARY_WINDOW_SP_DEFENSE]);
    Window_ScheduleCopyToVRAM(&summaryScreen->extraWindows[SUMMARY_WINDOW_SPEED]);
}
```
And don't forget to add a declaration for it in `src/applications/pokemon_summary_screen/window.h`:
```diff
void PokemonSummaryScreen_PrintContestMoveAttributes(PokemonSummaryScreen *summaryScreen, u32 move);
void PokemonSummaryScreen_ClearContestAttributeWindows(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintPoffinFeedMsg(PokemonSummaryScreen *summaryScreen, enum SummaryPoffinFeedMsg msg);
+ void PokemonSummaryScreen_PrintSkillsForCurrentState(PokemonSummaryScreen *summaryScreen);

#endif // POKEPLATINUM_POKEMON_SUMMARY_SCREEN_WINDOW_H
```

# Handle the Player Pressing A
Finally, we'll just need to add handling for when the player actually presses A. Add the following function somehwere in `src/applications/pokemon_summary_screen/main.c`

```c
static void CycleSkillsState(PokemonSummaryScreen *summaryScreen)
{
    summaryScreen->skillsState++;
    if (summaryScreen->skillsState == MAX_SKILLS_STATE) {
        summaryScreen->skillsState = SKILLS_STATE_STATS;
    }

    switch (summaryScreen->skillsState) {
    case SKILLS_STATE_EVS:
        PokemonSummaryScreen_ClearAndPrintButtonPrompt(summaryScreen, PokemonSummary_Text_EVs);
        break;
    case SKILLS_STATE_IVS:
        PokemonSummaryScreen_ClearAndPrintButtonPrompt(summaryScreen, PokemonSummary_Text_IVs);
        break;
    case SKILLS_STATE_STATS:
        PokemonSummaryScreen_ClearAndPrintButtonPrompt(summaryScreen, PokemonSummary_Text_Stats);
        break;
    }

    PokemonSummaryScreen_PrintSkillsForCurrentState(summaryScreen);
}
```

And add a forward declaration for it with the rest near the top of the file:

```diff
static u8 HideRibbonInfo(PokemonSummaryScreen *summaryScreen);
static void ChangeSelectedRibbon(PokemonSummaryScreen *summaryScreen, s8 delta);
static int TryFeedPoffin(PokemonSummaryScreen *summaryScreen);
+ static void CycleSkillsState(PokemonSummaryScreen *summaryScreen);

const ApplicationManagerTemplate gPokemonSummaryScreenApp = {
    PokemonSummaryScreen_Init,
```

Lastly, let's handle the player pressing the A button and call it in `HandleInput_Main()` in the same file:
```diff
    if (JOY_NEW(PAD_BUTTON_A)) {
        if (summaryScreen->data->mode == SUMMARY_MODE_FEED_POFFIN && summaryScreen->page == SUMMARY_MODE_SHOW_CONDITION_CHANGE) {
            Sound_PlayEffect(SEQ_SE_DP_DECIDE);
            return TryFeedPoffin(summaryScreen);
        }

...

        } else if (summaryScreen->page == SUMMARY_PAGE_EXIT) {
            Sound_PlayEffect(SEQ_SE_DP_DECIDE);
            summaryScreen->data->returnMode = SUMMARY_RETURN_CANCEL;
            return SUMMARY_STATE_TRANSITION_OUT;
+       } else if (summaryScreen->page == SUMMARY_PAGE_SKILLS) {
+           Sound_PlayEffect(SEQ_SE_DP_SYU01);
+           CycleSkillsState(summaryScreen);
+           return SUMMARY_STATE_HANDLE_INPUT;
        }
    }

    if (CheckSubscreenPressAndSetButton(summaryScreen) == TRUE) {
```


All finished! It should look like so:

![Example of fully implemented feature](summary-evs-ivs.mp4)