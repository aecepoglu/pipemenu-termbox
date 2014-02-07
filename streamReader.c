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
#include <unistd.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>

#include "csv.h"

#include "streamReader.h"
#include "item.h"


#define DEBUG(...) //printf(__VA_ARGS__)
#define MENUSIZE 16

struct csv_parser parser;

struct parsingData {
	struct menu *m;
	int fieldIndex;
} myParsingData;

FILE *LOGFP;

void freeMenu( struct menu *m) {
	int i;
	for(i=0; i<m->count; i++) {
		freeItem(m->items[i]);
	}
	free(m->items);
	free(m);
}

void cb_field(void *s, size_t i, void *data) {
	struct parsingData *pd = (struct parsingData*)data;
	switch( pd->fieldIndex ) {
		case 0:
			if( pd->m->count == pd->m->size) {
				pd->m->size *= 2;
				pd->m->items = (struct item**)realloc(pd->m->items, pd->m->size * sizeof(struct item*));
			}
			pd->m->items[pd->m->count] = (struct item*)malloc(sizeof(struct item));
			pd->m->items[pd->m->count]->isMenu = 0;
			pd->m->items[pd->m->count]->cmd = 0;
			setItemTitle( pd->m->items[pd->m->count], s, i);
			break;
		case 1:
			pd->m->items[pd->m->count]->isMenu = ((char*)s)[0] == '1';
			break;
		case 2:
			pd->m->items[pd->m->count]->cmd = strndup( s, i);
			break;
		case 3:
			pd->m->items[pd->m->count]->wantsRefresh = ((char*)s)[0] == '1';
			break;
	};
	pd->fieldIndex ++;
}


void cb_end(int c, void *data) {
	struct parsingData *pd = (struct parsingData*)data;
	pd->m->count ++;
	pd->fieldIndex = 0;
}

void initReader() {
	LOGFP=fopen("LOG.txt","a");
}
void endReader() {
	csv_free(&parser);
}

struct menu* readStream( FILE *fp) {
	csv_init(&parser, 0);
	struct menu *result = (struct menu*)malloc(sizeof(struct menu));

	result->size = MENUSIZE;
	result->items = (struct item**)calloc( MENUSIZE, sizeof(struct item*));
	result->count =0;
	result->windowStart = 0;
	result->selection = 0;

	myParsingData.m = result;
	myParsingData.fieldIndex = 0;

	char buf[BUFSIZ];
	int numRead;
	while( (numRead = fread(buf, 1, BUFSIZ, fp)) > 0 ) {
		csv_parse( &parser, buf, numRead, &cb_field, &cb_end, &myParsingData);
	}

	endReader();
	csv_fini(&parser, &cb_field, &cb_end, &myParsingData);
	return result;
}

/*
struct menu* readStream(FILE *fp) {
	struct menu *result = (struct menu*)malloc(sizeof(struct menu));

	result->size = MENUSIZE;
	result->items = (struct item**)calloc( MENUSIZE, sizeof(struct item*));

	regex_t *compiled = malloc(sizeof(regex_t));
	regcomp( compiled, "(.*),(0|1),(.*)\n", REG_EXTENDED);
	int subCount = 1+compiled->re_nsub;

	result->count =0;
	result->windowStart = 0;
	char buf[BUFSIZ];
	while( fgets( buf, BUFSIZ, fp) ) {
		DEBUG("\tbuf: <<%s>>\n", buf);
		regmatch_t matches[subCount];

		struct item *anItem = (struct item*)malloc(sizeof(struct item));
		if( result->count == result->size) {
			result->size *= 2;
			result->items = (struct item**)realloc(result->items, result->size * sizeof(struct item*));
		}

		if( regexec( compiled, buf, subCount, matches, 0) ) {
			setItemTitle( anItem, buf, strlen(buf)-1);
			anItem->cmd = 0;
			anItem->isMenu = 0;
		}
		else {
			setItemTitle( anItem, buf+matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
			anItem->cmd  = strndup( buf+matches[3].rm_so, matches[3].rm_eo - matches[3].rm_so);
			anItem->isMenu = buf[matches[2].rm_so] != '0';
		}

		result->items[result->count] = anItem;
		result->count ++;
		DEBUG("read %d items\n", result->count);
	}

	result->prev = 0;
	result->selection = 0;
}
*/

struct menu* executeCommand( char *cmd, bool isMenu) { //TODO error checking of stuff in here
	fprintf(LOGFP,"%s\n", cmd);
	struct menu *result = 0;

	int fd[2];
	int pid;

	if(isMenu)
		pipe(fd);
	
	pid = fork();
	if ( pid == 0) {
		if(isMenu) {
			close(fd[0]);
			close(1);
			dup(fd[1]);
		}
		else {
			close(1);
		}
		close(0);
		close(2);
		execl( "/bin/sh", "sh", "-c", cmd, NULL);
		
		exit(0);
	}
	else if(isMenu) {
		close(fd[1]);
		FILE *fp = fdopen( fd[0], "r");
		result = readStream( fp);
		fclose( fp);
	}
	waitpid( pid, 0, 0);

	return result;
}
