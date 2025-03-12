#include "game.h"
#include "gfx/gfx.h"

void PrintCentered(const char *str)
{
    gfx_PrintStringXY(str,
                      (GFX_LCD_WIDTH - gfx_GetStringWidth(str)) / 2,
                      (GFX_LCD_HEIGHT - 8) / 2);
}

void init_game(game_t *game) 
{
    srandom(time(NULL));
    game->state = PLAYING;
    game->cursor.x = 0;
    game->cursor.y = 0;
    game->mine_count = EASY_MINE_COUNT;
    game->flagged_tiles = 0;
    
    memset(game->tiles, 0, sizeof(game->tiles));
    
    // Randomly Place Mines
    u8 minesPlaced = 0;
    while (minesPlaced < game->mine_count)
    {
        u8 randRow = random() % SCREEN_ROWS;
        u8 randCol = random() % SCREEN_COLS;

        if (!game->tiles[randRow][randCol].has_mine) {
            game->tiles[randRow][randCol].has_mine = true;
            minesPlaced += 1;
        }
    }

    
    // Calculate Numbers for non-mine tiles
    for (u8 i = 0; i < SCREEN_ROWS; i++) {
        for (u8 j = 0; j < SCREEN_COLS; j++) {
            if (game->tiles[i][j].has_mine) continue;

            u8 mineCount = 0;

            for (i8 dx = -1; dx <= 1; dx++) {
                for (i8 dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;

                    // Neighbor Coordinates
                    i8 ni = i + dx;
                    i8 nj = j + dy;
                    
                    if ((ni >= 0 && ni < SCREEN_ROWS) && (nj >= 0 && nj < SCREEN_COLS)) {
                        if (game->tiles[ni][nj].has_mine) mineCount++;
                    }
                }
            }

            game->tiles[i][j].mine_amount = mineCount;
        }
    }
    
}

void update_game(game_t *game)
{
    // Cursor Directions
    if (kb_IsDown(kb_KeyUp) && game->cursor.y > 0) game->cursor.y--;
    else if (kb_IsDown(kb_KeyDown) && game->cursor.y < SCREEN_COLS - 1) game->cursor.y++;
    else if (kb_IsDown(kb_KeyLeft) && game->cursor.x > 0) game->cursor.x--;
    else if (kb_IsDown(kb_KeyRight) && game->cursor.x < SCREEN_ROWS - 1) game->cursor.x++;
    
    // Plus (Flag Tile)
    else if (kb_IsDown(kb_KeyAdd) && (game->flagged_tiles < game->mine_count)) {
        u8 x = game->cursor.x;
        u8 y = game->cursor.y;
    
        if (!game->tiles[x][y].flagged) {
            game->flagged_tiles++;
            game->tiles[x][y].flagged = true;
        }
    } else if (kb_IsDown(kb_KeyClear) && (game->flagged_tiles > 0)) {
        u8 x = game->cursor.x;
        u8 y = game->cursor.y;

        if (game->tiles[x][y].flagged) {
            game->flagged_tiles--;
            game->tiles[x][y].flagged = false;
        }
    }
        
    // Enter (Determine if tile has mine)
    else if (kb_IsDown(kb_KeyEnter)) {
        u8 x = game->cursor.x;
        u8 y = game->cursor.y;
    
        if (!game->tiles[x][y].has_mine) {
            game->tiles[x][y].revealed = true;
        } else {
            game->state = (u8)LOST;
        }
    }
}

void check_win(game_t *game)
{
    if (game->flagged_tiles == game->mine_count) {
        u8 valid_tiles = 0;
        for (u8 i = 0; i < SCREEN_ROWS; i++) {
            for (u8 j = 0; j < SCREEN_COLS; j++) {
                tile_t tile = game->tiles[i][j];
                
                if (tile.flagged && tile.has_mine) valid_tiles++;
            }
        }

        if (valid_tiles == game->mine_count) game->state = (u8)WON;
    }
}

void draw_game(game_t *game)
{
    gfx_ZeroScreen();
    gfx_SetColor(0xFF);

    for (u8 i = 0; i < SCREEN_ROWS; i++) {
        for (u8 j = 0; j < SCREEN_COLS; j++) {
            gfx_Rectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            if (game->tiles[i][j].flagged) gfx_TransparentSprite_NoClip(flag, i * TILE_SIZE, j * TILE_SIZE);

            else if (game->tiles[i][j].revealed) {
                if (!game->tiles[i][j].has_mine) {
                    gfx_SetTextXY(i * TILE_SIZE + 7, j * TILE_SIZE + 7);
                    gfx_PrintUInt(game->tiles[i][j].mine_amount, 1);
                } else {
                    gfx_SetColor(0xA0);
                    gfx_FillRectangle(i * TILE_SIZE + 1, j * TILE_SIZE + 1, TILE_SIZE - 1, TILE_SIZE - 1);
                }
            }
           
            gfx_SetColor(0xFF);
        }
    }

    // Draw Cursor
    gfx_SetColor(0xA0);
    gfx_Rectangle(game->cursor.x * TILE_SIZE, game->cursor.y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    
    // Draw Mine Count and Flagged Count
    gfx_SetTextXY(TILE_SIZE, GFX_LCD_HEIGHT - TILE_SIZE); 
    gfx_PrintString("Mines: ");
    gfx_PrintUInt(game->mine_count, 2); 

    gfx_SetTextXY(GFX_LCD_WIDTH - (TILE_SIZE * 4), GFX_LCD_HEIGHT - TILE_SIZE); 
    gfx_PrintString("Flags: ");
    gfx_PrintUInt(game->flagged_tiles, 2); 

    gfx_SwapDraw();
}

void draw_result(game_t *game) {
    gfx_ZeroScreen();

    switch (game->state) {
        case WON:
            gfx_SetTextFGColor(0x4F);
            PrintCentered("You've Won! :)");
            break;
        case LOST:
            gfx_SetTextFGColor(0xE0);
            PrintCentered("You've Lost! :(");
            break;
    }

    gfx_SwapDraw();
}