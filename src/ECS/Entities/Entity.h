#pragma once

#include <genesis.h>
#include "../Components/Components.h"

typedef enum
{
    JEEP,
    TANK,
    HELICOPTER,
    AIRPLANE,
    COUNTER
} EntityType;

typedef struct
{
    u16 Id;
    EntityType Type;
    bool IsActive;
} Entity;

typedef struct
{
    Entity MyEntity;
    TransformComponent Transform;
} Jeep;

Jeep* CreateJeep()
{
    // return (Jeep){
    //     .MyEntity = JEEP,
    //     .Transform = 
    // };
}

