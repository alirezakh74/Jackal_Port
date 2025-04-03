#include "jeep.h"
#include "resources.h"

// allocate variables
Jeep *PlayerJeep2 = NULL;
DIR CurrentJeepDir = UP;
DIR TargetJeepDir = UP;
bool ShouldIncrease = TRUE;
u32 StartTime = 0;
u32 ElapsedTime = 0;

Jeep PlayerJeep = {
    .position = {100, 100},
    .velocity = {0, 0},
    .sprite = NULL // Will be set later
};

void init_jeep()
{
    PAL_setPalette(PAL2, jeep.palette->data, DMA);
    PlayerJeep.sprite = SPR_addSprite(&jeep,
                                      PlayerJeep.position.x,
                                      PlayerJeep.position.y,
                                      TILE_ATTR(PAL2, 0, 0, 0));
}

Jeep *create_jeep(s16 x, s16 y)
{
    PAL_setPalette(PAL2, jeep.palette->data, DMA);
    Jeep *new_jeep = (Jeep *)MEM_alloc(sizeof(Jeep));
    new_jeep->position.x = FIX32(x);
    new_jeep->position.y = FIX32(y);
    new_jeep->sprite = SPR_addSprite(&jeep, x, y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    return new_jeep;
}