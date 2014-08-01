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
#include <stdlib.h>
#include <inttypes.h> //just to print PRIu16 and whatnot

#include "streamReader.h"
#include "item.h"
#include "config.h"

struct menu *mymenu;
bool isRunning;

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

void freeItem( struct item *i) {
	free(i->text);
	if(i->cmd)
		free(i->cmd);
	free(i);
}

void printItem( struct item *item, int y, bool isSelected) {
	int i;
	for( i=0; i<item->textLen; i++) {
		tb_change_cell(i, y, item->text[i], isSelected ? TB_BLUE : TB_WHITE, TB_DEFAULT);
	}
}

void setItemTitle( struct item *myitem, char *buf, int count) {
	//DEBUG("%.*s\n", count, buf);
	myitem->text = calloc(count,sizeof(uint32_t));
	myitem->textLen = 0;
	char *target = buf + count;
	while( buf < target) {
		int x = tb_utf8_char_to_unicode( & myitem->text[myitem->textLen], buf);
		buf += x;
		myitem->textLen ++;
	}
}

void scroll(int dy) {
	mymenu->windowStart += dy;
	mymenu->windowEnd += dy;
}

void refreshMenu(void) {
	struct menu *prev = mymenu->prev;
	int sel = mymenu->selection;
	free( mymenu);
	mymenu = executeCommand( prev->items[prev->selection]->cmd, prev->items[prev->selection]->isMenu );
	mymenu->selection = sel < mymenu->count ? sel : (mymenu->count - 1);
}


void move( int x) {
    int target = mymenu->selection + x;
	//printf("\t\t%d for [%d,%d)\n", target, mymenu->windowStart, mymenu->windowEnd);
    if( target >= mymenu->windowEnd) {
        scroll( MIN( mymenu->count - mymenu->windowEnd , x));
        mymenu->selection = target;
    }
    else if( target < mymenu->windowStart) {
        scroll( MAX( -mymenu->windowStart, x)); // same as -MIN( mymenu->windowStart, -x) //remember x is <0
        mymenu->selection = target;
    }
    else {
        //printItem( mymenu->items[mymenu->selection], mymenu->selection - mymenu->windowStart, false);
        //printItem( mymenu->items[target], target - mymenu->windowStart, true);
        mymenu->selection = target;
    }
}

void quit(void) {
	isRunning = false;
}

void moveUp() {
	move( mymenu->selection == 0
		? mymenu->count - 1
		: -1);
}

void moveDown() {
	move( mymenu->selection == mymenu->count -1 
		? -mymenu->selection
		: +1);
}

void enter(void) {
	struct item *i = mymenu->items[mymenu->selection];
	if( i->cmd ) {
		mymenu->needsRefresh = i->wantsRefresh;
		struct menu *new = executeCommand( i->cmd, i->isMenu );
		if(new != 0) {
			new->prev = mymenu;
			new->windowEnd = mymenu->windowEnd - mymenu->windowStart;
			mymenu = new;
		}
		else if(mymenu->needsRefresh) {
			refreshMenu();
		}
	}
}

void leave(void) {
	if( mymenu->prev != 0) {
		struct menu *prev = mymenu->prev;
		freeMenu( mymenu);
		mymenu = prev;

		if(mymenu->needsRefresh) {
			refreshMenu();
		}
	}
}

#include <unistd.h>
int main( int argc, char *args[]) {
	initReader();
	mymenu = readStream( stdin);

	tb_init();
	mymenu->windowEnd = tb_height();
	struct tb_event event;

	int i,j,keyI;

	isRunning = true;
	while(isRunning) {

		tb_clear();
		for( i=mymenu->windowStart, j=0; i<mymenu->count && i<mymenu->windowEnd; i++, j++) {
			printItem( mymenu->items[i], j, i == mymenu->selection);
		}
		tb_present();

		tb_poll_event( &event);

		if( event.type == TB_EVENT_KEY) {
			for(keyI=0; keyI < NUMKEYS; keyI++) {
				if( keys[keyI].key == event.key && keys[keyI].ch == event.ch) {
					keys[keyI].action();
				}
			}
		}
		else {
			/* A window resize event */
			struct menu *tmp;
			for( tmp = mymenu; tmp!=0; tmp = tmp->prev){
				tmp->windowEnd = tmp->windowStart + event.h;
				if( tmp->selection >= tmp->windowEnd)
					scroll(tmp->selection - tmp->windowEnd);
			}
		}

	}
	tb_shutdown();

	//endReader();
	
	printf("ending...");
	return 0;
}
