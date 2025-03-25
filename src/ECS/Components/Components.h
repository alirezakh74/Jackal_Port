#pragma once

#include <genesis.h>

typedef struct
{
    s32 x;
    s32 y;
} PositionComponent, VelocityComponent;

typedef struct
{
    PositionComponent Position;
    VelocityComponent Velocity;
} TransformComponent;


typedef struct
{
    Sprite* sprite;
} SpriteComponent;

