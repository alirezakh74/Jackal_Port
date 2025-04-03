#ifndef projectile_h
#define projectile_h

#include "genesis.h"

// Bullet constants
#define BULLET_SPEED FIX32(5.0)
#define MAX_BULLETS 8
#define FIRE_RATE 10            // frames between shots
#define BULLET_RANGE FIX32(100) // range that bullet could travel
#define END_FIRE_SPRITE_RATE 8 // frames for show last frame of bullet that hit ground or obstacle 
extern s16 Cooldown;

typedef enum
{
    BULLET = 0,
    GRENADE = 1,
    MISSILE = 2
} ProjectileType;

typedef struct
{
    ProjectileType type;
    Vect2D_f32 position;
    Vect2D_f32 velocity;
    fix32 start_y_pos;
    Sprite *sprite;
    u8 current_frame;
    s16 ended_travel_frames; // frames to show frame 1 of bullet sprite(end of bullet hit ground or obstacle)
    bool is_travel_ended;
    bool active;
} Projectile;

// Bullet
#pragma region Bullet
// variables
extern Projectile bullets[MAX_BULLETS];
extern s16 CurrentBulletIndex;

// functions
// Initialize bullets
void initBullets(ProjectileType type);
// Fire a bullet
void fireBullet(Vect2D_f32 ownerPosition);
// Update bullets
void updateBullets();
#pragma endregion

// Grenade
#pragma region Grenade
// variables
extern Projectile Grenades[MAX_BULLETS];
extern s16 CurrentGrenadeIndex;

//functions

#pragma endregion

#endif // projectile_h