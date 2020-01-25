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
#include "font.h"
#include "startscreen.h"

void sf_draw_startscreen_intro_text(void)
{
	// Intro text.
    db_state = Updating;
    Cls();
	SetColors(0, 0, 0);
    sf_draw_text("@SAFFRON SOFT 2020", 8*6, 100, 2, 0);
    // PutText(0, 90, "********************************", LOGICAL_IMP);
    db_state = ReadyToSwitch;
    sf_wait(200);

    // Go to dungeon state.
    sf_set_game_state(Dungeon);
}

void sf_set_startscreen_state(void)
{
    sf_draw_startscreen_intro_text();
}

void sf_update_startscreen_state(void)
{
}
