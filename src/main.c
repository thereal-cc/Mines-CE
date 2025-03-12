#include "game.h"

int main(void) 
{
    game_t game;
    kb_key_t key;

    init_game(&game);

    // Set Up Graphics & Sprites
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    gfx_SetDrawBuffer();

    // Set Up Text
    gfx_SetTextScale(1, 1);
    gfx_SetTextTransparentColor(0);
    gfx_SetTextBGColor(0);
    gfx_SetTextFGColor(0xFF);

    while (game.state != QUIT)
    {
        // Quit Game (State Persistant)
        kb_Scan();
        if (kb_IsDown(kb_KeyDel)) game.state = QUIT;

        if (game.state == PLAYING) {
            update_game(&game);
            check_win(&game);
            draw_game(&game);
        } else {
            draw_result(&game);
        }

        delay(16); // 16.67 ms
    }

    gfx_End();
    return 0;
}