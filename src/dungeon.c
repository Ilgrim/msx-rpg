//------------------------------------------------------------------
// Star Rangers - A Dungeon Crawler for MSX2 using Fusion-C
//------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_sprites.h"
#include "fusion-c/header/vdp_graph2.h"

#include "main.h"
#include "procgen.h"
#include "font.h"
#include "dungeon.h"

//------------------------------------------------------------------
// Variables.
//------------------------------------------------------------------

char *dungeon_map;

const int dir_translate_x[] = {0, 1, 0, -1};
const int dir_translate_y[] = {-1, 0, 1, 0};

char player_moves;

int player_pos_x;
int player_pos_y;

enum Direction player_dir;

char joy;

//------------------------------------------------------------------
// Functions.
//------------------------------------------------------------------

void sf_draw_third_wall_far_left(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE && dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(THIRD_WALL_FAR_LEFT_X, THIRD_WALL_FAR_LEFT_Y,
					   THIRD_WALL_FAR_DX, THIRD_WALL_FAR_DY,
					   DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_WALL_FAR_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_third_wall_far_right(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE && dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(THIRD_WALL_FAR_RIGHT_X, THIRD_WALL_FAR_RIGHT_Y,
					   THIRD_WALL_FAR_DX, THIRD_WALL_FAR_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - THIRD_WALL_FAR_DX, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_WALL_FAR_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_third_wall_left(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE && dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(THIRD_WALL_LEFT_X, THIRD_WALL_LEFT_Y,
					   THIRD_WALL_DX, THIRD_WALL_DY,
					   DUNGEON_SCREEN_X + THIRD_WALL_OFFSET_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_WALL_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_third_wall_right(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE && dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(THIRD_WALL_RIGHT_X, THIRD_WALL_RIGHT_Y,
					   THIRD_WALL_DX, THIRD_WALL_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - THIRD_WALL_DX - THIRD_WALL_OFFSET_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_WALL_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_far_left(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE && dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(SECOND_WALL_FAR_LEFT_X, SECOND_WALL_FAR_LEFT_Y,
					   SECOND_WALL_FAR_DX, SECOND_WALL_FAR_DY,
					   DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_WALL_FAR_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_far_right(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE && dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(SECOND_WALL_FAR_RIGHT_X, SECOND_WALL_FAR_RIGHT_Y,
					   SECOND_WALL_FAR_DX, SECOND_WALL_FAR_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - SECOND_WALL_FAR_DX, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_WALL_FAR_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_left(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE && dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(SECOND_WALL_LEFT_X, SECOND_WALL_LEFT_Y,
					   SECOND_WALL_DX, SECOND_WALL_DY,
					   DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_WALL_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_right(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE && dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(SECOND_WALL_RIGHT_X, SECOND_WALL_RIGHT_Y,
					   SECOND_WALL_DX, SECOND_WALL_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - SECOND_WALL_DX, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_WALL_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_first_wall_left(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE &&	dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(FIRST_WALL_LEFT_X, FIRST_WALL_LEFT_Y,
					   FIRST_WALL_DX, FIRST_WALL_DY,
					   DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y,
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_first_wall_right(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE &&	dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(FIRST_WALL_RIGHT_X, FIRST_WALL_RIGHT_Y,
					   FIRST_WALL_DX, FIRST_WALL_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - FIRST_WALL_DX, DUNGEON_SCREEN_Y,
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_third_wall_front(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE &&	dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(THIRD_FRONT_WALL_X, THIRD_FRONT_WALL_Y,
					   THIRD_FRONT_WALL_DX, THIRD_FRONT_WALL_DY,
					   DUNGEON_SCREEN_X + ((DUNGEON_SCREEN_DX - THIRD_FRONT_WALL_DX) * 0.5), DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_FRONT_WALL_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_front(uint x, uint y)
{
	if (x < DUNGEON_SIZE && y < DUNGEON_SIZE &&	dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
	{
		sf_screen_copy(SECOND_FRONT_WALL_X, SECOND_FRONT_WALL_Y,
					   SECOND_FRONT_WALL_DX, SECOND_FRONT_WALL_DY,
					   DUNGEON_SCREEN_X + ((DUNGEON_SCREEN_DX - SECOND_FRONT_WALL_DX) * 0.5), DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_FRONT_WALL_DY) * 0.5),
					   WALLS_PAGE, back_page, LOGICAL_TIMP);
	}
}

/*
 * This is the area to draw:
 *
 *  x x x x x
 *  x x x x x
 *    x o x
 *
 * horizontal order: far left, left, front, right, far right.
 * vertical order: third, second, first.
 */
void sf_draw_dungeon_walls(void)
{
	uint x, y, xd, yd;

	int mult = 1;

	if (player_dir == North || player_dir == South)
	{
		mult = -1;
	}

	// Draw by distance.
	for (int dist = 2; dist > -1; dist--)
	{
		x = player_pos_x + (dist * dir_translate_x[player_dir]);
		y = player_pos_y + (dist * dir_translate_y[player_dir]);

		// Far left walls.
		xd = x - (2 * mult * dir_translate_y[player_dir]);
		yd = y - (2 * mult * dir_translate_x[player_dir]);

		if (dist == 2)		sf_draw_third_wall_far_left(xd, yd);
		else if (dist == 1)	sf_draw_second_wall_far_left(xd, yd);

		// Left walls.
		xd = x - (mult * dir_translate_y[player_dir]);
		yd = y - (mult * dir_translate_x[player_dir]);

		if (dist == 2)		sf_draw_third_wall_left(xd, yd);
		else if (dist == 1)	sf_draw_second_wall_left(xd, yd);
		else				sf_draw_first_wall_left(xd, yd);

		// Far right walls.
		xd = x + (2 * mult * dir_translate_y[player_dir]);
		yd = y + (2 * mult * dir_translate_x[player_dir]);

		if (dist == 2)		sf_draw_third_wall_far_right(xd, yd);
		else if (dist == 1)	sf_draw_second_wall_far_right(xd, yd);

		// Right walls.
		xd = x + (mult * dir_translate_y[player_dir]);
		yd = y + (mult * dir_translate_x[player_dir]);

		if (dist == 2)		sf_draw_third_wall_right(xd, yd);
		else if (dist == 1)	sf_draw_second_wall_right(xd, yd);
		else				sf_draw_first_wall_right(xd, yd);

		// Front walls.
		if (dist == 2)		sf_draw_third_wall_front(x, y);
		else if (dist == 1)	sf_draw_second_wall_front(x, y);
	}
}

// Currently not being used.
void sf_draw_tiles_background(void)
{
	// #SAFFRON TEST: Draw tiles background.
	for (int x = 0; x < 32; x++)
	{
		for (int y = 0; y < 26; y++)
		{
			sf_screen_copy(160, 0,
						   8, 8,
						   x * 8, y * 8,
						   SPRITES_PAGE, back_page, opHMMM);
		}
	}
}

void sf_draw_avatars(void)
{
	// Avatar images.
	sf_screen_copy(0, 120,
				   32, 32,
				   180, 1 * 32 + 4,
				   SPRITES_PAGE, back_page, opHMMM);

	sf_screen_copy(32, 120,
				   32, 32,
				   180, 2 * 32 + 4 + 8,
				   SPRITES_PAGE, back_page, opHMMM);

	sf_screen_copy(64, 120,
				   32, 32,
				   180, 3 * 32 + 4 + 8 + 8,
				   SPRITES_PAGE, back_page, opHMMM);

	// HP bars.
	LMMV(180 + 32 + 2, 1 * 32 + 4 + back_page * 256,
		 4, 32,
		 11, 0);

	LMMV(180 + 32 + 2, 2 * 32 + 4 + 8 + back_page * 256,
		 4, 32,
		 11, 0);

	LMMV(180 + 32 + 2, 3 * 32 + 4 + 8 + 8 + back_page * 256,
		 4, 32,
		 11, 0);

	// TP bars.
	// Rect(186 + 32 + 2, 1 * 32 + 4 + back_page * 256,
	// 	 186 + 32 + 2 + 4, 1 * 32 + 4 + 32 + back_page * 256,
	// 	 8, FILL_ALL);

	// Rect(186 + 32 + 2, 2 * 32 + 4 + 8 + back_page * 256,
	// 	 186 + 32 + 2 + 4, 2 * 32 + 4 + 32 + 8 + back_page * 256,
	// 	 8, FILL_ALL);

	// Rect(186 + 32 + 2, 3 * 32 + 4 + 8 + 8 + back_page * 256,
	// 	 186 + 32 + 2 + 4, 3 * 32 + 4 + 32 + 8 + 8 + back_page * 256,
	// 	 8, FILL_ALL);
}

void sf_draw_combat_menu(void)
{
	// SetColors(15, 0, 0);
	// PutText(20, 160, "Fight", LOGICAL_TIMP);

	// SetColors(8, 0, 0);
	// PutText(20, 160 + 8+2, "Talk", LOGICAL_IMP);
	// PutText(20, 160 + 8+2 + 8+2, "Escape", LOGICAL_IMP);

	// SetColors(15, 0, 0);
	// PutText(20, 160, "FIGHT", LOGICAL_TIMP);

	// SetColors(8, 0, 0);
	// PutText(20, 160 + 8+2, "TALK", LOGICAL_IMP);
	// PutText(20, 160 + 8+2 + 8+2, "ESCAPE", LOGICAL_IMP);

	sf_draw_text("Attack              ", 8*2, 160, 0, 8);
	sf_draw_text("Tech", 8*2, 160 + 8+2, 8, 0);
	sf_draw_text("Defense", 8*2, 160 + 8+2 + 8+2, 8, 0);
}

void sf_draw_minimap(void)
{
	for (int x = 0; x < DUNGEON_SIZE; x++)
	{
		for (int y = 0; y < DUNGEON_SIZE; y++)
		{
			if (dungeon_map[x + y * DUNGEON_SIZE] == TILE_WALL)
			{
				Pset(x + 16, y + back_page * 256, 10, 0);
			}
			else if (player_pos_x == x && player_pos_y == y)
			{
				Pset(x + 16, y + back_page * 256, 13, 0);
			}
			else
			{
				Pset(x + 16, y + back_page * 256, 2, 0);
			}
		}
	}
}

// Draw the current dungeon room.
void sf_draw_dungeon_view(void)
{
	db_state = Updating;

	// #SAFFRON #TODO Menu test.
	sf_draw_combat_menu();

	// Debug: Minimap.
	sf_draw_minimap();

	// Debug: Palette.
	sf_draw_palette();

	// Party avatars.
	sf_draw_avatars();

	// Compass.
	switch(player_dir)
	{
		case North:	sf_draw_text("North",	80, 20, 9, 0);	break;
		case East:	sf_draw_text("East",	80, 20, 9, 0);	break;
		case South:	sf_draw_text("South",	80, 20, 9, 0);	break;
		case West:	sf_draw_text("West",	80, 20, 9, 0);	break;
	}

	// Dungeon background.
	sf_screen_copy(0, 0,
		DUNGEON_SCREEN_DX, DUNGEON_SCREEN_DY,
		DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y,
		SPRITES_PAGE, back_page, opHMMM);

	// Dungeon walls.
	sf_draw_dungeon_walls();

	// #TODO Enemies.
	//sf_screen_copy(169,55, 32,32, 60,110, SPRITES_PAGE, back_page, LOGICAL_TIMP);
	//sf_screen_copy(169,55, 32,32, 100,110, SPRITES_PAGE, back_page, LOGICAL_TIMP);

	db_state = ReadyToSwitch;
}

void sf_move(int newPosX, int newPosY)
{
	if (newPosX > 0 && newPosX < DUNGEON_SIZE - 1 &&
		newPosY > 0 && newPosY < DUNGEON_SIZE - 1 &&
		(dungeon_map[newPosX + newPosY * DUNGEON_SIZE] == TILE_ROOM ||
		dungeon_map[newPosX + newPosY * DUNGEON_SIZE] == TILE_CORRIDOR))
	{
		player_pos_x = newPosX;
		player_pos_y = newPosY;

		player_moves = TRUE;
	}
}

void sf_rotate_left(void)
{
	if (--player_dir < North)
	{
		player_dir = West;
	}

	player_moves = TRUE;
}

void sf_rotate_right(void)
{
	if (++player_dir > West)
	{
		player_dir = North;
	}

	player_moves = TRUE;
}

// Reads input from keyboard's arrow keys and joystick port 1.
void sf_update_input_dungeon_mode(void)
{
	// Cursor.
	for (char i = 0; i < 2; i++)
	{
		joy = JoystickRead(i);

		switch (joy)
		{
			case UP:
				sf_move(player_pos_x + dir_translate_x[player_dir],
						player_pos_y + dir_translate_y[player_dir]);
				break;

			case DOWN:
				sf_move(player_pos_x - dir_translate_x[player_dir],
						player_pos_y - dir_translate_y[player_dir]);
				break;

			case LEFT:
				sf_rotate_left();
				break;

			case RIGHT:
				sf_rotate_right();
				break;
		}
	}

	// Buttons.
	if (TriggerRead(JOY1_BUTTONB) == PRESSED )
	{
		sf_generate_dungeon();
	}

	if (TriggerRead(SPACEBAR) == PRESSED )
	{
		sf_generate_dungeon();
	}
}

void sf_set_dungeon_state(void)
{
	// Initialize variables.
	player_moves = FALSE;

	// This may be set by the random generator in the future?
	player_dir = North;

	// Generate dungeon.
	SetColors(0,0,0);
	Cls();
	sf_draw_text("Entering the Undercity...", 8, 8, 15, 0);
	dungeon_map = NULL;
	sf_generate_dungeon();

	// Initial draw call.
	Cls();
	sf_draw_dungeon_view();
}

void sf_update_dungeon_state(void)
{
	// Update input and dungeon view in different cycles.
	if (player_moves == FALSE)
	{
		sf_update_input_dungeon_mode();
	}
	else
	{
		player_moves = FALSE;

		sf_draw_dungeon_view();
	}
}
