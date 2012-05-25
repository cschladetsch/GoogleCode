#include "Precompiled.h"
#include "Sdl.h"

// SDL cursors.
SDL_Cursor* arrow_cursor;
SDL_Cursor* hand_cursor;

static const char *kHand[] = {
	"     XX                 ",
	"    X..X                ",
	"    X..X                ",
	"    X..X                ",
	"    X..XXXXXX           ",
	"    X..X..X..XXX        ",
	"XXX X..X..X..X..X       ",
	"X..XX..X..X..X..X       ",
	"X...X..X..X..X..X       ",
	" X..X..X..X..X..X       ",
	" X..X........X..X       ",
	" X..X...........X       ",
	" X..............X       ",
	" X.............X        ",
	" X.............X        ",
	"  X...........X         ",
	"   X.........X          ",
	"    X........X          ",
	"    X........X          ",
	"    XXXXXXXXXX          ",
};

static SDL_Cursor *init_system_cursor(const char *image[]) {
	int i = -1;
	Uint8 data[3*20];
	Uint8 mask[3*20];

	for ( int row=0; row<20; ++row ) {
		for ( int col=0; col<24; ++col ) {
			if ( col % 8 ) {
				data[i] <<= 1; mask[i] <<= 1;
			} else {
				++i;
				data[i] = mask[i] = 0;
			}
			switch (image[row][col]) {
			case '.':
				data[i] |= 0x01; mask[i] |= 0x01;
				break;
			case 'X':
				mask[i] |= 0x01;
				break;
			case ' ':
				break;
			}
		}
	}
	return SDL_CreateCursor(data, mask, 24, 20, 5, 0);
}


void CreateCursors()
{
	if (hand_cursor == NULL) hand_cursor = init_system_cursor(kHand);
	if (arrow_cursor == NULL) arrow_cursor = SDL_GetCursor();
}

//EOF
