/*
author: aec
email: aecepoglu at fastmail dot fm
--
This file is part of pipemenu-termbox.

pipemenu-termbox is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

pipemenu-termbox is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with pipemenu-termbox.  If not, see <http://www.gnu.org/licenses/>.
*/

void quit();
void enter();
void leave();
void moveUp();
void moveDown();

/*=====================
	KEYBINDINGS

Keybindings are used to trigger the functions above.
*/
struct key {
	uint16_t key;
	uint32_t ch;
	void(*action)(void);
};



/*
'NUMKEYS' should match the number of keybinding entries.

Keybindings for simple ascii keys are have 'key' value '0'.
Their 'ch' value should be the ascii value

Special keys(arrow keys, for example) have a 'key' value 
as defined in 'termbox.h' file, and their 'ch' value is '0'.
*/
#define NUMKEYS 9
struct key keys[] = {
	{0, 'q', &quit},
	{0, 'n', &moveUp},
	{0, 't', &moveDown},
	{0, 'h', &leave},
	{0, 's', &enter},
	{TB_KEY_ARROW_UP, 0, &moveUp},
	{TB_KEY_ARROW_DOWN, 0, &moveDown},
	{TB_KEY_ARROW_LEFT, 0, &leave},
	{TB_KEY_ARROW_RIGHT, 0, &enter},
};


/*=====================
	COLORS

Normal-output-mode of termbox is used (TB_OUTPUT_NORMAL).
This means 8 basic color options defined in 'termbox.h' can be used.
(9, including TB_DEFAULT)
*/
#define COLOR_FG_NORMAL TB_WHITE
#define COLOR_FG_SELECTED TB_BLUE
#define COLOR_BG TB_DEFAULT
