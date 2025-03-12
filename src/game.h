#ifndef GAME_H
#define GAME_H

#include <graphx.h>
#include <keypadc.h>
#include <time.h>
#include <debug.h>

#include "utils.h"
#include "gfx/global_palette.h"

enum STATE {
    TITLE,
    PLAYING,
    LOST,
    WON,
    QUIT
};

typedef struct v2
{
    u8 x;
    u8 y;
} v2;

typedef struct tile_t
{
    u8 mine_amount; // Mines Around Specific Tile
    bool has_mine; // If Tile has a mine
    bool revealed; // If a Tile has been revealed
    bool flagged;
} tile_t;

typedef struct game_t
{
    tile_t tiles[SCREEN_ROWS][SCREEN_COLS];
    v2 cursor;
    u8 state;
    u8 mine_count;
    u8 flagged_tiles;
} game_t;

void init_game(game_t *game);
void update_game(game_t *game);
void check_win(game_t *game);
void draw_game(game_t *game);
void draw_result(game_t *game);

#endif