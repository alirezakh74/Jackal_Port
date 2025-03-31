#include <genesis.h>
#include <resources.h>

// Use fixed-point for precise movement
#define JEEP_SPEED FIX32(0.5)       // Base speed
#define DIAGONAL_FACTOR FIX32(0.7071) // 1/sqrt(2)

typedef struct {
    Vect2D_f32 position;
    Vect2D_f32 velocity;
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
    Jeep* new_jeep = (Jeep*)MEM_alloc(sizeof(Jeep));
    new_jeep->position.x = x;
    new_jeep->position.y = y;
    new_jeep->sprite = SPR_addSprite(&jeep, x, y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    return new_jeep;
}

Jeep* PlayerJeep2 = NULL;

static void HandleInputs() 
{
    u16 joy_state = JOY_readJoypad(JOY_1);
    fix32 move_x = 0;
    fix32 move_y = 0;

    //u8 flip_v = false;
    //u8 flip_h = false;

    // Horizontal movement
    if(joy_state & BUTTON_LEFT) {
        KLog("LEFT CLICKED");
        move_x = -JEEP_SPEED; 
        //flip_h = true;
        //SPR_setHFlip(PlayerJeep2->sprite, true);
        //SPR_setAnim(PlayerJeep2->sprite, 0);
    }
    else if(joy_state & BUTTON_RIGHT) {
        KLog("RIGHT CLICKED");
        move_x = JEEP_SPEED;
        //flip_h = false;
        //SPR_setHFlip(PlayerJeep2->sprite, false);
        //SPR_setAnim(PlayerJeep2->sprite, 0);
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
        move_x = fix32Mul(move_x, DIAGONAL_FACTOR);
        move_y = fix32Mul(move_y, DIAGONAL_FACTOR);

        KLog_S1("int of move_x is = ", fix32Int(move_x));
        KLog_S1("frac of move_x is = ", fix32Frac(move_x));
        KLog_S1("int of move_y is = ", fix32Int(move_y));
        KLog_S1("frac of move_y is = ", fix32Frac(move_y));
    }

    // Apply movement
    PlayerJeep2->position.x += move_x;
    PlayerJeep2->position.y += move_y;
    
    //SPR_setVFlip(PlayerJeep2->sprite, flip_v);
    //SPR_setHFlip(PlayerJeep2->sprite, flip_h);
    
    // Update sprite position (convert to integer)
    SPR_setPosition(PlayerJeep2->sprite, 
        fix32ToInt(PlayerJeep2->position.x),
        fix32ToInt(PlayerJeep2->position.y));
}

u16 ind = TILE_USER_INDEX;
int x = 0, y = 0;
Map* map1;

int main()
{
    VDP_init();

    VDP_loadTileSet(&Background, ind, DMA);
    PAL_setPalette(PAL1, level_map_1_pallete.data, DMA);
    map1 = MAP_create(&level_map_1, BG_B, TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind));
    MAP_scrollTo(map1, x, 2565);

    SPR_init();

    //PAL_setPalette(PAL2, car.palette->data, DMA);
    //SPR_addSprite(&car, 10, 10, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    //init_jeep();
    PlayerJeep2 = create_jeep(150, 100);
    // SPR_setAnim(PlayerJeep2->sprite, 0);
    // SPR_setAnimationLoop(PlayerJeep2->sprite, FALSE);
    // SPR_setAutoAnimation(PlayerJeep2->sprite, FALSE);
    // SPR_setFrame(PlayerJeep2->sprite, 1);

    fix16 fixed16_number = FIX16(0.0);
    char fixed16_text[5];
    sprintf(fixed16_text, "%10i", fix32ToInt(fixed16_number));
    VDP_drawTextBG(BG_B, fixed16_text, 0, 0);

    while(1)
    {
        sprintf(fixed16_text, "%5i", fix16ToInt(fixed16_number));
        VDP_drawTextBG(BG_A, fixed16_text, 0, 0);

        sprintf(fixed16_text, "%5i", fix16ToInt(fix16Int(fixed16_number)));
        VDP_drawTextBG(BG_A, fixed16_text, 0, 3);

        sprintf(fixed16_text, "%5i", fix16Frac(fixed16_number));
        VDP_drawTextBG(BG_A, fixed16_text, 0, 6);

        fixed16_number += JEEP_SPEED;

        MAP_scrollTo(map1, x, y);
        //x++;
        y--;
        if(y < 0)
        {
            y = 2565;
        }

        HandleInputs();
        SPR_update();
        SYS_doVBlankProcess();
    }
    
    return (0);
}
