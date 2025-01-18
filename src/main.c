#include <genesis.h>
#include <resources.h>
#include <font.h>

u16 ind = TILE_USER_INDEX;
Map* map1;
Sprite* jeep_cursor;

u16 array_u16len(const u16 *arr)
{
    const u16 *src;

    src = arr;
    while (*src++);

    return (src - arr) - 1;
}

void VDP_drawTextOffset(VDPPlane plane, const u16 *vram_offsets, u16 len, u16 first_tile, u16 x, u16 y)
{
    u16 i, pw, ph, curX;

    // get the horizontal plane size (in cell)
    pw = (plane == WINDOW)?windowWidth:planeWidth;
    ph = (plane == WINDOW)?32:planeHeight;

    // string outside plane --> exit
    if ((x >= pw) || (y >= ph))
        return;

    // if string don't fit in plane, we cut it
    if (len > (pw - x))
        len = pw - x;
    i = 0;
	curX = 1 * x;
	u16 curTileInd = 0;
    u16 temp = 0;
    s16 value_convert = -73;
    while(i < len) {
        temp = -(value_convert + vram_offsets[i]);
        curTileInd = first_tile-1 + temp;
        
        if(curTileInd == first_tile - 1) // first tile is space char and minus one considered for new line
        {
            y++;
            curX = x + 1;
        }
		
		VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL0,0,FALSE,FALSE,curTileInd), curX, y);
		i++;
		curX--;
	}
}

// void drawCharCode(const char ch){
// 	KLog_S1("fdsdf: ", (int)ch);
// }

void handleInput()
{
    VDP_clearText(1,1,5);
    u16 value = JOY_readJoypad(JOY_1);
	
    if(value & BUTTON_UP) 
    {
        SPR_setPosition(jeep_cursor, 115, 125);
        VDP_drawText("Up",1,1);
    }
    if(value & BUTTON_DOWN) 
    {
        SPR_setPosition(jeep_cursor, 115, 140);
        VDP_drawText("Down",1,1);
    }
    if(value & BUTTON_START)
    {
        if(SPR_getPositionY(jeep_cursor) == 125)
        {
            VDP_clearText(18, 16, 11);
        }
        else
        {
            VDP_clearText(18, 18, 9);
        }

        XGM2_playPCM(&select_sfx, sizeof(select_sfx), SOUND_PCM_CH_AUTO);
    }
}

int x = 0, y = 0;
int main()
{
    VDP_init();

    VDP_loadTileSet(&Background, ind, DMA);
    PAL_setPalette(PAL3, level_map_1_pallete.data, DMA);
    map1 = MAP_create(&level_map_1, BG_B, TILE_ATTR_FULL(PAL3,true,false,false,ind));
    MAP_scrollTo(map1, x, y);
    
    //PAL_setPalette(PAL3, logo.palette->data, DMA);
    //VDP_drawImage(BG_A, &logo, 6, 0);

    SPR_init();

    PAL_setPalette(PAL2, car.palette->data, DMA);
    jeep_cursor = SPR_addSprite(&car, 115, 125, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
    SPR_update();

    VDP_drawText("PLAY SELECT", 14, 14);
    VDP_drawText("1 PLAYER", 18, 16);
    VDP_drawText("2 PLAYERS", 18, 18);
    VDP_drawText("Developed by Alireza_Kh74", 10, 21);

    XGM2_play(&music);
    XGM2_setPSGVolume(100);

    //ind = 1300;
    //VDP_loadTileSet(&font_fa, ind, DMA);

    //u16 textArr[] = {35,51,60,22,31,3,72,18,20,2,4,55,20,59};//35, 51, 60, 22, 31, 2, 72, 18, 20, 2, 4, 55, 20, 58
    //u16 textArr2[] = {65, 62, 66, 63};
   
    //VDP_drawTextOffset(BG_A, textArr, sizeof(textArr)/sizeof(textArr[0]), ind, 26, 23);
    //VDP_drawTextOffset(BG_A, textArr2, sizeof(textArr2)/sizeof(textArr2[0]), ind, 22, 26);

    while(1)
    {
        MAP_scrollTo(map1, x, y);
        //x++;
        y++;
        if(y > 2565)
        {
            y = 0;
        }
        handleInput();
        SPR_update();
        SYS_doVBlankProcess();
    }
    
    return (0);
}
