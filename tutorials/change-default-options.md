# Changing Default Options
*Author: RavePossum*

You may wish to change the default settings assigned to the game's options when a new save file is created. Let's use the example of changing the game's default text speed to fast.

This involves a simple tweak in `src/game_options.c`:
```diff
void Options_Init(Options *options)
{
    MI_CpuFill8(options, 0, sizeof(Options));

-   options->textSpeed = OPTIONS_TEXT_SPEED_NORMAL;
+   options->textSpeed = OPTIONS_TEXT_SPEED_FAST;
    options->soundMode = OPTIONS_SOUND_MODE_STEREO;
    options->battleStyle = OPTIONS_BATTLE_STYLE_SHIFT;
    options->battleScene = OPTIONS_BATTLE_SCENE_ON;
    options->buttonMode = OPTIONS_BUTTON_MODE_NORMAL;
    options->frame = OPTIONS_FRAME_1;
}
```

The constants for each options' settings are defined in `include/constants/game_options.h`.