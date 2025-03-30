#include <genesis.h>
#include <resources.h>

// Use fixed-point for precise movement
#define JEEP_SPEED FIX16(1.0)       // Base speed
#define DIAGONAL_FACTOR FIX16(0.7071) // 1/sqrt(2)

typedef struct {
    Vect2D_f16 position;
    Vect2D_f16 velocity;
    Sprite* sprite;
} Jeep;

Jeep PlayerJeep = {
    .position = { 100, 100 },
    .velocity = { 0, 0 },
    .sprite = NULL // Will be set later
};

void init_jeep() {
    PAL_setPalette(PAL2, jeep.palette->data, DMA);
    PlayerJeep.sprite = SPR_addSprite(&jeep, 
        PlayerJeep.position.x, 
        PlayerJeep.position.y,
        TILE_ATTR(PAL2, 0, 0, 0));
}

Jeep* create_jeep(s16 x, s16 y) {
    PAL_setPalette(PAL2, jeep.palette->data, DMA);
    Jeep* jeep = (Jeep*)MEM_alloc(sizeof(Jeep));
    jeep->position.x = x;
    jeep->position.y = y;
    jeep->velocity.x = 0;
    jeep->velocity.y = 0;
    jeep->sprite = SPR_addSprite(&jeep, x, y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return jeep;
}

Jeep* PlayerJeep2 = NULL;

static void HandleInputs() 
{
    u16 joy_state = JOY_readJoypad(JOY_1);
    fix16 move_x = 0;
    fix16 move_y = 0;

    //u8 flip_v = false;
    //u8 flip_h = false;

    // Horizontal movement
    if(joy_state & BUTTON_LEFT) {
        KLog("LEFT CLICKED");
        move_x = -JEEP_SPEED;
        //flip_h = true;
        //SPR_setHFlip(PlayerJeep2->sprite, true);
        SPR_setAnim(PlayerJeep2->sprite, 4);
    }
    else if(joy_state & BUTTON_RIGHT) {
        KLog("RIGHT CLICKED");
        move_x = JEEP_SPEED;
        //flip_h = false;
        //SPR_setHFlip(PlayerJeep2->sprite, false);
        SPR_setAnim(PlayerJeep2->sprite, 3);
    }

    // Vertical movement
    if(joy_state & BUTTON_UP) {
        KLog("UP CLICKED");
        move_y = -JEEP_SPEED;
        //flip_v = true;
    }
    else if(joy_state & BUTTON_DOWN) {
        KLog("DOWN CLICKED");
        move_y = JEEP_SPEED;
        //flip_v = false;
    }

    // Diagonal movement normalization
    if(move_x != 0 && move_y != 0) {
        move_x = fix16Mul(move_x, DIAGONAL_FACTOR);
        move_y = fix16Mul(move_y, DIAGONAL_FACTOR);
    }

    // Apply movement
    PlayerJeep2->position.x = PlayerJeep2->position.x + move_x;
    PlayerJeep2->position.y = PlayerJeep2->position.y + move_y;
    
    //SPR_setVFlip(PlayerJeep2->sprite, flip_v);
    //SPR_setHFlip(PlayerJeep2->sprite, flip_h);
    
    // Update sprite position (convert to integer)
    SPR_setPosition(PlayerJeep2->sprite, 
        fix16ToInt(PlayerJeep2->position.x),
        fix16ToInt(PlayerJeep2->position.y));
}

int main()
{
    VDP_init();

    SPR_init();

    //PAL_setPalette(PAL2, car.palette->data, DMA);
    //SPR_addSprite(&car, 10, 10, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    // init_jeep();
    PlayerJeep2 = create_jeep(10, 10);

    PAL_setPalette(PAL2, jeep.palette->data, DMA);
    Sprite* testJeep = SPR_addSprite(&jeep, 64, 64, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    SPR_setAnim(testJeep, 1);
    SPR_setAnimationLoop(testJeep, FALSE);
    SPR_setAutoAnimation(testJeep, FALSE);
    SPR_setFrame(testJeep, 2);
    
    KLog_S1("frames num = ",testJeep->animation->numFrame);

    while(1)
    {
        HandleInputs();
        SPR_update();
        SYS_doVBlankProcess();
    }
    
    return (0);
}
