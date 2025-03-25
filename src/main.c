#include <genesis.h>
#include <resources.h>

#define JEEP_SPEED 1

typedef struct {
    Vect2D_s16 position;
    Vect2D_s16 velocity;
    Sprite* sprite;
} Jeep;

Jeep PlayerJeep = {
    .position = { 100, 100 },
    .velocity = { 0, 0 },
    .sprite = NULL // Will be set later
};

void init_jeep() {
    PAL_setPalette(PAL2, car.palette->data, DMA);
    PlayerJeep.sprite = SPR_addSprite(&car, 
        PlayerJeep.position.x, 
        PlayerJeep.position.y,
        TILE_ATTR(PAL2, 0, 0, 0));
}

Jeep* create_jeep(s16 x, s16 y) {
    PAL_setPalette(PAL2, car.palette->data, DMA);
    Jeep* jeep = (Jeep*)MEM_alloc(sizeof(Jeep));
    jeep->position.x = x;
    jeep->position.y = y;
    jeep->velocity.x = 0;
    jeep->velocity.y = 0;
    jeep->sprite = SPR_addSprite(&car, x, y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return jeep;
}

Jeep* PlayerJeep2 = NULL;

static void HandleInputs()
{
    u16 joy_state = JOY_readJoypad(JOY_1);

    // Horizontal movement
    if(joy_state & BUTTON_LEFT) {
        KLog("LEFT CLICKED");
        PlayerJeep2->position.x -= JEEP_SPEED;
        SPR_setPosition(PlayerJeep2->sprite, PlayerJeep2->position.x, PlayerJeep2->position.y);
    }
    else if(joy_state & BUTTON_RIGHT) {
        KLog("RIGHT CLICKED");
        PlayerJeep2->position.x += JEEP_SPEED;
        SPR_setPosition(PlayerJeep2->sprite, PlayerJeep2->position.x, PlayerJeep2->position.y);
    }

    // Vertical movement
    if(joy_state & BUTTON_UP) {
        KLog("UP CLICKED");
        PlayerJeep2->position.y -= JEEP_SPEED;
        SPR_setPosition(PlayerJeep2->sprite, PlayerJeep2->position.x, PlayerJeep2->position.y);
    }
    else if(joy_state & BUTTON_DOWN) {
        KLog("DOWN CLICKED");
        PlayerJeep2->position.y += JEEP_SPEED;
        SPR_setPosition(PlayerJeep2->sprite, PlayerJeep2->position.x, PlayerJeep2->position.y);
    }
}

int main()
{
    VDP_init();

    SPR_init();

    //PAL_setPalette(PAL2, car.palette->data, DMA);
    //SPR_addSprite(&car, 10, 10, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    // init_jeep();
    PlayerJeep2 = create_jeep(10, 10);

    while(1)
    {
        HandleInputs();
        SPR_update();
        SYS_doVBlankProcess();
    }
    
    return (0);
}
