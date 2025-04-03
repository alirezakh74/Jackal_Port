#ifndef jeep_h
#define jeep_h

#include "genesis.h"
#include "../globals.h"

// macros
// Use fixed-point for precise movement
#define JEEP_SPEED FIX32(1.0)         // Base speed
#define DIAGONAL_FACTOR FIX32(0.7071) // 1/sqrt(2)

// structs
typedef struct
{
    Vect2D_f32 position;
    Vect2D_f32 velocity;
    Sprite *sprite;
} Jeep;

// functions
void init_jeep();
Jeep *create_jeep(s16 x, s16 y);

// variables
extern Jeep *PlayerJeep2;
extern DIR CurrentJeepDir;
extern DIR TargetJeepDir;
extern bool ShouldIncrease;
extern u32 StartTime;
extern u32 ElapsedTime;

#endif // jeep_h