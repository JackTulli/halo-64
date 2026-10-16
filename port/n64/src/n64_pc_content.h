#ifndef N64_PC_CONTENT_H
#define N64_PC_CONTENT_H

/* Reserved only in the PC converter's appended scenario rows. */
#define N64_PC_EQUIPMENT_FLAG 0x40000000u
#define N64_PC_VEHICLE_FLAG 0x4000u
#define N64_WEAPONS_PC_ADDITIVE 4

static inline int n64_pc_equipment_enabled(int weapons, int non_multiplayer)
{
    return !non_multiplayer && weapons == N64_WEAPONS_PC_ADDITIVE;
}
static inline int n64_pc_vehicle_enabled(int vehicles, int non_multiplayer)
{
    /* Existing ALL MAP and ALL + ROCKET choices; MAP DEFAULT stays authored. */
    return !non_multiplayer && (vehicles == 2 || vehicles == 3);
}
int n64_game_pc_equipment_enabled(void);
int n64_game_pc_vehicle_enabled(void);
#endif
