#include "projectile.h"
#include "resources.h"

s16 Cooldown = FIRE_RATE;

Projectile bullets[MAX_BULLETS];
s16 CurrentBulletIndex = 0;

void initBullets(ProjectileType type)
{
    switch (type)
    {
    case BULLET:
        for (u8 i = 0; i < MAX_BULLETS; i++)
        {
            bullets[i].current_frame = 0;
            bullets[i].is_travel_ended = FALSE;
            bullets[i].active = FALSE;
            PAL_setPalette(PAL3, bullet.palette->data, DMA);
            bullets[i].sprite = SPR_addSprite(&bullet, 0, 0, TILE_ATTR(PAL3, TRUE, 0, 0));
            SPR_setAnimationLoop(bullets[i].sprite, FALSE);
            SPR_setAutoAnimation(bullets[i].sprite, FALSE);
            SPR_setVisibility(bullets[i].sprite, HIDDEN);
        }
        break;

    case GRENADE:
        break;
    
    case MISSILE:
        break;
    
    default:
        break;
    }
}

void fireBullet(Vect2D_f32 ownerPosition)
{
    CurrentBulletIndex++;
    if (CurrentBulletIndex >= MAX_BULLETS)
    {
        CurrentBulletIndex = 0;
    }

    // for(u8 i = 0; i < MAX_BULLETS; i++) {
    if (!bullets[CurrentBulletIndex].active)
    {
        bullets[CurrentBulletIndex].is_travel_ended = FALSE;
        bullets[CurrentBulletIndex].position.x = ownerPosition.x + FIX32(7);
        bullets[CurrentBulletIndex].position.y = ownerPosition.y + FIX32(8);
        bullets[CurrentBulletIndex].start_y_pos = bullets[CurrentBulletIndex].position.y;

        // Set bullet direction (example: right direction)
        bullets[CurrentBulletIndex].velocity.x = FIX32(0);
        bullets[CurrentBulletIndex].velocity.y = -BULLET_SPEED;

        bullets[CurrentBulletIndex].active = TRUE;
        SPR_setPosition(bullets[CurrentBulletIndex].sprite,
                        fix32ToInt(bullets[CurrentBulletIndex].position.x),
                        fix32ToInt(bullets[CurrentBulletIndex].position.y));
        SPR_setVisibility(bullets[CurrentBulletIndex].sprite, VISIBLE);
        return;
    }
    // }
}

void updateBullets()
{
    for (u8 i = 0; i < MAX_BULLETS; i++)
    {
        if(bullets[i].current_frame == 1)
        {
            // add one frame to nded_travel_frames
            bullets[i].ended_travel_frames += 1;

            if(bullets[i].ended_travel_frames >= END_FIRE_SPRITE_RATE)
            {
                bullets[i].active = FALSE;
                SPR_setFrame(bullets[i].sprite, 0);
                SPR_setVisibility(bullets[i].sprite, HIDDEN);
                bullets[i].current_frame = 0;
                return;
            }
        }

        if (bullets[i].active)
        {
            // Update position
            if(!bullets[i].is_travel_ended)
            {
                bullets[i].position.x += bullets[i].velocity.x;
                bullets[i].position.y += bullets[i].velocity.y;
                // Update sprite position
                SPR_setPosition(bullets[i].sprite,
                    fix32ToInt(bullets[i].position.x),
                    fix32ToInt(bullets[i].position.y));
            }

            // Check range
            if ((bullets[i].start_y_pos - bullets[i].position.y) > BULLET_RANGE)
            {
                //bullets[i].active = FALSE;
                bullets[i].is_travel_ended = TRUE;
                //SPR_setVisibility(bullets[i].sprite, HIDDEN);
            }
            // Check screen boundaries
            if (bullets[i].position.x > FIX32(320) ||
                bullets[i].position.x < FIX32(0) ||
                bullets[i].position.y > FIX32(240) ||
                bullets[i].position.y < FIX32(0))
            {
                bullets[i].active = FALSE;
                SPR_setVisibility(bullets[i].sprite, HIDDEN);
            }

            if(bullets[i].is_travel_ended && (bullets[i].current_frame != 1))
            {
                //bullets[i].sprite = FALSE;
                bullets[i].current_frame = 1;
                SPR_setFrame(bullets[i].sprite, 1);
            }
        }
    }
}