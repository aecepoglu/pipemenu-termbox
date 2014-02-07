/*
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
#include <termbox.h>

#ifndef ITEM_H
#define ITEM_H

struct item {
	uint32_t *text;
	//char *text;
	int textLen;
	bool isMenu;					// Should teh output of this command be read to create sub-menus
	char *cmd;						// The command to run when item is entered. Null value means nothing to execute.
	bool wantsRefresh;
};

void printItem( struct item *myitem, int y, bool isSelected);
void setItemTitle( struct item *myitem, char *buf, int count);
void freeItem( struct item *myitem);

#endif //ITEM_H
