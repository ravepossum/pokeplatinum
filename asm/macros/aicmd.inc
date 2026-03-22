    .equ FALSE, 0
    .equ TRUE, 1

    .macro IfRandomLessThan i:req, jump:req
    .long 0
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfRandomGreaterThan i:req, jump:req
    .long 1
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfRandomEqualTo i:req, jump:req
    .long 2
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfRandomNotEqualTo i:req, jump:req
    .long 3
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro AddToMoveScore i:req
    .long 4
    .long \i
    .endm

    .macro IfHPPercentLessThan battler:req, i:req, jump:req
    .long 5
    .long \battler
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHPPercentGreaterThan battler:req, i:req, jump:req
    .long 6
    .long \battler
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHPPercentEqualTo battler:req, i:req, jump:req
    .long 7
    .long \battler
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHPPercentNotEqualTo battler:req, i:req, jump:req
    .long 8
    .long \battler
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfStatus battler:req, status:req, jump:req
    .long 9
    .long \battler
    .long \status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfNotStatus battler:req, status:req, jump:req
    .long 10
    .long \battler
    .long \status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfVolatileStatus battler:req, volatile_status:req, jump:req
    .long 11
    .long \battler
    .long \volatile_status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfNotVolatileStatus battler:req, volatile_status:req, jump:req
    .long 12
    .long \battler
    .long \volatile_status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveEffect battler:req, move_effects:req, jump:req
    .long 13
    .long \battler
    .long \move_effects
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfNotMoveEffect battler:req, move_effects:req, jump:req
    .long 14
    .long \battler
    .long \move_effects
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfSideCondition battler:req, side_conditions:req, jump:req
    .long 15
    .long \battler
    .long \side_conditions
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfNotSideCondition battler:req, side_conditions:req, jump:req
    .long 16
    .long \battler
    .long \side_conditions
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedLessThan loaded_val:req, jump:req
    .long 17
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedGreaterThan loaded_val:req, jump:req
    .long 18
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedEqualTo loaded_val:req, jump:req
    .long 19
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedNotEqualTo loaded_val:req, jump:req
    .long 20
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedMask loaded_val:req, jump:req
    .long 21
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedNotMask loaded_val:req, jump:req
    .long 22
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveEqualTo move:req, jump:req
    .long 23
    .long \move
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveNotEqualTo move:req, jump:req
    .long 24
    .long \move
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedinTable table:req, jump:req
    .long 25
    .long (\table-.) / 4 - 2
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedNotinTable table:req, jump:req
    .long 26
    .long (\table-.) / 4 - 2
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfAttackerHasDamagingMoves jump:req
    .long 27
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfAttackerHasNoDamagingMoves jump:req
    .long 28
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadTurnCount
    .long 29
    .endm

    .macro LoadTypeFrom load_type_target:req
    .long 30
    .long \load_type_target
    .endm

    .macro LoadMovePower
    .long 31
    .endm

    .macro FlagMoveDamageScore bool:req
    .long 32
    .long \bool
    .endm

    .macro LoadBattlerPreviousMove battler:req
    .long 33
    .long \battler
    .endm

    .macro IfTempEqualTo loaded_val:req, jump:req
    .long 34
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfTempNotEqualTo loaded_val:req, jump:req
    .long 35
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfSpeedCompareEqualTo cmp:req, jump:req
    .long 36
    .long \cmp
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfSpeedCompareNotEqualTo cmp:req, jump:req
    .long 37
    .long \cmp
    .long (\jump-.) / 4 - 1
    .endm

    .macro CountAlivePartyBattlers battler:req
    .long 38
    .long \battler
    .endm

    .macro LoadCurrentMove
    .long 39
    .endm

    .macro LoadCurrentMoveEffect
    .long 40
    .endm

    .macro LoadBattlerAbility battler:req
    .long 41
    .long \battler
    .endm

    .macro CalcMaxEffectiveness
    .long 42
    .endm

    .macro IfMoveEffectivenessEquals modifier:req, jump:req
    .long 43
    .long \modifier
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfPartyMemberStatus battler:req, pokemon_status:req, jump:req
    .long 44
    .long \battler
    .long \pokemon_status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfPartyMemberNotStatus battler:req, pokemon_status:req, jump:req
    .long 45
    .long \battler
    .long \pokemon_status
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadCurrentWeather
    .long 46
    .endm

    .macro IfCurrentMoveEffectEqualTo battle_effect:req, jump:req
    .long 47
    .long \battle_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCurrentMoveEffectNotEqualTo battle_effect:req, jump:req
    .long 48
    .long \battle_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfStatStageLessThan battler:req, stat:req, i:req, jump:req
    .long 49
    .long \battler
    .long \stat
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfStatStageGreaterThan battler:req, stat:req, i:req, jump:req
    .long 50
    .long \battler
    .long \stat
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfStatStageEqualTo battler:req, stat:req, i:req, jump:req
    .long 51
    .long \battler
    .long \stat
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfStatStageNotEqualTo battler:req, stat:req, i:req, jump:req
    .long 52
    .long \battler
    .long \stat
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCurrentMoveKills roll_damage:req, jump:req
    .long 53
    .long \roll_damage
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCurrentMoveDoesNotKill roll_damage:req, jump:req
    .long 54
    .long \roll_damage
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveKnown battler:req, move:req, jump:req
    .long 55
    .long \battler
    .long \move
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveNotKnown battler:req, move:req, jump:req
    .long 56
    .long \battler
    .long \move
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveEffectKnown battler:req, battle_effect:req, jump:req
    .long 57
    .long \battler
    .long \battle_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveEffectNotKnown battler:req, battle_effect:req, jump:req
    .long 58
    .long \battler
    .long \battle_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerUnderEffect battler:req, check_effect:req, jump:req
    .long 59
    .long \battler
    .long \check_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCurrentMoveMatchesEffect check_effect:req, jump:req
    .long 60
    .long \check_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro Escape
    .long 61
    .endm

    .macro Dummy3E i:req
    .long 62
    .long \i
    .endm

    .macro Dummy3F
    .long 63
    .endm

    .macro LoadHeldItem battler:req
    .long 64
    .long \battler
    .endm

    .macro LoadHeldItemEffect battler:req
    .long 65
    .long \battler
    .endm

    .macro LoadGender battler:req
    .long 66
    .long \battler
    .endm

    .macro LoadIsFirstTurnInBattle battler:req
    .long 67
    .long \battler
    .endm

    .macro LoadStockpileCount battler:req
    .long 68
    .long \battler
    .endm

    .macro LoadBattleType
    .long 69
    .endm

    .macro LoadRecycleItem battler:req
    .long 70
    .long \battler
    .endm

    .macro LoadTypeOfLoadedMove
    .long 71
    .endm

    .macro LoadPowerOfLoadedMove
    .long 72
    .endm

    .macro LoadEffectOfLoadedMove
    .long 73
    .endm

    .macro LoadProtectChain battler:req
    .long 74
    .long \battler
    .endm

    .macro PushAndGoTo jump:req
    .long 75
    .long (\jump-.) / 4 - 1
    .endm

    .macro GoTo jump:req
    .long 76
    .long (\jump-.) / 4 - 1
    .endm

    .macro PopOrEnd
    .long 77
    .endm

    .macro IfLevel check_level:req, jump:req
    .long 78
    .long \check_level
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfTargetIsTaunted jump:req
    .long 79
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfTargetIsNotTaunted jump:req
    .long 80
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfTargetIsPartner jump:req
    .long 81
    .long (\jump-.) / 4 - 1
    .endm

    .macro FlagBattlerIsType battler:req, type:req
    .long 82
    .long \battler
    .long \type
    .endm

    .macro CheckBattlerAbility battler:req, ability:req
    .long 83
    .long \battler
    .long \ability
    .endm

    .macro IfActivatedFlashFire battler:req, jump:req
    .long 84
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHeldItemEqualTo battler:req, item:req, jump:req
    .long 85
    .long \battler
    .long \item
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfFieldConditionsMask field_conditions:req, jump:req
    .long 86
    .long \field_conditions
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadSpikesLayers battler:req, side_conditions:req
    .long 87
    .long \battler
    .long \side_conditions
    .endm

    .macro IfAnyPartyMemberIsWounded battler:req, jump:req
    .long 88
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfAnyPartyMemberUsedPP battler:req, jump:req
    .long 89
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadFlingPower battler:req
    .long 90
    .long \battler
    .endm

    .macro LoadCurrentMovePP
    .long 91
    .endm

    .macro IfCanUseLastResort battler:req, jump:req
    .long 92
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadCurrentMoveClass
    .long 93
    .endm

    .macro LoadDefenderLastUsedMoveClass
    .long 94
    .endm

    .macro LoadBattlerSpeedRank battler:req
    .long 95
    .long \battler
    .endm

    .macro LoadBattlerTurnCount battler:req
    .long 96
    .long \battler
    .endm

    .macro IfPartyMemberDealsMoreDamage roll_damage:req, jump:req
    .long 97
    .long \roll_damage
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHasSuperEffectiveMove jump:req
    .long 98
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerDealsMoreDamage battler:req, roll_damage:req, jump:req
    .long 99
    .long \battler
    .long \roll_damage
    .long (\jump-.) / 4 - 1
    .endm

    .macro SumPositiveStatStages battler:req
    .long 100
    .long \battler
    .endm

    .macro DiffStatStages battler:req, battle_stat:req
    .long 101
    .long \battler
    .long \battle_stat
    .endm

    .macro IfBattlerHasHigherStat battler:req, battle_stat:req, jump:req
    .long 102
    .long \battler
    .long \battle_stat
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerHasLowerStat battler:req, battle_stat:req, jump:req
    .long 103
    .long \battler
    .long \battle_stat
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerHasEqualStat battler:req, battle_stat:req, jump:req
    .long 104
    .long \battler
    .long \battle_stat
    .long (\jump-.) / 4 - 1
    .endm

    .macro CheckIfHighestDamageWithPartner roll_damage:req
    .long 105
    .long \roll_damage
    .endm

    .macro IfBattlerFainted battler:req, jump:req
    .long 106
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerNotFainted battler:req, jump:req
    .long 107
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadAbility battler:req
    .long 108
    .long \battler
    .endm

    .equ TABLE_END, 0xFFFFFFFF

    .macro TableEntry entry:req
    .long \entry
    .endm

    .macro LabelDistance dst:req, src:req
    .long (\dst-\src) / 4
    .endm
