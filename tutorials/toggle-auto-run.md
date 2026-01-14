# Toggle Auto Run with B
*Author: RavePossum*

It's a pain to constantly hold B to run, so auto-run is a common ROMhack feature. Let's add a form of auto-run that you can toggle by pressing the B button.

## Edit PlayerData
First, we'll need to add a field for tracking auto-run to the `PlayerData` struct. 

Make the following change in `include/struct_defs/struct_0205EC34.h`:

```diff
typedef struct {
    u16 cyclingGear;
-   u16 runningShoes;
+   u16 runningShoes:15;
+   u16 autoRun:1;
    u32 form;
} PlayerData;
```
*Note: here we're purposefully reusing the top bit from the `runningShoes` field rather than adding a new field in order to preserve save file compatibility. The `PlayerData` struct is saved as a part of the saved field state.*

## Add setter and getter functions for auto-run
The `PlayerData` struct, like many structs in Platinum, is not globally exposed. We'll add a getter and setter function to allow accessing and modifying our new field.

Make the following changes in `include/player_avatar.h`:
```diff
BOOL PlayerAvatar_DistortionGravityChanged(PlayerAvatar *const playerAvatar);
BOOL PlayerAvatar_DistortionStateOnFloor(PlayerAvatar *const playerAvatar);
+ u16 PlayerData_GetAutoRun(PlayerData *playerData);
+ u16 PlayerData_SetAutoRun(PlayerData *playerData, u16 value);

#endif // POKEPLATINUM_PLAYER_AVATAR_H
```

And at the bottom of `src/player_avatar.c`:
```diff
        || state == AVATAR_DISTORTION_STATE_ACTIVE
        || state == AVATAR_DISTORTION_STATE_FLOOR;
}

+ u16 PlayerData_GetAutoRun(PlayerData *playerData)
+ {
+     return playerData->autoRun;
+ }
+ 
+ u16 PlayerData_SetAutoRun(PlayerData *playerData, u16 value)
+ {
+     playerData->autoRun = value;
+ }

```

## Toggle the new auto-run field
The auto-run field should be toggled on and off whenever the player presses B in the overworld, similar to how they can press it to run originally.

Make the following change in `src/overlay005/field_control.c`:
```diff
void FieldInput_Update(FieldInput *input, FieldSystem *fieldSystem, u16 pressedKeys, u16 heldKeys)

...

    int moveState = Player_MoveState(fieldSystem->playerAvatar);
    int avatarState = PlayerAvatar_MoveState(fieldSystem->playerAvatar);
    int playerDir = PlayerAvatar_GetDir(fieldSystem->playerAvatar);

    input->pressedKeys = pressedKeys;
    input->heldKeys = heldKeys;

+   PlayerData *playerData = PlayerAvatar_PlayerData(fieldSystem->playerAvatar);
+
+   if (pressedKeys & PAD_BUTTON_B && PlayerData_HasRunningShoes(playerData)) {
+       PlayerData_SetAutoRun(playerData, !PlayerData_GetAutoRun(playerData));
+   }

    if (moveState == PLAYER_MOVE_STATE_END || moveState == PLAYER_MOVE_STATE_NONE) {
        if (pressedKeys & PAD_BUTTON_X) {
            input->menu = TRUE;
```

## Check the auto-run field for movement
Finally, we'll need to replace the existing function that checks for the B button being held down with our own that will simply check if the auto-run field is set to `TRUE`.

Make this change in `src/unk_0205F180.c`:
```diff
int sub_020613FC(PlayerAvatar *playerAvatar, u16 pad)
{
-   if (pad & PAD_BUTTON_B) {
-       return 1;
-   }
-
-   return 0;
+   return PlayerData_GetAutoRun(PlayerAvatar_PlayerData(playerAvatar));
}
```
*Note: Now the `pad` parameter to `sub_020613FC()` will be unused, so you can remove it in the function definition, declaration, and all calls to the function. I've just left that out from the tutorial to reduce the overall footprint of the changes.*

After you've made all these changes, you should be able to simply press B once and then run as long as you please.