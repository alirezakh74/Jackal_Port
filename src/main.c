#include <genesis.h>
#include <resources.h>

// Use fixed-point for precise movement
#define JEEP_SPEED FIX32(1.0)       // Base speed
#define DIAGONAL_FACTOR FIX32(0.7071) // 1/sqrt(2)

typedef enum {
    RIHGT = 0,
    UP_RIGHT = 1,
    UP = 2,
    UP_LEFT = 3,
    LEFT = 4,
    DOWN_LEFT = 5,
    DOWN = 6,
    DOWN_RIGHT = 7,
    DIR_COUNTER = 8
} DIR;

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
DIR CurrentJeepDir = UP;
//DIR PreviusJeepDir = UP;
DIR TargetJeepDir = UP;
bool ShouldIncrease = TRUE;
u32 StartTime = 0;
u32 ElapsedTime = 0;

static void HandleInputs() 
{
    u16 joy_state = JOY_readJoypad(JOY_1);
    fix32 move_x = 0;
    fix32 move_y = 0;

    //u8 flip_v = false;
    //u8 flip_h = false;

    // Horizontal movement
    if(joy_state & BUTTON_LEFT) {
        //KLog("LEFT CLICKED");
        move_x = -JEEP_SPEED; 
        //flip_h = true;
        //SPR_setHFlip(PlayerJeep2->sprite, true);
        //SPR_setAnim(PlayerJeep2->sprite, 0);
    }
    else if(joy_state & BUTTON_RIGHT) {
        //KLog("RIGHT CLICKED");
        move_x = JEEP_SPEED;
        //flip_h = false;
        //SPR_setHFlip(PlayerJeep2->sprite, false);
        //SPR_setAnim(PlayerJeep2->sprite, 0);
    }

    // Vertical movement
    if(joy_state & BUTTON_UP) {
        //KLog("UP CLICKED");
        move_y = -JEEP_SPEED;
        //flip_v = true;
    }
    else if(joy_state & BUTTON_DOWN) {
        //KLog("DOWN CLICKED");
        move_y = JEEP_SPEED;
        //flip_v = false;
    }

    // Diagonal movement normalization
    if(move_x != 0 && move_y != 0) {
        move_x = fix32Mul(move_x, DIAGONAL_FACTOR);
        move_y = fix32Mul(move_y, DIAGONAL_FACTOR);

        // KLog_S1("int of move_x is = ", fix32Int(move_x));
        // KLog_S1("frac of move_x is = ", fix32Frac(move_x));
        // KLog_S1("int of move_y is = ", fix32Int(move_y));
        // KLog_S1("frac of move_y is = ", fix32Frac(move_y));
    }

    // Apply movement
    PlayerJeep2->position.x += move_x;
    PlayerJeep2->position.y += move_y;

    if(move_x > 0)
    {
        //SPR_setAnimAndFrame(PlayerJeep2->sprite, 0, 3);
        TargetJeepDir = RIHGT;
    }
    if(move_x < 0)
    {
        //SPR_setAnimAndFrame(PlayerJeep2->sprite, 1, 0);
        TargetJeepDir = LEFT;
    }
    if(move_y > 0)
    {
        //SPR_setAnimAndFrame(PlayerJeep2->sprite, 1, 2);
        TargetJeepDir = DOWN;
    }
    if(move_y < 0)
    {
        //SPR_setAnimAndFrame(PlayerJeep2->sprite, 0, 1);
        TargetJeepDir = UP;
    }

    if(move_x < 0 && move_y < 0)
    {
        //SPR_setAnimAndFrame(PlayerJeep2->sprite, 0, 0);
        TargetJeepDir = UP_LEFT;
    }
    if(move_x < 0 && move_y > 0)
    {
        //SPR_setAnimAndFrame(PlayerJeep2->sprite, 1, 1);
        TargetJeepDir = DOWN_LEFT;
    }
    if(move_x > 0 && move_y < 0)
    {
        //SPR_setAnimAndFrame(PlayerJeep2->sprite, 0, 2);
        TargetJeepDir = UP_RIGHT;
    }
    if(move_x > 0 && move_y > 0)
    {
        //SPR_setAnimAndFrame(PlayerJeep2->sprite, 1, 3);
        TargetJeepDir = DOWN_RIGHT;
    }

    s8 DeltaDir = TargetJeepDir - CurrentJeepDir;
    bool IsDeltaDirPositive = (DeltaDir > 0);
    DeltaDir = abs(DeltaDir);

    s8 CurrentIndex = CurrentJeepDir;

    if(DeltaDir <= 4)
    {
        if(IsDeltaDirPositive)
        {
            // index should increase
            ShouldIncrease = TRUE;
            if(CurrentJeepDir == 3 && TargetJeepDir == 7)
            {
                ShouldIncrease = FALSE;
            }
        }
        else
        {
            // index should decrease
            ShouldIncrease = FALSE;
            if(CurrentJeepDir == 7 && TargetJeepDir == 3)
            {
                ShouldIncrease = TRUE;
            }
        }
    }
    else if(DeltaDir > 4)
    {
        if(IsDeltaDirPositive)
        {
            // index should increase
            ShouldIncrease = FALSE;
        }
        else
        {
            // index should decrease
            ShouldIncrease = TRUE;
        }
    }

    //KLog_S1("delta dir = ", DeltaDir);
    ElapsedTime = getTick() / 60;
    if(ElapsedTime - StartTime >= 1)
    {
        if(move_x != 0 || move_y != 0)
        {
            //KLog_S1("timer = ", StartTime);
            StartTime = ElapsedTime;

            if(CurrentJeepDir != TargetJeepDir)
            {
                if(ShouldIncrease)
                {
                    KLog_S1("b Current index = ", CurrentIndex);
                    CurrentIndex++;
                    if(CurrentIndex > (DIR_COUNTER - 1))
                    {
                        CurrentIndex = 0;
                    }
                    KLog_S1("a Current index = ", CurrentIndex);
                }
                else
                {
                    KLog_S1("b Current index = ", CurrentIndex);
                    CurrentIndex--;
                    if(CurrentIndex < 0)
                    {
                        CurrentIndex = DIR_COUNTER - 1;
                    }
                    KLog_S1("a Current index = ", CurrentIndex);
                }
            }

            switch (CurrentIndex)
            {
            case 0:
                SPR_setAnimAndFrame(PlayerJeep2->sprite, 0, 3);
                CurrentJeepDir = RIHGT;
                break;
            case 1:
                SPR_setAnimAndFrame(PlayerJeep2->sprite, 0, 2);
                CurrentJeepDir = UP_RIGHT;
                break;
            case 2:
                SPR_setAnimAndFrame(PlayerJeep2->sprite, 0, 1);
                CurrentJeepDir = UP;
                break;
            case 3:
                SPR_setAnimAndFrame(PlayerJeep2->sprite, 0, 0);
                CurrentJeepDir = UP_LEFT;
                break;
            case 4:
                SPR_setAnimAndFrame(PlayerJeep2->sprite, 1, 0);
                CurrentJeepDir = LEFT;
                break;
            case 5:
                SPR_setAnimAndFrame(PlayerJeep2->sprite, 1, 1);
                CurrentJeepDir = DOWN_LEFT;
                break;
            case 6:
                SPR_setAnimAndFrame(PlayerJeep2->sprite, 1, 2);
                CurrentJeepDir = DOWN;
                break;
            case 7:
                SPR_setAnimAndFrame(PlayerJeep2->sprite, 1, 3);
                CurrentJeepDir = DOWN_RIGHT;
                break;
            default:
                break;
            }
        }

        //KLog_S1("Current index = ", CurrentIndex);

        //PreviusJeepDir = CurrentJeepDir;
    }
    
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
    SPR_setAnim(PlayerJeep2->sprite, 0);
    SPR_setAnimationLoop(PlayerJeep2->sprite, FALSE);
    SPR_setAutoAnimation(PlayerJeep2->sprite, FALSE);
    SPR_setFrame(PlayerJeep2->sprite, 1);

    StartTime = getTick() / 60;

    while(1)
    {
        //MAP_scrollTo(map1, x, y);
        //x++;
        //y--;
        // if(y < 0)
        // {
        //     y = 2565;
        // }

        HandleInputs();
        SPR_update();
        SYS_doVBlankProcess();
    }
    
    return (0);
}
