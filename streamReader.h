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
