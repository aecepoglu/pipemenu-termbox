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

#ifndef STREAMREADER_H
#define STREAMREADER_H

#include <stdio.h>
#include <termbox.h>
#include "item.h"

struct menu {
	struct item **items;
	int size;
	int count;
	int windowStart, windowEnd, selection;
	struct menu *prev;
	bool needsRefresh;
};

void freeMenu( struct menu* m);

struct menu *readStream(FILE *fp);
struct menu *executeCommand( char *cmd, bool isMenu);

void initReader();
void endReader();
#endif
