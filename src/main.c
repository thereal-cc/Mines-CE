#include "game.h"

int main(void) 
{
    game_t game;
    kb_key_t key;

    // Set Up Graphics & Sprites
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    gfx_SetDrawBuffer();

    // Set Up Text
    gfx_SetTextScale(3, 3);
    gfx_SetTextTransparentColor(0);
    gfx_SetTextBGColor(0);
    gfx_SetTextFGColor(0xFF);

    v2 title_cursor;
    game.state = TITLE;

    while (game.state != QUIT)
    {
        kb_Scan();
        if (kb_IsDown(kb_KeyDel)) game.state = QUIT;

        switch (game.state) {
            case TITLE:
                title_screen(&title_cursor, &game);
                break;
            case PLAYING:
                update_game(&game);
                check_win(&game);
                draw_game(&game);
                break;
            default:
                draw_result(&game);
                break;
        }
    
        delay(16); // 16.67 ms
    }

    gfx_End();
    return 0;
}