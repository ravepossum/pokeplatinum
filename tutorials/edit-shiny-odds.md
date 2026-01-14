# Editing Shiny Odds
*Author: RavePossum*

Editing shiny odds is a simple tweak. The odds in modern Pokemon games are 1/4096 instead of Platinum's 1/8192, so let's tweak it to that value.

In `src/pokemon.c`, find and edit the `Pokemon_InlineIsPersonalityShiny()` function:
```diff
static inline BOOL Pokemon_InlineIsPersonalityShiny(u32 monOTID, u32 monPersonality)
{
-    return (((monOTID & 0xFFFF0000) >> 16) ^ (monOTID & 0xFFFF) ^ ((monPersonality & 0xFFFF0000) >> 16) ^ (monPersonality & 0xFFFF)) < 8;
+    return (((monOTID & 0xFFFF0000) >> 16) ^ (monOTID & 0xFFFF) ^ ((monPersonality & 0xFFFF0000) >> 16) ^ (monPersonality & 0xFFFF)) < 16;
}
```

The number on the right-hand side of the inequality (the 8 we're changing to a 16) divided by 65536 represents the shiny odds.

8/65536 is roughly 1/8192, and 16/65536 is roughly 1/4096.