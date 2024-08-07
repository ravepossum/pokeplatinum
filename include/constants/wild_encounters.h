#ifndef POKEPLATIUM_CONSTANTS_WILD_ENCOUNTERS_H
#define POKEPLATIUM_CONSTANTS_WILD_ENCOUNTERS_H

#define MAX_GRASS_ENCOUNTERS     12
#define MAX_SWARM_ENCOUNTERS     2
#define MAX_TIMED_ENCOUNTERS     2
#define MAX_RADAR_ENCOUNTERS     4
#define MAX_DUAL_SLOT_ENCOUNTERS 2
#define MAX_WATER_ENCOUNTERS     5

enum ENCOUNTER_TYPE {
    ENCOUNTER_TYPE_GRASS = 0,
    ENCOUNTER_TYPE_SURF,
    ENCOUNTER_TYPE_FISHING
};

enum ENCOUNTER_FISHING_ROD_TYPE {
    FISHING_TYPE_OLD_ROD = 0,
    FISHING_TYPE_GOOD_ROD,
    FISHING_TYPE_SUPER_ROD
};

#endif // POKEPLATIUM_CONSTANTS_WILD_ENCOUNTERS_H
